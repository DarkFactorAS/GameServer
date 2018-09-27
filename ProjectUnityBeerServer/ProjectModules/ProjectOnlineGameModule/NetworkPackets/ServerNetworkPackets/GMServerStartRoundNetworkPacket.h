#ifndef PROJECT_ONLINEGAME_NETWORKPACKETS_SERVER_GMSTARTROUND_NETWORKPACKET
#define PROJECT_ONLINEGAME_NETWORKPACKETS_SERVER_GMSTARTROUND_NETWORKPACKET 1

#include "ServerBaseOnlineGameNetworkPacket.h"

class GMServerStartRoundNetworkPacket : public ServerBaseOnlineGameNetworkPacket
{
public:

  GMServerStartRoundNetworkPacket(uint32 gameId);
  GMServerStartRoundNetworkPacket(const BinaryStream* datastream);

  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new GMServerStartRoundNetworkPacket(datastream); }

  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("GMServerStartRound"); }
  virtual void                  Execute()         DF_OVERRIDE;
  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;

private:

  uint32                        m_GameId;
};

#endif /// PROJECT_ONLINEGAME_NETWORKPACKETS_SERVER_GMSTARTROUND_NETWORKPACKET#pragma once
