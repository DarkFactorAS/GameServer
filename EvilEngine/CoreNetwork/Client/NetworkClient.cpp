/*************************************************************************************
* Project     : Evil Engine 2012
* File        : NetworkClient
* Description : Network class that contains the logic to connect from a client to
*               a server.
*               
* Author      : Thor Richard Hansen
 *************************************************************************************/

#include "Precompile.h"
#include "EvilEngine/CoreLib/BasicTypes/String/StringList.h"
#include "NetworkClient.h"
#include "EvilEngine/CoreNetwork/Common/NetworkReactor.h"
#include "EvilEngine/CoreNetwork/Common/NetworkConnection.h"
#include "EvilEngine/CoreIO/Logger/Logger.h"

/***********************************************************************************
* NetworkClient:
* Constructor for the NetworkClient class
* 
* @param (String)         threadName - Debugname of the thread this is running in
* 
* @author Thor Richard Hansen
***********************************************************************************/
NetworkClient::NetworkClient( const String& threadName ) : 
  NetworkBase( threadName ),
  m_Connection(NULL)
{
  m_CurrentHost = NETWORKHOST_NONE;
}

/***********************************************************************************
* NetworkClient:
* Constructor for the NetworkClient class
*
* @param (String)         threadName - Debugname of the thread this is running in
*
* @author Thor Richard Hansen
***********************************************************************************/
NetworkClient::NetworkClient(const String& threadName, bool usePackets) :
  NetworkBase(threadName, usePackets),
  m_Connection(NULL)
{
  m_CurrentHost = NETWORKHOST_NONE;
}

/***********************************************************************************
* GetInstance:
* Returns the instance of the connection used for the client.
* 
* @return (uint32) - Returns the connection instance
* 
* @author Thor Richard Hansen
***********************************************************************************/
uint32 NetworkClient::GetInstance() const
{
  if ( m_Connection != NULL )
  {
    return m_Connection->GetInstance(); 
  }
  return 0;
}

bool NetworkClient::IsConnected() const
{
  return NetworkBase::IsConnected( GetInstance() );
}

bool NetworkClient::MustReconnect() const
{
  if ( m_Connection == NULL || m_Connection->MustReconnect() )
  {
    return true;
  }
  return false;
}

bool NetworkClient::HasHostType( int networkType )
{
  // Make sure we have this type
  std::map< int, std::pair< String, String > >::iterator itFound = m_NetworkHosts.find( networkType );
  if ( itFound != m_NetworkHosts.end() )
  {
    return true;
  }
  return false;
}

bool NetworkClient::SetCurrentHost( int networkType )
{
  if ( HasHostType( networkType) ) 
  {
    m_CurrentHost = networkType;
    return true;
  }
  return false;
}

bool NetworkClient::AddNetworkHost( int networkType, const String& networkAddress, const String& networkPort )
{
  // If we already have this addres, do not bother to update it
  std::map< int, std::pair< String, String > >::iterator itFound = m_NetworkHosts.find(networkType);
  if (itFound != m_NetworkHosts.end())
  {
    std::pair< String, String > keyValue = itFound->second;
    if (keyValue.first.EqualsNoCase(networkAddress) && keyValue.second.EqualsNoCase(networkPort))
    {
      return false;
    }
  }

  m_NetworkHosts[ networkType ] = std::pair< String, String > ( networkAddress, networkPort );
  if ( m_CurrentHost == NETWORKHOST_NONE )
  {
    m_CurrentHost = networkType;
  }
  return true;
}

bool NetworkClient::AddNetworkHost(int networkType, const String& networkAddressPort )
{
  StringList strList = StringList::Split( networkAddressPort, ":" );
  if ( strList.size() == 2 )
  {
    return AddNetworkHost( networkType, strList.at(0), strList.at(1) );
  }
  return false;
}

std::pair<String,String> NetworkClient::GetNetworkHost( int networkType )
{
  return m_NetworkHosts[ networkType ];
}

/***********************************************************************************
* ConnectAsync:
* Connects async to the server. This will resolve the hostname to correct ip and
* initiate the connect. The connection failure or success is handled in
* HandleAsyncNameLookup and HandleAsyncConnect.
* 
* @param  (String)  serverAddress - The ip/hostname of the server we want to connect to
* @param  (String)  serverPort    - The port of the server we want to connect to
* @return (bool)                  - Returns true if the connect initiated successfully 
* 
* @author Thor Richard Hansen
***********************************************************************************/
bool NetworkClient::ConnectAsync()
{
  if ( m_Connection != NULL && m_Connection->IsConnected() )
  {
    m_Connection->Disconnect();
    delete m_Connection;
    m_Connection = NULL;
  }

  if ( m_CurrentHost == NETWORKHOST_NONE )
  {
    return false;
  }

  std::pair< String, String > networkHost = GetNetworkHost( m_CurrentHost );

  // Create a new connection object
  m_Connection = CreateConnectionObject();
  LinkConnection( m_Connection );
  m_Connection->SetNetworkStatus( NetworkConnection::NETWORKSTATUS_RESOLVINGHOSTNAME );
  
  LogInfoFMT("NetworkClient","[%s] Trying to resolve %s", m_ThreadName.c_str(), networkHost.first.c_str() );

  boost::asio::ip::tcp::resolver::query query( networkHost.first.c_str(), networkHost.second.c_str());
  m_Resolver = new boost::asio::ip::tcp::resolver( NetworkReactor::GetIOService() );
  m_Resolver->async_resolve(query, boost::bind(&NetworkClient::HandleAsyncNameLookup, this, boost::asio::placeholders::error, boost::asio::placeholders::iterator));
  return true;
}

void NetworkClient::Disconnect()
{
  if ( m_Connection != NULL )
  {
    m_Connection->Disconnect();
  }
}

/***********************************************************************************
* HandleAsyncNameLookup:
* Callback function for when the name resolve/lookup has succeeded or failed. If
* the lookup fails, the connection will disconnect, otherwise it will try to do 
* the actual connect
* 
* @param  (error_code)  error     - If we received an error, this is the errorcode
* @param  (iterator)    iterator  - Iterator to a list of resolved ip numbers
*                                   for the hostname
* 
* @author Thor Richard Hansen
***********************************************************************************/
void NetworkClient::HandleAsyncNameLookup(const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator iterator)
{
  if(!error)
  {
    while( iterator != boost::asio::ip::tcp::resolver::iterator() )
    {
      boost::asio::ip::tcp::endpoint endpoint = *iterator;
      boost::asio::ip::address addr= endpoint.address();
      if(addr.is_v4())
      {
        std::pair< String, String > networkHost = GetNetworkHost( m_CurrentHost );

        LogInfoFMT( "NetworkClient", "[%s] Connecting to %s:%s (%s) ( ipv4 )", m_ThreadName.c_str(), networkHost.first.c_str(), networkHost.second.c_str(), addr.to_string().c_str());
        m_Connection->SetNetworkStatus( NetworkConnection::NETWORKSTATUS_CONNECTING );
        m_Connection->GetSocket().async_connect(endpoint, boost::bind( &NetworkClient::HandleAsyncConnect, this, boost::asio::placeholders::error, ++iterator, m_Connection));

        delete m_Resolver;
        m_Resolver = NULL;

        return;
      }
      else
      {
        LogInfoFMT( "NetworkClient", "[%s] Ignoring ipv6 connection : %s", m_ThreadName.c_str(), addr.to_string().c_str());
      }

      iterator++;
    }
  }
  
  // If we get an error and we didnt manually abort it
  if (error != boost::asio::error::operation_aborted)
  {
    m_Connection->Disconnect( error.message() );
    QueueCommand( m_Connection->GetInstance(), NetworkConnection::NETWORKSTATUS_FAILED );
  }
  delete m_Resolver;
  m_Resolver = NULL;
}

/***********************************************************************************
* HandleAsyncNameConnect:
* Callback function for when connection either succeeded or failed.
* 
* @param  (error_code)        error       - If we received an error, this is the errorcode
* @param  (iterator)          iterator    - Iterator to a list of resolved ip numbers
*                                           for the hostname
* @param  (NetworkConnection) connection  - Pointer to the connection that initated
*                                           the connection.
* 
* @author Thor Richard Hansen
***********************************************************************************/
void NetworkClient::HandleAsyncConnect( const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator iterator, NetworkConnection* connection )
{
  if(!error)
  {
    m_ConnectionMutex.Aquire();
    m_Connections[ connection->GetInstance() ] = connection;
    m_ConnectionMutex.Release();

    connection->SetNetworkStatus( NetworkConnection::NETWORKSTATUS_CONNECTED );
    connection->OnConnected();
    std::pair< String, String > networkHost = GetNetworkHost(m_CurrentHost);
    LogInfoFMT("NetworkClient", "[%s] Connection to %s:%s (%s) established.", m_ThreadName.c_str(), networkHost.first.c_str(), networkHost.second.c_str(), connection->IpAddress().c_str());
  }
  // do nothing, this only happens when cancel is called on Socket, which only happens during dtor.
  else if (error == boost::asio::error::operation_aborted)
  {
    SignalConnectChanged( false );
    return;
  }
  else if( iterator != boost::asio::ip::tcp::resolver::iterator() && connection != NULL)
  {
    std::pair< String, String > networkHost = GetNetworkHost( m_CurrentHost );
    LogInfoFMT("NetworkClient", "[%s] Connection to %s:%s failed. Reason = %s", m_ThreadName.c_str(), networkHost.first.c_str(), networkHost.second.c_str(), error.message().c_str() );
    boost::asio::ip::tcp::endpoint endpoint = *iterator;
    LogInfoFMT("NetworkClient", "[%s] Connecting to next ipaddress : %s", m_ThreadName.c_str(), endpoint.address().to_string().c_str() );

    m_Connection->GetSocket().async_connect(endpoint, boost::bind( &NetworkClient::HandleAsyncConnect, this, boost::asio::placeholders::error, ++iterator, m_Connection));
  }
  else
  {
    std::pair< String, String > networkHost = GetNetworkHost( m_CurrentHost );
    LogInfoFMT("NetworkClient", "[%s] Connection to %s:%s failed. Reason = %s", m_ThreadName.c_str(), networkHost.first.c_str(), networkHost.second.c_str(), error.message().c_str() );

    connection->SetNetworkStatus(NetworkConnection::NETWORKSTATUS_FAILED);
    connection->Disconnect( false );

    if (connection != NULL && connection->GetSocket().is_open())
    {
      connection->GetSocket().close();
    }
  }
}

/***********************************************************************************
* SendData:
* Will send the binaryStream over the network to the other end (client/server)
* 
* @param (BinaryStream) binaryStream - The data we want to send
*
* @author Thor Richard Hansen
***********************************************************************************/
bool NetworkClient::SendData( BinaryStream* binaryStream )
{
  return SendData( m_Connection, binaryStream);
}

bool NetworkClient::SendData( NetworkConnection* connection, BinaryStream* binaryStream )
{
  if ( binaryStream == NULL )
  {
    LogFatal( "NetworkClient", "Trying to send a NULL pointer over the network!" );
    return false;
  }

  if ( connection == NULL || connection->GetNetworkStatus() == NetworkConnection::NETWORKSTATUS_DISCONNECTED )
  {
    ConnectAsync();
  }
  else if ( connection != NULL )
  {
    NetworkBase::SendData( connection, binaryStream );
   // return m_Connection->SendData( binaryStream );
  }
  return false;
}
