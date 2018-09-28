#ifndef PROJECT_ONLINEGAME_NETWORKPACKETS_SERVER_GMSTARTROUND_NETWORKPACKET
#define PROJECT_ONLINEGAME_NETWORKPACKETS_SERVER_GMSTARTROUND_NETWORKPACKET 1

#include "ServerBaseOnlineGameNetworkPacket.h"

class GMServerProgressPlayerStatusNetworkPacket : public ServerBaseOnlineGameNetworkPacket
{
public:

  GMServerProgressPlayerStatusNetworkPacket(uint32 gameId, uint32 thisStatus);
  GMServerProgressPlayerStatusNetworkPacket(const BinaryStream* datastream);

  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new GMServerProgressPlayerStatusNetworkPacket(datastream); }

  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("GMServerProgressPlayerStatus"); }
  virtual void                  Execute()         DF_OVERRIDE;
  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;

private:

  uint32                        m_GameId;
  uint32                        m_StatusId;
};

#endif /// PROJECT_ONLINEGAME_NETWORKPACKETS_SERVER_GMSTARTROUND_NETWORKPACKET#pragma once
