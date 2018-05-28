#ifndef EVILENGINE_CORENETWORK_NETWORKBASE
#define EVILENGINE_CORENETWORK_NETWORKBASE

//#define BOOST_LIB_DIAGNOSTIC

#include "EvilEngine/CoreLib/BasicTypes/Mutex/Mutex.h"
#include "EvilEngine/CoreLib/BasicTypes/BinaryStream/BinaryStream.h"
#include "EvilEngine/CoreLib/Signal/Signal.h"
#include "EvilEngine/CoreLib/Signal/VFConnector.h"
#include "EvilEngine/CoreLib/Signal/SignalTarget.h"
#include "EvilEngine/CoreLib/BasicTypes/Definitions/PlatformDefinition.h"

#include <list>
#include <map>
#include <vector>

class NetworkConnection;

class NetworkBase : public SignalTarget
{
public:

  enum NetworkVersion
  {
    NETWORKPACKETVERSION = 2
  };

  enum NETWORK_PACKET_TYPE
  {
    NETWORKPACKETTYPE_DATA,
    NETWORKPACKETTYPE_DISCONNECTED,
    NETWORKPACKETTYPE_RESOLVINGHOSTNAME,
    NETWORKPACKETTYPE_CONNECTING,
    NETWORKPACKETTYPE_CONNECTED,
    NETWORKPACKETTYPE_FAILED,
    NETWORKPACKETTYPE_DATAPACKET,
  };

  struct MessageData
  {
    uint32              m_ConnectionId;
    uint32              m_MessageId;
    const BinaryStream* m_Data;
    MessageData(uint32 connectionId, uint32 messageId, const BinaryStream* data) :
      m_ConnectionId(connectionId),
      m_MessageId(messageId),
      m_Data(data)
    {
    }
  };

  NetworkBase();
  NetworkBase( const String& threadName );
  NetworkBase( const String& threadName, bool usePackets );
  virtual ~NetworkBase(){};

  NetworkConnection*              RemoveConnection(uint32 instance);
  bool                            IsConnected(uint32 instance) const;
  const String&                   GetThreadName(){ return m_ThreadName; }

  // Depricated
  virtual void                    OnReceivedNetworkData( NetworkConnection* /*connection*/, BinaryStream* /*dataStream*/ ) DF_FINAL {};
  virtual void                    OnReceivedNetworkData(uint32 /*connectionInstance*/, BinaryStream* /*dataStream*/) DF_FINAL {};
  virtual void                    OnReceivedNetworkPacket(uint32 /*connectionInstance*/, const BinaryStream* /*dataStream*/) DF_FINAL{};
  virtual void                    OnReceivedNetworkData(uint32 /*connectionInstance*/, const BinaryStream* /*dataStream*/) DF_FINAL {};

  // Use these two
  virtual bool                    SendData(NetworkConnection* connection, BinaryStream* data);

  virtual void                    OnReceivedPartialNetworkData(uint32 /*connectionInstance*/, const BinaryStream* /*dataStream*/) {};

  virtual bool                    Start(){ return true;};
  virtual bool                    Stop(){ return true;};
  virtual bool                    Run();
  virtual void                    OnDisconnect( uint32 /*instanceId*/ ){};
  virtual NetworkConnection*      GetConnection(uint32 instance) const;

  void                            QueueCommand(uint32 connectionId, uint32 messageId);
  void                            QueueData(NetworkConnection* connection, const BinaryStream* data);

  /// Depricated
  void                            FrameProcess(){};

  BinaryStream*                   CreatePacket(uint32 packetType);
  BinaryStream*                   CreateBinaryStream();

  std::vector< NetworkBase::MessageData >             PopQueuedPackets();

  /// Static methods to read the errorstring from the errorcode
  static const String&            GetErrorString( uint32 errorCode );
  static void                     RegisterNetworkErrorMessage( uint32 errorCode, const String& errorMessage );

  
  Signal0< void >                 SignalConnectionFailed;
  Signal1< void, bool >           SignalConnectChanged;

protected:

  /// Routing functions from the connection in the network therad to the NetworkBase in the main thread
  void                            LinkConnection(NetworkConnection* connection);
  void                            SlotConnectionChanged(NetworkConnection* connection, uint32 status);

  /// These callbacks are called in the main thread
  /// These should be removed when projects are moved over to new network layout
  //virtual void                    OnConnectionFailed( uint32 instanceId );
  virtual void                    OnRemoteHostDisconnected( uint32 instanceId );
  virtual void                    OnConnected(uint32 instanceId);


  virtual NetworkConnection*      CreateConnectionObject();
  void                            DisconnectClient(uint32 instance, bool disconnectWhenDoneSending );

  String                          MakeIpAddressStr(uint32 instance) const;

  /// Create a binary stream with the correct header
  void                            SetUseNetworkPackets( bool usePackets){ m_UsePackets = usePackets; }
  bool                            GetUseNetworkPackets() const { return m_UsePackets; }
  void                            HandlePackedData(NetworkConnection* connection, const BinaryStream* /*dataStream*/);

  //uint32                          m_ThreadId;
  String                          m_ThreadName;
  Mutex                           m_ConnectionMutex;

  std::vector< MessageData >      m_QueuedPackets;
  Mutex                           m_DataMutex;

  // Connections
  typedef std::map<uint32, NetworkConnection*> ConnectionMap;
  ConnectionMap                   m_Connections;
  ConnectionMap                   m_NewConnections;

  /// Mutex that protects the read/write operation
  Mutex                           m_WriteMutex;
  Mutex                           m_ReadMutex;

  /// Read/Write queues
  std::list<BinaryStream>         m_WriteQueue;
  std::list<BinaryStream>         m_ReadQueue;

  /// If all network traffic is using packets
  bool                            m_UsePackets;

  // Static lookup map for error messages
  static std::map<uint32,String>  s_ErrorMessages;
};
#endif /// EVILENGINE_CORENETWORK_NETWORKBASE
