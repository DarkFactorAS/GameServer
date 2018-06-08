#ifndef PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_CLIENTCREATEDGAMENETWORKPACKET
#define PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_CLIENTCREATEDGAMENETWORKPACKET 1

#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/Packets/BaseNetworkPacket.h"

class Playfield;
class LobbyGameData;

class ClientCreatedLobbyGameNetworkPacket : public BaseNetworkPacket
{
public:

  ClientCreatedLobbyGameNetworkPacket(LobbyGameData* lobbyGame);
  ClientCreatedLobbyGameNetworkPacket(const BinaryStream* datastream);

  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new ClientCreatedLobbyGameNetworkPacket(datastream); }

  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;
  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("ClientCreatedLobbyGame"); }
  virtual void                  Execute()         DF_OVERRIDE;

private:

  LobbyGameData*                m_LobbyGame;

};

#endif /// PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_CLIENTCREATEDGAMENETWORKPACKET#pragma once
