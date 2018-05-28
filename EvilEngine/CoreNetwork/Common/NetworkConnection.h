#ifndef EVILENGINE_CORENETWORK_NETWORKCONNECTION
#define EVILENGINE_CORENETWORK_NETWORKCONNECTION

#include "EvilEngine/CoreLib/BasicTypes/Mutex/Mutex.h"
#include "EvilEngine/CoreLib/BasicTypes/String/CoreString.h"
#include "EvilEngine/CoreLib/Signal/Signal.h"

#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/smart_ptr/detail/atomic_count.hpp>
#include <boost/detail/atomic_count.hpp>

#include <list>
#include <vector>

class BinaryStream;
class NetworkBase;
class ZlibCompression;

class NetworkConnection
{
  friend class NetworkBase;

public:

  enum NETWORK_STATUS
  {
    NETWORKSTATUS_DISCONNECTED,
    NETWORKSTATUS_RESOLVINGHOSTNAME,
    NETWORKSTATUS_CONNECTING,
    NETWORKSTATUS_CONNECTED,
    NETWORKSTATUS_FAILED,
  };

  NetworkConnection( NetworkBase* parent );
  virtual ~NetworkConnection();

  /// ID of this connection
  uint32 GetInstance() const { return m_Instance; }
  uint32 GetConnectionId() const { return m_Instance; }

  /// Abstract functions that has to be implemented
  virtual void OnConnected();
  virtual void OnDisconnected(){ m_IsConnected = false; };
  virtual void OnReceivedNetworkData( BinaryStream& dataStream );

  /// Rename to disconnect
  virtual void Disconnect( const String& errorMessage, bool disconnectWhenDoneSending = false );
  virtual void Disconnect( bool disconnectWhenDoneSending = false );
  /// Send data to the other part
  virtual bool SendData( BinaryStream* data );

  String IpAddress() const;
  boost::asio::ip::tcp::socket& GetSocket() { return m_Socket; }
  bool IsConnected(){ return m_IsConnected; }
  bool IsDisconnected(){ return m_NetworkStatus == NETWORKSTATUS_DISCONNECTED; }
  void DisconnectWhenDoneSending(){ m_DisconnectWhenDoneSending = true; }

  bool Run();
  bool RemoveConnection(){ return m_RemoveConnection; }

  NetworkBase*                    GetParent(){ return m_NetParent; }

  void                            SetNetworkStatus( int status );
  int                             GetNetworkStatus() const{ return m_NetworkStatus; }
  bool                            MustReconnect();

  /// The stream used to pack/unpack multiple datapackets
  void                                          SetPendingStream(BinaryStream* pendingStream) { m_PendingStream = pendingStream; }
  BinaryStream*                                 GetPendingStream() { return m_PendingStream; }
  void                                          ReleasePendingStream();

protected:

  /// Functions to modify the data before sending/receiving ( like compression )
  virtual void HandleSendNetworkData( const BinaryStream& data );
  virtual void HandleReceivedNetworkData( BinaryStream& data );

  virtual void HandleRead(const boost::system::error_code& error, size_t bytes_transferred, NetworkConnection* connection );
  virtual void HandleWrite(const boost::system::error_code& errorCode, size_t bytesWritten, NetworkConnection* connection );

  /// Enable zlib compression for the network traffic
  void                                          EnableCompression();

  /// Return the name of the threadname
  String                                        GetThreadName();

  /// True if this connection is connected
  bool                                          m_IsConnected;

  /// True if this connection is to be removed
  bool                                          m_RemoveConnection;

  /// ID of the connection
  uint32                                        m_Instance;

  /// Socket
  boost::asio::ip::tcp::socket                  m_Socket;

  /// True if the connection should disconnect when it doesnt have more to send
  bool                                          m_DisconnectWhenDoneSending;

  /// The client or server part
  NetworkBase*                                  m_NetParent;

  /// Mutex that protects the read/write operation
  Mutex                                         m_WriteMutex;
  Mutex                                         m_ReadMutex;

  /// Read/Write queues
  std::list<BinaryStream>                       m_WriteQueue;
  std::list<BinaryStream>                       m_ReadQueue;

  /// Write queue
  mutable boost::detail::atomic_count           m_AsyncWriteCount;
  std::vector<boost::asio::const_buffer>        m_AsyncWriteQueue;

  static const size_t RECEIVE_BUFFER_SIZE = 1<<14; // 16k
  uint8 m_ReceiveBuffer[RECEIVE_BUFFER_SIZE];

  int                                           m_NetworkStatus;

private:

  Signal2< void, NetworkConnection*, uint32 >   SignalConnectionChanged;

  /// Pointer to the zlib compression class
  ZlibCompression*                              m_Compressor;

  /// Used to combine split packets for this connection
  BinaryStream*                                 m_PendingStream;
};

#endif /// EVILENGINE_CORENETWORK_NETWORKCONNECTION
