#ifndef PROJECT_ONLINEGAME_NETWORKPACKETS_CLIENT_RECEIVEDACTIONCARD_NETWORKPACKET
#define PROJECT_ONLINEGAME_NETWORKPACKETS_CLIENT_RECEIVEDACTIONCARD_NETWORKPACKET 1

#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/Packets/BaseNetworkPacket.h"

#include "ProjectUnityBeerServer/ProjectModules/ProjectOnlineGameModule/Data/OnlineGameData.hpp"
#include "ProjectUnityBeerServer/ProjectModules/ProjectOnlineGameModule/Data/ActionCard.hpp"

#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/Data/Playfield.h"
#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/WorldBuilderModule/ProjectWorldBuilderServerModule.h"

class ClientReceivedActionCardsNetworkPacket : public BaseNetworkPacket
{
public:

  ClientReceivedActionCardsNetworkPacket(uint32 gameId, uint32 accountId, std::vector<ActionCard*> actionCardList);
  ClientReceivedActionCardsNetworkPacket(const BinaryStream* datastream);

  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new ClientReceivedActionCardsNetworkPacket(datastream); }

  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("ClientReceivedActionCards"); }
  virtual void                  Execute()         DF_OVERRIDE;
  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;

private:

  uint32                        m_GameId;
  uint32                        m_AccountId;
  std::vector<ActionCard*>      m_ActionCardList;
};

#endif /// PROJECT_ONLINEGAME_NETWORKPACKETS_CLIENT_RECEIVEDACTIONCARD_NETWORKPACKET#pragma once
