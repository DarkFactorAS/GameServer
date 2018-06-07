#ifndef PROJECT_ONLINEGAME_NETWORKPACKETS_SERVERREQUESTGAMEDATA
#define PROJECT_ONLINEGAME_NETWORKPACKETS_SERVERREQUESTGAMEDATA 1

#include "ProjectUnityBeerServer/ProjectModules/ProjectGameManagementModule/NetworkPackets/BaseGameManagementNetworkPacket.h"

#include "ProjectUnityBeerServer/ProjectModules/ProjectOnlineGameModule/NetworkPackets/ServerNetworkPackets/ServerBaseOnlineGameNetworkPacket.h"

//#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/Data/Playfield.h"
//#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/WorldBuilderModule/ProjectWorldBuilderServerModule.h"



class ServerRequestGameDataNetworkPacket : public ServerBaseOnlineGameNetworkPacket
{
public:

  ServerRequestGameDataNetworkPacket(uint32 gameId);
  ServerRequestGameDataNetworkPacket(const BinaryStream* datastream);

  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new ServerRequestGameDataNetworkPacket(datastream); }

  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("ServerRequestGameData"); }
  virtual void                  Execute()         DF_OVERRIDE;
  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;

private:

  uint32  m_GameId;

};

#endif /// PROJECT_ONLINEGAME_NETWORKPACKETS_SERVERREQUESTGAMEDATA#pragma once
