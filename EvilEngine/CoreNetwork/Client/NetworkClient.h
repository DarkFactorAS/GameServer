
#ifndef EVILENGINE_CORENETWORK_CLIENT_NETWORKCLIENT
#define EVILENGINE_CORENETWORK_CLIENT_NETWORKCLIENT

#include "../Common/NetworkBase.h"
#include "EvilEngine/CoreLib/Signal/SignalTarget.h"

#include <stdlib.h>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

class NetworkConnection;
class BinaryStream;

#define USE_PACKETS true

class NetworkClient : public NetworkBase
{
public:

  enum NETWORK_MODE
  {
    NETWORKHOST_NONE = 0,
    NETWORKMODE_LOCALHOST,
    NETWORKMODE_LIVE,
    NETWORKMODE_TESTLIVE,
  };

  NetworkClient( const String& threadName );
  NetworkClient( const String& threadName, bool usePackets );
  virtual ~NetworkClient(){};

  virtual bool                    ConnectAsync();
  uint32                          GetInstance() const;
  virtual bool                    SendData( BinaryStream* data );
  virtual bool                    SendData( NetworkConnection* connection, BinaryStream* data ) DF_OVERRIDE;
  virtual void                    Disconnect();
  bool                            HasHostType( int networkType );
  bool                            SetCurrentHost( int networkType );
  int                             GetCurrentHost() const{ return m_CurrentHost; }
  bool                            AddNetworkHost( int networkType, const String& networkAddress, const String& networkPort );
  bool                            AddNetworkHost(int networkType, const String& networkAddressPort);
  std::pair<String,String>        GetNetworkHost( int networkType );

  bool                            IsConnected() const;
  bool                            MustReconnect() const;

  NetworkConnection*              GetConnection() const { return m_Connection; }
  virtual NetworkConnection*      GetConnection(uint32 /*instance*/) const{ return m_Connection; }

protected:

  NetworkConnection*              m_Connection;
  boost::asio::ip::tcp::resolver* m_Resolver;

  void                            HandleAsyncNameLookup(const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator iterator);
  void                            HandleAsyncConnect( const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator iterator, NetworkConnection* connection );

  int                             m_CurrentHost;
  std::map< int, std::pair< String, String > > m_NetworkHosts;
};

#endif /// EVILENGINE_CORENETWORK_CLIENT_NETWORKCLIENT
