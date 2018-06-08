#ifndef PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_CLIENTCREATEDGAMENETWORKPACKET
#define PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_CLIENTCREATEDGAMENETWORKPACKET 1

#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/Packets/BaseNetworkPacket.h"

class Playfield;
class LobbyGameData;

class ClientLobbyGameListNetworkPacket : public BaseNetworkPacket
{
public:

  ClientLobbyGameListNetworkPacket(std::vector<LobbyGameData*> lobbyGame);
  ClientLobbyGameListNetworkPacket(const BinaryStream* datastream);

  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new ClientLobbyGameListNetworkPacket(datastream); }

  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;
  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("ClientLobbyGameList"); }
  virtual void                  Execute()         DF_OVERRIDE;

private:

  std::vector< LobbyGameData* > m_LobbyGameList;

};

#endif /// PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_CLIENTCREATEDGAMENETWORKPACKET#pragma once
