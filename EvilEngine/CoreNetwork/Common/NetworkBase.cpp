/*************************************************************************************
* Project     : Evil Engine 2012
* File        : NetworkBase
* Description : Network class that contains the common logic for the network client and
*               server connection. This class is based on the boost networking
*               
* Author      : Thor Richard Hansen
 *************************************************************************************/

#include "Precompile.h"
#include "NetworkBase.h"
#include "NetworkConnection.h"
#include "CoreLib/BasicTypes/Mutex/ScopedMutex.h"

std::map< uint32, String >  NetworkBase::s_ErrorMessages;

/************************************************************************************************
* NetworkBase:
* Constructor for the NetworkBase class
* 
* @author Thor Richard Hansen
*************************************************************************************************/
NetworkBase::NetworkBase() :
  m_ThreadName("Network"),
  m_UsePackets(false)
{
}

/************************************************************************************************
* NetworkBase:
* Constructor for the NetworkBase class
* 
* @param (String)         threadName - Debug name of the thread this is running in
* 
* @author Thor Richard Hansen
*************************************************************************************************/
NetworkBase::NetworkBase( const String& threadName ) :
  m_ThreadName( threadName ),
  m_UsePackets(false)
{
}

/************************************************************************************************
* NetworkBase:
* Constructor for the NetworkBase class
*
* @param (String)         threadName - Debug name of the thread this is running in
* @param (bool)           usePackets - Set to true if the system is using network packets
*
* @author Thor Richard Hansen
*************************************************************************************************/
NetworkBase::NetworkBase(const String& threadName, bool usePackets ) :
  m_ThreadName(threadName),
  m_UsePackets(usePackets)
{
}

/************************************************************************************************
* CreateBinaryStream:
* Creates a binarystream to use for sending network data. Will also add the proper header if
* the system is using network packets
*
* @return (BinaryStream*) - Returns the binarystream to use
*
* @author Thor Richard Hansen
*************************************************************************************************/
BinaryStream* NetworkBase::CreateBinaryStream()
{
  BinaryStream* data = new BinaryStream();
  if ( m_UsePackets )
  {
    data->WriteUInt32(0); // packetsize
    data->WriteUInt8(NETWORKPACKETVERSION);
  }
  return data;
}

/************************************************************************************************
* CreateConnectionObject:
* Creates a new NetworkConnection object. Each network connection has its own
* instance of the NetwoorkConnection object.
* 
* @return (NetworkConnection*) - Returns the network connection
*
* @author Thor Richard Hansen
*************************************************************************************************/
NetworkConnection* NetworkBase::CreateConnectionObject()
{
  NetworkConnection* newConnection = new NetworkConnection( this );
  return newConnection;
}

/************************************************************************************************
* IsConnected:
* Returns true if the NetworkConnection is connected to the counterpart
* 
* @param  (uint32)              instance - Instance of the connection to check
* @return (bool)                         - Returns true if we are connected
*
* @author Thor Richard Hansen
*************************************************************************************************/
bool NetworkBase::IsConnected(uint32 instance) const
{
  NetworkConnection* connection = GetConnection(instance);
  if ( connection != NULL && connection->IsConnected())
  {
    return true;
  }
  return false;
}

/************************************************************************************************
* GetConnection:
* Get the connection associated with the id instance.
* 
* @param  (uint32)              instance - Instance of the connection to check
* @return (NetworkConnection*)           - Returns the network connection
*
* @author Thor Richard Hansen
*************************************************************************************************/
NetworkConnection* NetworkBase::GetConnection(uint32 instance) const
{
  if ( instance == 0 )
  {
    return NULL;
  }

  ScopedMutex mutex( &m_ConnectionMutex );

  ConnectionMap::const_iterator iFind = m_Connections.find(instance);
  if (iFind != m_Connections.end())
  {
    return iFind->second;
  }
  return NULL;
}

/************************************************************************************************
* RemoveConnection:
* Removes a specific network connection from the list. Note that this will not 
* disconnect the client, it will just stop handling its messages
* 
* @param  (uint32)              instance - Instance of the connection to check
* @return (NetworkConnection*)           - Returns the network connection
*
* @author Thor Richard Hansen
*************************************************************************************************/
NetworkConnection* NetworkBase::RemoveConnection(uint32 instance)
{
  ScopedMutex mutex( &m_ConnectionMutex );

  ConnectionMap::iterator iFind = m_Connections.find(instance);
  if (iFind != m_Connections.end()) 
  {
    NetworkConnection* connection = iFind->second;
    m_Connections.erase(iFind);
    return connection;
  }
  return NULL;
}

/************************************************************************************************
* DisconnectClient:
* Disconnect the client from its counterpart
* 
* @param  (uint32)              instance                  - Instance of the connection
*                                                           to check
* @param  (bool)                disconnectWhenDoneSending - When everyting is 
*                                                           written to the counterpart
*                                                           disconnect the client
*
* @author Thor Richard Hansen
*************************************************************************************************/
void NetworkBase::DisconnectClient(uint32 instance, bool disconnectWhenDoneSending )
{
  NetworkConnection* connection = GetConnection(instance);
  if (connection != NULL )
  {
    BinaryStream* partialDataStream = connection->GetPendingStream();
    if (partialDataStream != NULL )
    {
      if (partialDataStream->GetSize() > 4)
      {
        uint32 expectedDataLength = partialDataStream->ReadUInt32(0);
        uint32 readPartialDataLeft = partialDataStream->GetReadDataLeft();
        uint32 percent = (readPartialDataLeft * 100) / expectedDataLength;

        LogInfoFMT("NetworkBase", "[%s][NetID:%d] DisconnectClient : Received partial datastream : %d / %d ( Partial : %d )", m_ThreadName.c_str(), connection->GetInstance(), readPartialDataLeft, expectedDataLength, percent);
        OnReceivedPartialNetworkData(instance, partialDataStream);
      }

      delete partialDataStream;
      connection->SetPendingStream( NULL );
    }

    bool isConnected = connection->IsConnected();
    connection->Disconnect( disconnectWhenDoneSending );
    if ( isConnected )
    {
      OnDisconnect( connection->GetInstance() );
    }
  }
}

/************************************************************************************************
* LinkConnection:
* Link the status changes for a connection to the client/server so that they can be notified
*
* @param  (NetworkConnection)   connection                - The connection to link
*
* @author Thor Richard Hansen
*************************************************************************************************/
void NetworkBase::LinkConnection( NetworkConnection* connection )
{
  connection->SignalConnectionChanged.Connect(this, &NetworkBase::SlotConnectionChanged);
}

void NetworkBase::SlotConnectionChanged(NetworkConnection* connection, uint32 status )
{
  String str = String::FormatString( "[%s][NetID:%d] Connection '%s' received status %d/", 
                                    m_ThreadName.c_str(), 
                                    connection->GetInstance(), 
                                    connection->IpAddress().c_str(),
                                    status );

  switch( status )
  {
    case NetworkConnection::NETWORKSTATUS_DISCONNECTED:
      LogInfoFMT("NetworkBase", "%s/DISCONNECTED", str.c_str() );
      QueueCommand(connection->GetInstance(), NETWORKPACKETTYPE_DISCONNECTED);
      break;
    case NetworkConnection::NETWORKSTATUS_RESOLVINGHOSTNAME:
      //LogInfoFMT("NetworkBase", "%s/RESOLVING HOSTNAME", str.c_str() );
      QueueCommand(connection->GetInstance(), NETWORKPACKETTYPE_RESOLVINGHOSTNAME);
      break;
    case NetworkConnection::NETWORKSTATUS_CONNECTING:
      //LogInfoFMT("NetworkBase", "%s/CONNECTING", str.c_str() );
      QueueCommand(connection->GetInstance(), NETWORKPACKETTYPE_CONNECTING);
      break;
    case NetworkConnection::NETWORKSTATUS_CONNECTED:
      //LogInfoFMT("NetworkBase", "%s/CONNECTED", str.c_str() );
      QueueCommand(connection->GetInstance(), NETWORKPACKETTYPE_CONNECTED);
      break;
    case NetworkConnection::NETWORKSTATUS_FAILED:
      LogInfoFMT("NetworkBase", "%s/FAILED", str.c_str() );
      QueueCommand(connection->GetInstance(), NETWORKPACKETTYPE_FAILED);
      break;
    default:
      LogErrorFMT("NetworkBase", "Unknown networkstatus %d!", status );
      break;
  }
}

/************************************************************************************************
* Run:
* Runs and handles all connections. This will read and write all the network 
* messages for every client
* 
* @return  (bool) - Returns true if the client/server should still be running. If
*                   this is returning false, the network interface will shut down.
*
* @author Thor Richard Hansen
*************************************************************************************************/
bool NetworkBase::Run()
{
  if ( !m_NewConnections.empty() )
  {
    ScopedMutex mutex( &m_ConnectionMutex );
    m_Connections.insert( m_NewConnections.begin(), m_NewConnections.end() );
    m_NewConnections.clear();
  }

  for (ConnectionMap::iterator it = m_Connections.begin(); it != m_Connections.end();)
  {
    NetworkConnection* connection = it->second;
    if ( connection == NULL )
    {
      //m_NetParent->RemoveConnection( m_Instance );
      it++;
      continue;
    }
    else if ( connection->RemoveConnection() )
    {
      DisconnectClient( it->first, false );
      ScopedMutex mutex( &m_ConnectionMutex );
      ConnectionMap::iterator itDel = it++;
      m_Connections.erase( itDel );
      continue;
    }
    else if ( !connection->Run() )
    {
      DisconnectClient( it->first, false );
      if ( connection->RemoveConnection() )
      {
        ScopedMutex mutex( &m_ConnectionMutex );
        ConnectionMap::iterator itDel = it++;
        m_Connections.erase( itDel );
        continue;
      }
    }
    it++;
  }

  return true;
}

/************************************************************************************************
* MakeIpAddressStr:
* Return the IP address from a connection
* 
* @param  (uint32) instance - Instance of the connection to check
* @return (String)          - Return the ip in a dotted notation ( like 192.168.1.1 ) 
*
* @author Thor Richard Hansen
*************************************************************************************************/
String NetworkBase::MakeIpAddressStr(uint32 instance) const
{
  NetworkConnection* connection = GetConnection(instance);
  if (connection != NULL)
  {
    return connection->IpAddress();
  }
  return String::zero;
}

/************************************************************************************************
* QueueCommand:
* To avoid dispatching network data/messages in the network thread, they are queued
* internally and processed in the FrameProcess function ( that is executed from the 
* main thread usually ).
* 
* @param  (uint32)              connectionId  - ID of the connection that wants to queue this message
* @param  (uint32)              messageId     - What kind of message that is queued
*
* @author Thor Richard Hansen
*************************************************************************************************/
void NetworkBase::QueueCommand( uint32 connectionId, uint32 messageId )
{
  MessageData message( connectionId, messageId, NULL );
  {
    ScopedMutex dataMutex( &m_DataMutex );
    m_QueuedPackets.push_back( message );
  }
}

/*************************************************************************************************
* QueueData:
* To avoid dispatching network data/messages in the network thread, they are queued
* internally and processed in the FrameProcess function ( that is executed from the
* main thread usually ).
*
* @param  (uint32)              connectionId  - ID of the connection that wants to queue this message
* @param  (const BinaryStream*) data          - The binarystream with this message
*
* @author Thor Richard Hansen
*************************************************************************************************/
void NetworkBase::QueueData( NetworkConnection* connection, const BinaryStream* data)
{
  if ( m_UsePackets )
  { 
    HandlePackedData( connection, data );
  }
  else
  {
    MessageData message( connection->GetInstance(), NETWORKPACKETTYPE_DATA, new BinaryStream(data));
    {
      ScopedMutex dataMutex(&m_DataMutex);
      m_QueuedPackets.push_back(message);
    }
  }
}


bool NetworkBase::SendData(NetworkConnection* connection, BinaryStream* data)
{
  if (data != NULL)
  {
    // If packets are enabled, write the packet size
    if ( m_UsePackets )
    {
      data->WriteUInt32(0, data->GetSize() - 4);
    }

    if (connection != NULL)
    {
      return connection->SendData(data);
    }
  }
  return false;
}


void NetworkBase::OnRemoteHostDisconnected(uint32 /*instanceId*/)
{
  SignalConnectChanged(false);
}

void NetworkBase::OnConnected(uint32 /*instanceId*/)
{
  SignalConnectChanged(true);
}

/************************************************************************************************
* OnConnectionFailed:
* Send a signal that the rest of the code can listen for when we fail to connect
* to a server
*
* @param (uint32) instanceId - ID of the connection that failed
*
* @author Thor Richard Hansen
*************************************************************************************************/
//void NetworkBase::OnConnectionFailed(uint32 /*instanceId*/)
//{
//  SignalConnectionFailed();
//}

std::vector< NetworkBase::MessageData > NetworkBase::PopQueuedPackets()
{
  std::vector< NetworkBase::MessageData >  networkPackets;
  {
    ScopedMutex dataMutex(&m_DataMutex);
    std::swap(networkPackets, m_QueuedPackets);
  }
  return networkPackets;
}

void NetworkBase::HandlePackedData(NetworkConnection* connection, const BinaryStream* dataStream)
{
  BinaryStream* pendingStream = connection->GetPendingStream();
  if (pendingStream != NULL)
  {
    pendingStream->WriteRaw(dataStream);
    dataStream = pendingStream;
  }

  while (dataStream->GetReadDataLeft() > 0)
  {
    uint32 expectedDataLength = dataStream->ReadUInt32();

    if (expectedDataLength == 0 )
    {
      LogErrorFMT("NetworkBase", "PacketNetworkBase:Expected datalength (%d) is 0!", expectedDataLength);
      break;
    }

    // If we expect blobs larger than 10MB, stuff is broken
    if (expectedDataLength > BinaryStream::MAX_DATA_SIZE)
    {
      LogErrorFMT("NetworkBase", "PacketNetworkBase:Expected datalength (%d) exceeds limit of %d", expectedDataLength, BinaryStream::MAX_DATA_SIZE);
      break;
    }

    uint32 readDataLeft = dataStream->GetReadDataLeft();
    if (expectedDataLength > readDataLeft)
    {
      dataStream->SetReadPos(dataStream->GetReadPos() - 4); // Rewind the datalength read
      if (dataStream->GetReadPos() > 0 || pendingStream == NULL)
      {
        uint32 readPartialDataLeft = dataStream->GetReadDataLeft();
        pendingStream = new BinaryStream(dataStream->GetData() + dataStream->GetReadPos(), readPartialDataLeft);
        pendingStream->SetWritePos(readPartialDataLeft);
        connection->SetPendingStream(pendingStream);

        uint32 percent = (readDataLeft * 100) / expectedDataLength;
        LogInfoFMT("NetworkBase", "[%s][NetID:%d] Create datastream : %d / %d ( %d %% )", m_ThreadName.c_str(), connection->GetInstance(), readDataLeft, expectedDataLength, percent);
      }
      //else // Temp - To track down issue with incomplete datastreams
      //{
      //  uint32 percent = (readDataLeft * 100) / expectedDataLength;
      //  LogInfoFMT("NetworkBase", "[%s][NetID:%d] Datastream : %d / %d ( %d %% )", m_ThreadName.c_str(), connection->GetInstance(),readDataLeft, expectedDataLength, percent);
      //}
      return;
    }

    // Temp - To track down issue with incomplete datastreams
    if (pendingStream != NULL)
    {
      uint32 percent = (readDataLeft * 100) / expectedDataLength;
      LogInfoFMT("NetworkBase", "[%s][NetID:%d] Datastream : complete %d / %d ( %d %% )", m_ThreadName.c_str(), connection->GetInstance(), readDataLeft, expectedDataLength, percent);
    }

    uint32 seekPosition = dataStream->GetReadPos();
    uint8 version = dataStream->ReadUInt8();
    if (version != NETWORKPACKETVERSION)
    {
      LogErrorFMT("NetworkBase", "[%s][NetID:%d] Sender sendt data with wrong version (%d). Expected version %d", m_ThreadName.c_str(), connection->GetInstance(), version, NETWORKPACKETVERSION);
      break;
    }

    if (dataStream->IsOK())
    {
      MessageData message(connection->GetInstance(), NETWORKPACKETTYPE_DATAPACKET, new BinaryStream(dataStream->GetData() + dataStream->GetReadPos(), expectedDataLength  ) );
      {
        ScopedMutex dataMutex(&m_DataMutex);
        m_QueuedPackets.push_back(message);
      }
    }
    else
    {
      LogErrorFMT("NetworkBase", "[%s][NetID:%d] Datastream packet was not OK!", m_ThreadName.c_str(), connection->GetInstance());
    }

    dataStream->SetReadPos(seekPosition + expectedDataLength);
  }

  // Delete the blob
  connection->ReleasePendingStream();
}

/***********************************************************************************
* CreatePacket:
* Create a new BinaryStream with the proper header when sending bugreport
*
* @param  (uint32)        packetType - When kind of packet this is
* @return (BinaryStream)             - The connection object to use
*
* @author Thor Richard Hansen
***********************************************************************************/
BinaryStream* NetworkBase::CreatePacket(uint32 packetType)
{
  BinaryStream* data = CreateBinaryStream();
  data->WriteUInt32(packetType);
  return data;
}

/************************************************************************************************
 * GetErrorString:
 * Returns the errorcode as a human readable string
 *
 * @param  (uint32)        errorCode - ID of the error
 * @return (String)                  - The readable string
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
const String& NetworkBase::GetErrorString( uint32 errorCode )
{
  std::map< uint32, String >::iterator itFound = s_ErrorMessages.find( errorCode );
  if ( itFound != s_ErrorMessages.end() )
  {
    return itFound->second;
  }
  
  dfBugreportFMT(false , "No errorstring for errorcode : %d", errorCode);
  String customString = String::FormatString("Unknown error (%d)", errorCode);
  RegisterNetworkErrorMessage( errorCode, customString );
  return GetErrorString( errorCode );
}

/************************************************************************************************
 * RegisterNetworkErrorMessage:
 * Register an errormessage with the error enum.
 *
 * @param  (uint32)              packetTypeId   - The ID of the packetType this error belongs to
 * @param  (const String&)       errorMessage   - The error to display to the user
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
void NetworkBase::RegisterNetworkErrorMessage( uint32 errorCode, const String& errorMessage )
{
  // TODO > Check if the string is the same as well If it is different assert here!
  std::map< uint32, String >::iterator itFound = s_ErrorMessages.find( errorCode );
  if ( itFound != s_ErrorMessages.end() )
  {
    LogWarningFMT( "NetworkBase::RegisterNetworkErrorMessage", "Error message [%d] is already registered (%s)", errorCode, errorMessage.c_str() );
  }
  s_ErrorMessages[ errorCode ] = errorMessage;
}

