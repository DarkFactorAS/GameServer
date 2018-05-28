/*************************************************************************************
* Project     : Evil Engine 2012
* File        : NetworkServer
* Description : Class that sets up a server socket and listens for incoming clients
*               connecting to the specified port. Each connection is handled in a
*               separate thread.
*               
* Author      : Thor Richard Hansen
*************************************************************************************/

#include "Precompile.h"
#include "NetworkServer.h"
#include "Common/NetworkConnection.h"
#include "Common/NetworkReactor.h"
#include "CoreLib/BasicTypes/Mutex/ScopedMutex.h"

/***********************************************************************************
* Constructor:
*
* @param (String)  threadName - A custom name for this thread. Used for debugging
* @param (uint16)  serverPort - Tee port this server should listen to 
*
* @author Thor Richard Hansen
***********************************************************************************/
NetworkServer::NetworkServer( const String& threadName, uint16 serverPort ) :
  NetworkBase( threadName ),
  m_Port( serverPort ),
  m_Acceptor( NetworkReactor::GetIOService() )
{
}

/***********************************************************************************
* Constructor:
*
* @param (String)  threadName - A custom name for this thread. Used for debugging
* @param (uint16)  serverPort - Tee port this server should listen to
* @param (bool)    usePackets - Set to true to use network packets
*
* @author Thor Richard Hansen
***********************************************************************************/
NetworkServer::NetworkServer(const String& threadName, uint16 serverPort, bool usePackets) :
  NetworkBase(threadName, usePackets),
  m_Port(serverPort),
  m_Acceptor(NetworkReactor::GetIOService())
{
}

NetworkServer::~NetworkServer()
{
  if ( IsRunning() )
  {
    Stop();
  }
}

bool NetworkServer::IsRunning() const
{
  return m_Acceptor.is_open();
}

bool NetworkServer::Start()
{
  boost::system::error_code errorCode;
  boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), m_Port);

  m_Acceptor.open(endpoint.protocol());
  m_Acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
  m_Acceptor.set_option(boost::asio::ip::tcp::no_delay(true));

  if ( m_Acceptor.bind( endpoint, errorCode) )
  {
    return false;
  }
  m_Acceptor.listen();

  StartAccept();

  LogInfoFMT( m_ThreadName, "Server started on > %s : %d", GetServerAddr().c_str(), GetPort());

  return true;
}

bool NetworkServer::Stop()
{
  if (m_Acceptor.is_open())
  {
    //ConnectionMap_t connectionMapCopy;
    //{
    //  SpinWriterLock guard( m_ConnectionMutex );
    //  connectionMapCopy = m_Connections;
    //}

    //ConnectionMap::iterator iConnection = connectionMapCopy.begin();
    ConnectionMap::iterator iConnection = m_Connections.begin();
    while (iConnection != m_Connections.end())
    {
      NetworkConnection* connection = iConnection->second;
      connection->Disconnect("Server shutdown");
      ++iConnection;
    }

    boost::system::error_code ec;
    m_Acceptor.close(ec);

    // we will wait for the cancel to say it caceled a timer before we continue
    // the reason is that the timer might not be running, and we could be in the middle of the
    // SlotRun function.  We are guaranteed that a timer runs (or will be started) if we reach this code.
    //while (m_Timer.cancel() == 0 )
    //{
    //  boost::this_thread::sleep(boost::posix_time::microseconds(1)); 
    //}
  }
  return true;
}

void NetworkServer::StartAccept()
{
  NetworkConnection* connection = CreateConnectionObject();
  //connection->SignalConnectionChanged.Connect(this, &NetworkServer::SlotConnectionChanged);
  LinkConnection(connection);

  m_Acceptor.async_accept(connection->GetSocket(), boost::bind(&NetworkServer::HandleAccept, this, boost::asio::placeholders::error, connection) );
}

void NetworkServer::HandleAccept(const boost::system::error_code& error, NetworkConnection* connection)
{
  if (!error)
  {
    // only need to lock while we insert into Connections map
    {
      ScopedMutex mutex( &m_ConnectionMutex );
      m_NewConnections[ connection->GetInstance() ] = connection;
    }
    connection->OnConnected();

    QueueCommand( connection->GetInstance(), NETWORKPACKETTYPE_CONNECTED);

    // Create a new listener for the next connection
    StartAccept();
  }
  else
  {
    if (error != boost::asio::error::operation_aborted) 
    {
      Stop();
    }
  }
}

String NetworkServer::GetServerAddr()
{
  boost::system::error_code ec;
  boost::asio::ip::tcp::endpoint endpoint = m_Acceptor.local_endpoint(ec);
  if (!ec)
  {
    return endpoint.address().to_string();
  }
  return "??";
}
