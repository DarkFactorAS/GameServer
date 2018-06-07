#ifndef PROJECT_ONLINEGAME_NETWORKPACKETS_CLIENTRECEIVEDGAMEDATA_NETWORKPACKET
#define PROJECT_ONLINEGAME_NETWORKPACKETS_CLIENTRECEIVEDGAMEDATA_NETWORKPACKET 1

#include "ProjectUnityBeerServer/ProjectModules/ProjectGameManagementModule/NetworkPackets/BaseGameManagementNetworkPacket.h"
#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/Data/Playfield.h"
#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/WorldBuilderModule/ProjectWorldBuilderServerModule.h"

#include "ProjectUnityBeerServer/ProjectModules/ProjectOnlineGameModule/Data/OnlineGameData.hpp"

class ClientReceivedGameDataNetworkPacket : public BaseAuthentcatedNetworkPacket
{
public:

  ClientReceivedGameDataNetworkPacket(const OnlineGameData* gameData);
  ClientReceivedGameDataNetworkPacket(const BinaryStream* datastream);

  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new ClientReceivedGameDataNetworkPacket(datastream); }

  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("ClientReceivedGameData"); }
  virtual void                  Execute()         DF_OVERRIDE;
  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;

private:

  uint32  m_GameId;
  const OnlineGameData* m_GameData;
};

#endif /// PROJECT_ONLINEGAME_NETWORKPACKETS_CLIENTRECEIVEDGAMEDATA_NETWORKPACKET#pragma once
