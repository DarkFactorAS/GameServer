
#ifndef EVILENGINE_CORENETWORK_NETWORKSERVER
#define EVILENGINE_CORENETWORK_NETWORKSERVER

#include "../Common/NetworkBase.h"

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

class NetworkConnection;
class BinaryStream;

class NetworkServer : public NetworkBase
{
public:
  NetworkServer( const String& threadName, uint16 serverPort );
  NetworkServer( const String& threadName, uint16 serverPort, bool usePackets );
  ~NetworkServer();

  /// Start the network server
  virtual bool                    Start() DF_OVERRIDE;
  /// Stop the network server
  virtual bool                    Stop() DF_OVERRIDE;
  /// Return true if the acceptor is still active
  virtual bool                    IsRunning() const;
  /// Return the address this server is listening on
  String                          GetServerAddr();
  /// Port this server is listening on
  uint16                          GetPort(){ return m_Port; }

protected:

  void                            HandleAccept(const boost::system::error_code& error, NetworkConnection* connection);

  virtual void                    StartAccept();

private:

  /// The port the server is listening on or client connecting to
  uint16                          m_Port;

  /// Boost object that is responsible for setting up the listener.
  boost::asio::ip::tcp::acceptor  m_Acceptor;
};

#endif /// EVILENGINE_CORENETWORK_NETWORKSERVER
