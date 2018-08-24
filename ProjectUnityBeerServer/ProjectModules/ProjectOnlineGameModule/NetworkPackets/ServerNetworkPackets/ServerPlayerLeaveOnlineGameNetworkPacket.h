#ifndef PROJECT_ONLINEGAME_NETWORKPACKETS_SERVERPLAYERLEAVEONLINEGAME
#define PROJECT_ONLINEGAME_NETWORKPACKETS_SERVERPLAYERLEAVEONLINEGAME 1

//#include "ProjectUnityBeerServer/ProjectModules/ProjectGameManagementModule/NetworkPackets/BaseGameManagementNetworkPacket.h"

#include "ProjectUnityBeerServer/ProjectModules/ProjectOnlineGameModule/NetworkPackets/ServerNetworkPackets/ServerBaseOnlineGameNetworkPacket.h"

//#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/Data/Playfield.h"
//#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/WorldBuilderModule/ProjectWorldBuilderServerModule.h"



class ServerPlayerLeaveOnlineGameNetworkPacket : public ServerBaseOnlineGameNetworkPacket
{
public:

  ServerPlayerLeaveOnlineGameNetworkPacket(uint32 gameId);
  ServerPlayerLeaveOnlineGameNetworkPacket(const BinaryStream* datastream);

  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new ServerPlayerLeaveOnlineGameNetworkPacket(datastream); }

  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("ServerPlayerLeaveOnlineGame"); }
  virtual void                  Execute()         DF_OVERRIDE;
  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;

private:

  uint32  m_GameId;

};

#endif /// PROJECT_ONLINEGAME_NETWORKPACKETS_SERVERPLAYERLEAVEONLINEGAME#pragma once
