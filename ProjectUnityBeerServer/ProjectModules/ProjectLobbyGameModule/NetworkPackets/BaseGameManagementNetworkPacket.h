#ifndef PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_BASEGAMEMANAGEMENTNETWORKPACKET
#define PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_BASEGAMEMANAGEMENTNETWORKPACKET 1

#include "EvilEngine/CoreLib/BasicTypes/BinaryStream/BinaryStream.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/Packets/BaseNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/Network/BaseAuthentcatedNetworkPacket.h"

#include "ProjectUnityBeerServer/ProjectModules/ProjectLobbyGameModule/Data/GameManagementPacketData.h"
#include "ProjectUnityBeerServer/ProjectModules/ProjectLobbyGameModule/Module/ProjectLobbyGameServerModule.h"

class BaseGameManagementNetworkPacket : public BaseAuthentcatedNetworkPacket
{
public:

  BaseGameManagementNetworkPacket(GameEnginePacketData::PacketData packetType, const BinaryStream* datastream);
  BaseGameManagementNetworkPacket(GameEnginePacketData::PacketData packetType);

  void                          SendGameErrorToClient(uint32 errorId);
  void                          SendGameErrorToClient(GameEnginePacketData::PacketError errorId);

  void                          SendPacketToLobbyGamePlayers(LobbyGameData* lobbyGame, BaseNetworkPacket* packet);

  ProjectLobbyGameServerModule* GetModule();
};

#endif /// PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_BASEGAMEMANAGEMENTNETWORKPACKET#pragma once
