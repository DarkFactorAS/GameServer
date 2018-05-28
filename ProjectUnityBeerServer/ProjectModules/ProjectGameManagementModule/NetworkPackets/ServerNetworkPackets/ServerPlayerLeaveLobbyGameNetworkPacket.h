#ifndef PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_SERVERLEAVELOBBYGAMENETWORKPACKET
#define PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_SERVERLEAVELOBBYGAMENETWORKPACKET 1

#include "ProjectUnityBeerServer/ProjectModules/ProjectGameManagementModule/NetworkPackets/BaseGameManagementNetworkPacket.h"

#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/Data/Playfield.h"
#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/WorldBuilderModule/ProjectWorldBuilderServerModule.h"

class ServerPlayerLeaveLobbyGameNetworkPacket : public BaseGameManagementNetworkPacket
{
public:

  ServerPlayerLeaveLobbyGameNetworkPacket(uint32 gameId);
  ServerPlayerLeaveLobbyGameNetworkPacket(const BinaryStream* datastream);

  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new ServerPlayerLeaveLobbyGameNetworkPacket(datastream); }

  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("ServerPlayerLeaveLobbyGame"); }
  virtual void                  Execute()         DF_OVERRIDE;
  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;

private:

  uint32                        m_GameId;

};

#endif /// PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_SERVERLEAVELOBBYGAMENETWORKPACKETServerPlayerLeaveLobbyGameNetworkPacket#pragma once
