
#include "Precompile.h"
#include "BaseGameManagementNetworkPacket.h"

#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"

#include "ProjectLobbyGameModule/Data/LobbyGameData.hpp"
#include "ProjectLobbyGameModule/Data/LobbyGamePlayer.hpp"
#include "ProjectLobbyGameModule/NetworkPackets/ClientNetworkPackets/ClientGameErrorNetworkPacket.h"

BaseGameManagementNetworkPacket::BaseGameManagementNetworkPacket(GameEnginePacketData::PacketData packetType, const BinaryStream* datastream) :
  BaseAuthentcatedNetworkPacket((uint32)packetType, datastream)
{
}

BaseGameManagementNetworkPacket::BaseGameManagementNetworkPacket(GameEnginePacketData::PacketData packetType) :
  BaseAuthentcatedNetworkPacket((uint32)packetType)
{
}

ProjectLobbyGameServerModule* BaseGameManagementNetworkPacket::GetModule()
{
  return safe_cast<ProjectLobbyGameServerModule*> ( m_GameEngineModule );
}

void BaseGameManagementNetworkPacket::SendGameErrorToClient(GameEnginePacketData::PacketError errorId)
{
  SendGameErrorToClient((uint32)errorId);
}

void BaseGameManagementNetworkPacket::SendGameErrorToClient(uint32 errorCodeId)
{
  CoreGameEngine* gameEngine = GetGameEngine();
  if (gameEngine != NULL)
  {
    LogInfoFMT("BaseGameManagementNetworkPacket", "SendGameErrorToClient::Received error => %d/%d", m_PacketType, errorCodeId);

    if (errorCodeId == GameEnginePacketData::ErrorCode_NoError)
    {
      errorCodeId = GameEnginePacketData::ErrorCode_CodeError;
    }

    String errorMessage = String::zero;
    ProjectLobbyGameServerModule* module = GetModule();
    if (module != NULL)
    {
      errorMessage = module->GetGameError(errorCodeId);
    }

    gameEngine->SendPacketToEndpoint(m_ConnectionId, new ClientGameErrorNetworkPacket(m_PacketType, errorCodeId, errorMessage));
  }
}

void BaseGameManagementNetworkPacket::SendPacketToLobbyGamePlayers(LobbyGameData* lobbyGame, BaseNetworkPacket* packet)
{
  ProjectLobbyGameServerModule* lobbyGameModule = GetModule();
  if (lobbyGameModule != NULL)
  {
    lobbyGameModule->SendPacketToLobbyGamePlayers(lobbyGame, packet);
  }
}
