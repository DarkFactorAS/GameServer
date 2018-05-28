#ifndef EVILENGINE_CORENETWORK_NETWORKENGINE
#define EVILENGINE_CORENETWORK_NETWORKENGINE

#include "../Common/NetworkBase.h"
#include "EvilEngine/CoreEngine/CoreEngine/CoreEngine.h"

class CoreNetworkEngine : public CoreEngine
{
public:

  CoreNetworkEngine();

  virtual bool                                      Run();
  virtual void                                      SetNetworkBase( NetworkBase* networkBase ){ m_NetworkBase = networkBase; }
  bool                                              SendPacket(uint32 connectionId, BinaryStream* data);

protected:

  virtual void                                      OnReceivedNetworkData(uint32 /*connectionInstance*/, const BinaryStream* /*dataStream*/) {};
  virtual void                                      OnReceivedNetworkPacket(uint32 /*connectionInstance*/, const BinaryStream* /*dataStream*/) {};
  virtual void                                      OnConnectionChanged( uint32 /*connectionInstance*/, uint32 /*conectionStatus*/ ){};

  NetworkBase*                                      m_NetworkBase;

private:

  void                                              SendNetworkPackets();

  /// NetworkConnectionInstance, BinaryStreamData
  std::vector< std::pair< uint32, BinaryStream* > > m_SendQueue;
};

#endif /// EVILENGINE_CORENETWORK_NETWORKENGINE