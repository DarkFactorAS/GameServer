#ifndef PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_SERVERJOINLOBBYGAMENETWORKPACKET
#define PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_SERVERJOINLOBBYGAMENETWORKPACKET 1

#include "ProjectUnityBeerServer/ProjectModules/ProjectGameManagementModule/NetworkPackets/BaseGameManagementNetworkPacket.h"

#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/Data/Playfield.h"
#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/WorldBuilderModule/ProjectWorldBuilderServerModule.h"

class ServerPlayerJoinLobbyGameNetworkPacket : public BaseGameManagementNetworkPacket
{
public:

  ServerPlayerJoinLobbyGameNetworkPacket(uint32 gameId, uint32 robotId);
  ServerPlayerJoinLobbyGameNetworkPacket(const BinaryStream* datastream);

  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new ServerPlayerJoinLobbyGameNetworkPacket(datastream); }

  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("ServerPlayerJoinLobbyGame"); }
  virtual void                  Execute()         DF_OVERRIDE;
  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;

private:

  uint32                        m_GameId;
  uint32                        m_RobotId;

};

#endif /// PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_SERVERJOINLOBBYGAMENETWORKPACKET#pragma once
