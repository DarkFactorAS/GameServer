#ifndef PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_CLIENTCREATEDGAMENETWORKPACKET
#define PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_CLIENTCREATEDGAMENETWORKPACKET 1

#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/Packets/BaseNetworkPacket.h"

class Playfield;
class LobbyGameData;

class ClientActiveOnlineGameListNetworkPacket : public BaseNetworkPacket
{
public:

  ClientActiveOnlineGameListNetworkPacket(std::vector<uint32> onlineGameList);
  ClientActiveOnlineGameListNetworkPacket(const BinaryStream* datastream);

  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new ClientActiveOnlineGameListNetworkPacket(datastream); }

  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;
  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("ClientActiveOnlineGameList"); }
  virtual void                  Execute()         DF_OVERRIDE;

private:

  std::vector< uint32 > m_OnlineGameList;

};

#endif /// PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_CLIENTCREATEDGAMENETWORKPACKET#pragma once
