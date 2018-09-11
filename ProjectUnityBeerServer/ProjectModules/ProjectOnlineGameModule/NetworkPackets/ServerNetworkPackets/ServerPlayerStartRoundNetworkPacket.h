#ifndef PROJECT_ONLINEGAME_NETWORKPACKETS_SERVER_PLAYERSTARTROUND_NETWORKPACKET
#define PROJECT_ONLINEGAME_NETWORKPACKETS_SERVER_PLAYERSTARTROUND_NETWORKPACKET 1

#include "ServerBaseOnlineGameNetworkPacket.h"

class ServerPlayerStartRoundNetworkPacket : public ServerBaseOnlineGameNetworkPacket
{
public:

  ServerPlayerStartRoundNetworkPacket(uint32 accountId, uint32 gameId);
  ServerPlayerStartRoundNetworkPacket(const BinaryStream* datastream);

  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new ServerPlayerStartRoundNetworkPacket(datastream); }

  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("ServerPlayerStartRound"); }
  virtual void                  Execute()         DF_OVERRIDE;
  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;

private:

  uint32                        m_GameId;
  uint32                        m_AccountId;
};

#endif /// PROJECT_ONLINEGAME_NETWORKPACKETS_SERVER_PLAYERSTARTROUND_NETWORKPACKET#pragma once
