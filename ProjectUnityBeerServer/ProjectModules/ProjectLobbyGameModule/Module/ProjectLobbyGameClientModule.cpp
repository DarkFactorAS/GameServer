
#include "PreCompile.h"
#include "ProjectLobbyGameClientModule.h"

#include "EvilEngine/CoreLib/Utils/Base64Util.h"
#include "EvilEngine/CoreDatabase/CoreDatabase.h"

#include "ProjectLobbyGameModule/Data/GameManagementPacketData.h"
#include "ProjectLobbyGameModule/NetworkPackets/ClientNetworkPackets/ClientGameErrorNetworkPacket.h"
#include "ProjectLobbyGameModule/NetworkPackets/ClientNetworkPackets/ClientCreatedLobbyGameNetworkPacket.h"
#include "ProjectLobbyGameModule/NetworkPackets/ClientNetworkPackets/ClientCreatedQuickGameNetworkPacket.h"

ProjectLobbyGameClientModule::ProjectLobbyGameClientModule() :
  CoreGameEngineModule(PROJECT_MODULETYPE_CLIENTGAMEMANAGEMENT)
{
  // Client network packets
  RegisterPacketType(GameEnginePacketData::PacketData_ClientGameError, ClientGameErrorNetworkPacket::Create);
  RegisterPacketType(GameEnginePacketData::PacketData_ClientCreatedLobbyGame, ClientCreatedLobbyGameNetworkPacket::Create);
  RegisterPacketType(GameEnginePacketData::PacketData_ClientCreatedQuickGame, ClientCreatedQuickGameNetworkPacket::Create);
}

void ProjectLobbyGameClientModule::CreatedGame(LobbyGameData* lobbyGame)
{
  SignalCreatedGame(lobbyGame);
}

void ProjectLobbyGameClientModule::GameNetworkError(uint32 packetType, uint32 errorCode, const String& errorMessage)
{
  SignalGameNetworkError(packetType, errorCode, errorMessage);
}
