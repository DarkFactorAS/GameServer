#ifndef PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_SERVERRANDOMJOINLOBBYGAMENETWORKPACKET
#define PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_SERVERRANDOMJOINLOBBYGAMENETWORKPACKET 1

#include "ProjectUnityBeerServer/ProjectModules/ProjectLobbyGameModule/NetworkPackets/BaseGameManagementNetworkPacket.h"

#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/Data/Playfield.h"
#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/WorldBuilderModule/ProjectWorldBuilderServerModule.h"

class ServerRandomPlayerJoinLobbyGameNetworkPacket : public BaseGameManagementNetworkPacket
{
public:

  ServerRandomPlayerJoinLobbyGameNetworkPacket(uint32 gameId);
  ServerRandomPlayerJoinLobbyGameNetworkPacket(const BinaryStream* datastream);

  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new ServerRandomPlayerJoinLobbyGameNetworkPacket(datastream); }

  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("ServerRandomPlayerJoinLobbyGame"); }
  virtual void                  Execute()         DF_OVERRIDE;
  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;

private:

  uint32                        m_GameId;

};

#endif /// PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_SERVERRANDOMJOINLOBBYGAMENETWORKPACKET#pragma once
