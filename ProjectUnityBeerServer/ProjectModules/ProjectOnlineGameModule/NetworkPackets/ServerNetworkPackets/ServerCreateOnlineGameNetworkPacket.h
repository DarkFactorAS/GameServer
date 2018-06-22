#ifndef PROJECT_ONLINEGAME_NETWORKPACKETS_SERVERCREATEONLINEGAMENETWORKPACKET
#define PROJECT_ONLINEGAME_NETWORKPACKETS_SERVERCREATEONLINEGAMENETWORKPACKET 1

#include "ServerBaseOnlineGameNetworkPacket.h"

class ServerCreateOnlineGameNetworkPacket : public ServerBaseOnlineGameNetworkPacket
{
public:

  ServerCreateOnlineGameNetworkPacket(uint32 lobbyGameId);
  ServerCreateOnlineGameNetworkPacket(const BinaryStream* datastream);

  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new ServerCreateOnlineGameNetworkPacket(datastream); }

  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("ServerCreateOnlineGame"); }
  virtual void                  Execute()         DF_OVERRIDE;
  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;

private:

  uint32                        m_LobbyGameId;

};

#endif /// PROJECT_ONLINEGAME_NETWORKPACKETS_SERVERCREATEONLINEGAMENETWORKPACKET#pragma once
