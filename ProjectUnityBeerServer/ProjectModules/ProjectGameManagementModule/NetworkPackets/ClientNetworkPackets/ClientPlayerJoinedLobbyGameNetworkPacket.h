#ifndef PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_CLIENTJOINEDLOBBYGAMENETWORKPACKET
#define PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_CLIENTJOINEDLOBBYGAMENETWORKPACKET 1

#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/Packets/BaseNetworkPacket.h"

class Playfield;
class LobbyGameData;
class LobbyGamePlayer;

class ClientPlayerJoinedLobbyGameNetworkPacket : public BaseNetworkPacket
{
public:

  ClientPlayerJoinedLobbyGameNetworkPacket(uint32 gameId, LobbyGamePlayer* lobbyPlayer);
  ClientPlayerJoinedLobbyGameNetworkPacket(const BinaryStream* datastream);
  ~ClientPlayerJoinedLobbyGameNetworkPacket();

  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new ClientPlayerJoinedLobbyGameNetworkPacket(datastream); }

  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;
  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("ClientPlayerJoinedLobbyGameNetwork"); }
  virtual void                  Execute()         DF_OVERRIDE;

private:

  uint32                        m_GameId;
  LobbyGamePlayer*              m_LobbyPlayer;

};

#endif /// PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_CLIENTJOINEDLOBBYGAMENETWORKPACKET#pragma once
