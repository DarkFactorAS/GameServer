#ifndef PROJECT_ONLINEGAME_NETWORKPACKETS_CLIENT_RECEIVEDACTIONCARD_NETWORKPACKET
#define PROJECT_ONLINEGAME_NETWORKPACKETS_CLIENT_RECEIVEDACTIONCARD_NETWORKPACKET 1

#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/Packets/BaseNetworkPacket.h"
#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/Data/Playfield.h"
#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/WorldBuilderModule/ProjectWorldBuilderServerModule.h"

#include "ProjectUnityBeerServer/ProjectModules/ProjectOnlineGameModule/Data/OnlineGameData.hpp"

class ClientReceivedActionCardsNetworkPacket : public BaseNetworkPacket
{
public:

  ClientReceivedActionCardsNetworkPacket(uint32 gameId, std::vector<uint32> actionCardList);
  ClientReceivedActionCardsNetworkPacket(const BinaryStream* datastream);

  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new ClientReceivedActionCardsNetworkPacket(datastream); }

  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("ClientReceivedActionCards"); }
  virtual void                  Execute()         DF_OVERRIDE;
  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;

private:

  uint32  m_GameId;
  std::vector<uint32> m_ActionCardList;
};

#endif /// PROJECT_ONLINEGAME_NETWORKPACKETS_CLIENT_RECEIVEDACTIONCARD_NETWORKPACKET#pragma once
