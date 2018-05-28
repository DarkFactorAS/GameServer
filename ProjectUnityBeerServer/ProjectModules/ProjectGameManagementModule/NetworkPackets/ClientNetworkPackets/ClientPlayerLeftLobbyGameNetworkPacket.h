#ifndef PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_CLIENTPLAYERLEFTLOBBYGAMENETWORKPACKET
#define PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_CLIENTPLAYERLEFTLOBBYGAMENETWORKPACKET 1

#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/Packets/BaseNetworkPacket.h"

class Playfield;
class LobbyGameData;
class LobbyGamePlayer;

class ClientPlayerLeftLobbyGameNetworkPacket : public BaseNetworkPacket
{
public:

  ClientPlayerLeftLobbyGameNetworkPacket(uint32 gameId, LobbyGamePlayer* lobbyPlayer);
  ClientPlayerLeftLobbyGameNetworkPacket(const BinaryStream* datastream);
  ~ClientPlayerLeftLobbyGameNetworkPacket();

  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new ClientPlayerLeftLobbyGameNetworkPacket(datastream); }

  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;
  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("ClientPlayerLeftLobbyGame"); }
  virtual void                  Execute()         DF_OVERRIDE;

private:

  uint32                        m_GameId;
  LobbyGamePlayer*              m_LobbyPlayer;

};

#endif /// PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_CLIENTPLAYERLEFTLOBBYGAMENETWORKPACKET#pragma once
