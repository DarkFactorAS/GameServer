#ifndef PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_CLIENTCREATEDQUICKGAMENETWORKPACKET
#define PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_CLIENTCREATEDQUICKGAMENETWORKPACKET 1

#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/Packets/BaseNetworkPacket.h"

class Playfield;
class LobbyGameData;

class ClientCreatedQuickGameNetworkPacket : public BaseNetworkPacket
{
public:

  ClientCreatedQuickGameNetworkPacket(uint32 gameId);
  ClientCreatedQuickGameNetworkPacket(const BinaryStream* datastream);

  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new ClientCreatedQuickGameNetworkPacket(datastream); }

  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;
  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("ClientCreatedQuickGame"); }
  virtual void                  Execute()         DF_OVERRIDE;

private:

  uint32                        m_GameId;
};

#endif /// PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_CLIENTCREATEDQUICKGAMENETWORKPACKET#pragma once
