
#include "PreCompile.h"
#include "ProjectClientLobbyGameModule.h"

#include "EvilEngine/CoreLib/Utils/Base64Util.h"
#include "EvilEngine/CoreDatabase/CoreDatabase.h"

#include "ProjectLobbyGameModule/Data/GameManagementPacketData.h"
#include "ProjectLobbyGameModule/NetworkPackets/ClientNetworkPackets/ClientGameErrorNetworkPacket.h"
#include "ProjectLobbyGameModule/NetworkPackets/ClientNetworkPackets/ClientCreatedLobbyGameNetworkPacket.h"
#include "ProjectLobbyGameModule/NetworkPackets/ClientNetworkPackets/ClientCreatedQuickGameNetworkPacket.h"

ProjectGameManagementClientModule::ProjectGameManagementClientModule() :
  CoreGameEngineModule(PROJECT_MODULETYPE_CLIENTGAMEMANAGEMENT)
{
  // Client network packets
  RegisterPacketType(GameEnginePacketData::PacketData_ClientGameError, ClientGameErrorNetworkPacket::Create);
  RegisterPacketType(GameEnginePacketData::PacketData_ClientCreatedLobbyGame, ClientCreatedLobbyGameNetworkPacket::Create);
  RegisterPacketType(GameEnginePacketData::PacketData_ClientCreatedQuickGame, ClientCreatedQuickGameNetworkPacket::Create);
}

void ProjectGameManagementClientModule::CreatedGame(LobbyGameData* lobbyGame)
{
  SignalCreatedGame(lobbyGame);
}

void ProjectGameManagementClientModule::GameNetworkError(uint32 packetType, uint32 errorCode, const String& errorMessage)
{
  SignalGameNetworkError(packetType, errorCode, errorMessage);
}
