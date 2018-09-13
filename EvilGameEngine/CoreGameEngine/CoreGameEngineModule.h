/*************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : EngineModule
* Description : Abstract interface class for EngineModule modules. This interface
*               has to be implemented to be able to run as a sub component in the
*               CoreEngine.
*
* Author      : Thor Richard Hansen
*************************************************************************************/

#ifndef EVILENGINE_COREGAMEENGINE_COREGAMEENGINEMODULE
#define EVILENGINE_COREGAMEENGINE_COREGAMEENGINEMODULE 1

#include "EvilEngine/CoreEngine/CoreEngine/EngineModule.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/Packets/BaseNetworkPacket.h"

class CoreGameEngineModule : public EngineModule
{
public:

  CoreGameEngineModule(int moduleId) : EngineModule(moduleId)
  {
  };

  virtual ~CoreGameEngineModule() {};

  typedef BaseNetworkPacket*            NetworkPacket_fp(const BinaryStream*);
  void                                  RegisterPacketType(uint32 packetTypeId, NetworkPacket_fp* createInstance);
  void                                  DisconnectConnectionId(uint32 connectionId);
  virtual bool                          ReceivePacket(uint32 packetTypeId, uint32 connectionInstance, const BinaryStream* dataStream);
  std::map< uint32, NetworkPacket_fp* > m_PacketCreator;

  bool                                  SendPacketToClient(uint32 connectionId, BaseNetworkPacket* packet);
};

#endif /// EVILENGINE_COREGAMEENGINE_COREGAMEENGINEMODULE