#ifndef PROJECT_ONLINEGAME_NETWORKPACKETS_CLIENTCREATEDONLINEGAME_NETWORKPACKET
#define PROJECT_ONLINEGAME_NETWORKPACKETS_CLIENTCREATEDONLINEGAME_NETWORKPACKET 1

#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/Packets/BaseNetworkPacket.h"
#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/Data/Playfield.h"
#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/WorldBuilderModule/ProjectWorldBuilderServerModule.h"

#include "ProjectUnityBeerServer/ProjectModules/ProjectOnlineGameModule/Data/OnlineGameData.hpp"

class ClientCreatedOnlineGameNetworkPacket : public BaseNetworkPacket
{
public:

  ClientCreatedOnlineGameNetworkPacket(uint32 lobbyGameId, uint32 onlineGameId);
  ClientCreatedOnlineGameNetworkPacket(const BinaryStream* datastream);

  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new ClientCreatedOnlineGameNetworkPacket(datastream); }

  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("ClientCreatedOnlineGame"); }
  virtual void                  Execute()         DF_OVERRIDE;
  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;

private:

  uint32  m_LobbyGameId;
  uint32  m_OnlineGameId;

};

#endif /// PROJECT_ONLINEGAME_NETWORKPACKETS_CLIENTCREATEDONLINEGAME_NETWORKPACKET#pragma once
