/*************************************************************************************
* Project     : Evil Engine 2012
* File        : NetworkConnection
* Description : Handles everything regarding a connection. All classes in the 
*               application/game that uses this network implementation will overload
*               this class to do specific readers/writers.
*               
* Author      : Thor Richard Hansen
*************************************************************************************/

#include "Precompile.h"
#include "NetworkConnection.h"
#include "NetworkBase.h"
#include "NetworkReactor.h"
#include "CoreLib/BasicTypes/BinaryStream/BinaryStream.h"
#include "EvilEngine/CoreLib/Thread/Thread.h"
#include "EvilEngine/CoreNetwork/ZLibCompression/ZLibCompression.h"

/***********************************************************************************
* Constructor:
* 
* @param (NetworkBase) parent - Pointer to the client or server owning this 
*                               connection.
*
* @author Thor Richard Hansen
***********************************************************************************/
NetworkConnection::NetworkConnection( NetworkBase* parent ) :
  m_IsConnected(false),
  m_RemoveConnection(false),
  m_Instance(0),
  m_Socket( NetworkReactor::GetIOService() ),
  m_DisconnectWhenDoneSending(false),
  m_NetParent( parent ),
  m_AsyncWriteCount(0),
  m_NetworkStatus(NETWORKSTATUS_DISCONNECTED),
  m_Compressor(NULL),
  m_PendingStream( NULL )
{ 
  static uint32 s_Instance = 0;
  m_Instance = ++s_Instance;
}

NetworkConnection::~NetworkConnection()
{
  if ( m_Compressor != NULL )
  {
    delete m_Compressor;
    m_Compressor = NULL;
  }
  ReleasePendingStream();
}

void NetworkConnection::ReleasePendingStream()
{
  delete m_PendingStream;
  m_PendingStream = NULL;
}

/***********************************************************************************
* GetThreadName:
* Returns the name of the thread this connection is running in
*
* @author Thor Richard Hansen
***********************************************************************************/
String NetworkConnection::GetThreadName()
{
  if ( m_NetParent != NULL )
  {
    return m_NetParent->GetThreadName();
  }
  return StaticStr("Unknown");
}

/***********************************************************************************
* EnableCompression:
* Turns on the ZLibCompression on the datastream if it is not enabled already
* 
* @author Thor Richard Hansen
***********************************************************************************/
void NetworkConnection::EnableCompression()
{
  if ( m_Compressor == NULL )
  {
    m_Compressor = new ZlibCompression();
  }
}

/***********************************************************************************
* Disconnect:
* Disconnect this connection from the server/client.
* 
* @param (String) errorMessage              - Text message to why the disconnect 
*                                             was called 
* @param (bool)   disconnectWhenDoneSending - If set to true, the connection will 
*                                             send all its data before disconnecting
*
* @author Thor Richard Hansen
***********************************************************************************/
void NetworkConnection::Disconnect( const String& /*errorMessage*/, bool disconnectWhenDoneSending )
{
  String threadName = GetThreadName();
  //LogErrorFMT("NetworkConnection","[%s][NetID:%d] Disconnect. Reason = '%s'", threadName.c_str(), m_Instance, errorMessage.c_str() );
  Disconnect( disconnectWhenDoneSending );
}

void NetworkConnection::SetNetworkStatus(int status) 
{
  m_NetworkStatus = status; 
  SignalConnectionChanged(this, status);
}

bool NetworkConnection::MustReconnect()
{
  switch( m_NetworkStatus )
  {
    case NetworkConnection::NETWORKSTATUS_DISCONNECTED:
    case NetworkConnection::NETWORKSTATUS_FAILED:
      return true;
    default:
      return false;
  }
}

/***********************************************************************************
* Disconnect:
* Disconnect this connection from the server/client.
* 
* @param (bool)   disconnectWhenDoneSending - If set to true, the connection will 
*                                             send all its data before disconnecting
*
* @author Thor Richard Hansen
***********************************************************************************/
void NetworkConnection::Disconnect( bool disconnectWhenDoneSending )
{
  if ( m_IsConnected && disconnectWhenDoneSending )
  {
    DisconnectWhenDoneSending();
    return;
  }

 
  if (m_IsConnected)
  {
    m_IsConnected = false;
    m_Socket.close();
    //SignalConnectionChanged( this, NETWORKSTATUS_DISCONNECTED );
    SetNetworkStatus(NetworkConnection::NETWORKSTATUS_DISCONNECTED);
    ReleasePendingStream();
  }

  m_RemoveConnection = true;

  m_WriteMutex.Aquire();
  m_WriteQueue.clear();
  m_WriteMutex.Release();
}

/***********************************************************************************
* OnConnected:
* Called when this connection connected successfully to the server/client.
* 
* @author Thor Richard Hansen
***********************************************************************************/
void NetworkConnection::OnConnected()
{
  String threadName = GetThreadName();
  Thread::SetThreadName( threadName.c_str() );  
  m_IsConnected = true; 

  LogErrorFMT("NetworkConnection", "[%s][NetID:%d] OnConnected.", threadName.c_str(), m_Instance);

  //SignalConnectionChanged( this, NETWORKSTATUS_CONNECTED );
  boost::asio::async_read(m_Socket, boost::asio::buffer(m_ReceiveBuffer, NetworkConnection::RECEIVE_BUFFER_SIZE), boost::asio::transfer_at_least(1), boost::bind(&NetworkConnection::HandleRead, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, this));
};

/***********************************************************************************
* SendData:
* Will send the binaryStream over the network to the other end (client/server)
* 
* @param (BinaryStream) binaryStream - The data we want to send
*
* @author Thor Richard Hansen
***********************************************************************************/
bool NetworkConnection::SendData( BinaryStream* binaryStream )
{
  if (binaryStream == NULL)
  {
    LogFatal("NetworkConnection", "Trying to send a NULL pointer over the network!");
  }
  if ( !m_DisconnectWhenDoneSending )
  {
    m_WriteMutex.Aquire();
    m_WriteQueue.push_back( binaryStream );
    m_WriteMutex.Release();
    return false;
  }
  return true;
}

/***********************************************************************************
* Run:
* Runs the connection, meaning handling all the data to be sent and the data that
* the connection received from the network
* 
* @return (bool) This connection will be active as long as it returns true. If the
*                connection returns false it is terminated.
*
* @author Thor Richard Hansen
***********************************************************************************/
bool NetworkConnection::Run()
{
  if ( !m_IsConnected )
  {
    return true;
  }

  // write stuff on the socket
  if ( m_AsyncWriteCount == 0 )
  {
    m_WriteMutex.Aquire();
    for ( std::list<BinaryStream>::const_iterator itData = m_WriteQueue.begin(); itData != m_WriteQueue.end(); itData++ )
    {
      const BinaryStream& data = *itData;
      HandleSendNetworkData( data );
    }
    m_WriteQueue.clear();
    m_WriteMutex.Release();

    if ( !m_AsyncWriteQueue.empty() )
    {
      ++m_AsyncWriteCount;
      boost::asio::async_write(m_Socket, m_AsyncWriteQueue, boost::bind(&NetworkConnection::HandleWrite, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, this));
    }
  }

  if (m_DisconnectWhenDoneSending)
  {
    Disconnect( false );
    return false;
  }


  if ( !m_ReadQueue.empty() )
  {
    m_ReadMutex.Aquire();
    for ( std::list<BinaryStream>::iterator itData = m_ReadQueue.begin(); itData != m_ReadQueue.end(); itData++ )
    {
      BinaryStream& data = *itData;
      HandleReceivedNetworkData( data );
    }
    m_ReadQueue.clear();
    m_ReadMutex.Release();
  }

  return true;
}

/***********************************************************************************
* HandleRead:
* Function that takes the data from the network and packs it into a BinaryStream. 
* This callback is called from the boost reactor when data is read.
* 
* @param (error_code)        error            - The errorcode if any from receiving the data
* @param (size_t)            byte_transferred - The number of bytes received from the network
* @param (NetworkConnection) connection       - The connection we received the data on
*
* @author Thor Richard Hansen
***********************************************************************************/
void NetworkConnection::HandleRead(const boost::system::error_code& error, size_t bytes_transferred, NetworkConnection* /*connection*/ )
{
  if ( bytes_transferred > 0 )
  {
    BinaryStream* readStream = new BinaryStream( m_ReceiveBuffer, (uint32) bytes_transferred );
    m_ReadMutex.Aquire();
    m_ReadQueue.push_back( readStream );
    m_ReadMutex.Release();

    boost::asio::async_read(m_Socket, boost::asio::buffer(m_ReceiveBuffer, NetworkConnection::RECEIVE_BUFFER_SIZE), boost::asio::transfer_at_least(1), boost::bind(&NetworkConnection::HandleRead, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, this));
  }
  else
  {
    Disconnect( String::FormatString("Broken read ( 0 bytes received : Error %d / %s )", error.value(), error.message().c_str()) , false );
  }
}

/***********************************************************************************
* HandleWrite:
* Callback function that is called after we have written data on the network socket
* 
* @param (error_code)        error        - The errorcode if any from sending the data
* @param (size_t)            byteWritten  - The number of bytes sent to the network
* @param (NetworkConnection) connection   - The connection we sent the data on
*
* @author Thor Richard Hansen
***********************************************************************************/
void NetworkConnection::HandleWrite(const boost::system::error_code& errorCode, size_t /*bytesWritten*/, NetworkConnection* /*connection*/ )
{
  m_AsyncWriteQueue.clear();
  --m_AsyncWriteCount;

  if(errorCode)
  {
    if (errorCode == boost::asio::error::operation_aborted || !m_IsConnected) 
    {
      String threadName = GetThreadName();
      LogInfoFMT("NetworkConnection", "[%s][NetID:%d] :: HandleWrite :: Network error = %s", threadName.c_str(),m_Instance, errorCode.message().c_str() );
      return;
    }
    Disconnect( errorCode.message() );
  }
}

/***********************************************************************************
* IpAddress:
* Returns a textstring representation of the IP formatted as a dotted number
* ( like "192.168.1.2" )
* 
* @return (String) - THe ip on the connection as text
*
* @author Thor Richard Hansen
***********************************************************************************/
String NetworkConnection::IpAddress() const
{
  if (m_IsConnected)
  {
    boost::system::error_code ec;
    boost::asio::ip::tcp::endpoint endpoint = m_Socket.remote_endpoint(ec);
    if (!ec)
    {
      return endpoint.address().to_string();
    }
  }

  return String::zero;
}

/***********************************************************************************
* HandleSendNetworkData:
* Virtual function that will push the binarystream data to the writequeue for
* the network reactor. This function can be overrided to compress or do other
* changes to the data before sending it.
* 
* @param (BinaryStream) data - The data we want to write to the network 
*
* @author Thor Richard Hansen
***********************************************************************************/
void NetworkConnection::HandleSendNetworkData( const BinaryStream& data )
{
  if ( m_Compressor != NULL )
  {
    uint32 dataSize = data.GetWritePos();
    BinaryStream* compressedStream = m_Compressor->Compress( (uint8*) data.ReadRaw( 0, dataSize ), dataSize );
    m_AsyncWriteQueue.push_back(boost::asio::const_buffer( compressedStream->GetData(), compressedStream->GetSize() ) );
    delete compressedStream;
  }
  else
  {
    m_AsyncWriteQueue.push_back(boost::asio::const_buffer( data.GetData(), data.GetSize() ) );
  }
}

/***********************************************************************************
* HandleReceivedNetworkData:
* Virtual function that will forward the data we received from the network. This
* function can be overriden to decompress or do other changes to the data before 
* using it on the connection/client/server.
* 
* @param (BinaryStream) data - The data we received from the network 
*
* @author Thor Richard Hansen
***********************************************************************************/
void NetworkConnection::HandleReceivedNetworkData( BinaryStream& data )
{
  if ( m_Compressor != NULL )
  {
    uint32 dataSize = data.GetSize();
    BinaryStream* dataStream = m_Compressor->Decompress( (uint8*) data.ReadRaw( 0, dataSize ), dataSize );
    if ( dataStream != NULL )
    {
      OnReceivedNetworkData( *dataStream );
      delete dataStream;
    }
    else 
    {
      Disconnect( String::FormatString("%s : Failed to decompress the datastream (%d bytes)", IpAddress().c_str(), data.GetSize() ) );
    }
  }
  else
  {
    OnReceivedNetworkData( data );
  }
}

/************************************************************************************************
 * OnReceivedNetworkData:
 * Handles the data after it is received on the network thread. The base implementation is to 
 * push the data back to the main thread/network client/server.
 *
 * @param  (BinaryStream)   dataStream  - The datablob we just read from the network
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
void NetworkConnection::OnReceivedNetworkData( BinaryStream& dataStream )
{
  if ( m_NetParent != NULL )
  {
    m_NetParent->QueueData( this, &dataStream );
  }
  else
  {
    LogFatalFMT("NetworkConnection","Connection[%d] Failed to handle network data read", m_Instance );
  }
}
