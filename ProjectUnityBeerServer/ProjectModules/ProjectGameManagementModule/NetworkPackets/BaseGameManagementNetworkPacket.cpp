
#include "Precompile.h"
#include "BaseGameManagementNetworkPacket.h"

#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"
#include "EvilGameEngine/CoreGameLogin/LoginModule/CoreGameServerLoginModule.h"

#include "ProjectGameManagementModule/Data/LobbyGameData.hpp"
#include "ProjectGameManagementModule/Data/LobbyGamePlayer.hpp"
#include "ProjectGameManagementModule/NetworkPackets/ClientNetworkPackets/ClientGameErrorNetworkPacket.h"

BaseGameManagementNetworkPacket::BaseGameManagementNetworkPacket(GameEnginePacketData::PacketData packetType, const BinaryStream* datastream) :
  BaseAuthentcatedNetworkPacket((uint32)packetType, datastream)
{
}

BaseGameManagementNetworkPacket::BaseGameManagementNetworkPacket(GameEnginePacketData::PacketData packetType) :
  BaseAuthentcatedNetworkPacket((uint32)packetType)
{
}

ProjectGameManagementServerModule* BaseGameManagementNetworkPacket::GetModule()
{
  CoreGameEngine* gameEngine = GetGameEngine();
  if (gameEngine != NULL)
  {
    return safe_cast<ProjectGameManagementServerModule*> (gameEngine->GetEngineModule(ProjectGameManagementServerModule::PROJECT_MODULETYPE_GAMEMANAGEMENT));
  }
  return NULL;
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
    ProjectGameManagementServerModule* module = GetModule();
    if (module != NULL)
    {
      errorMessage = module->GetGameError(errorCodeId);
    }

    gameEngine->SendPacketToEndpoint(m_ConnectionId, new ClientGameErrorNetworkPacket(m_PacketType, errorCodeId, errorMessage));
  }
}

void BaseGameManagementNetworkPacket::SendPacketToLobbyGamePlayers(LobbyGameData* lobbyGame, BaseNetworkPacket* packet)
{
  CoreGameEngine* gameEngine = GetGameEngine();
  if (gameEngine != NULL)
  {
    CoreGameServerLoginModule* loginModule = CoreGameServerLoginModule::GetModule(gameEngine);
    if (loginModule != NULL)
    {
      const std::vector<LobbyGamePlayer*> playerList = lobbyGame->GetPlayerList();
      for (std::vector<LobbyGamePlayer*>::const_iterator itPlayer = playerList.begin(); itPlayer != playerList.end(); ++itPlayer)
      {
        LobbyGamePlayer* lobbyPlayer = *itPlayer;

        Account* account = loginModule->GetCachedAccount(lobbyPlayer->GetAccountId());
        if (account != NULL)
        {
          gameEngine->SendPacketToEndpoint(account->GetConnectionId(), packet);
        }
      }
      return;
    }
    gameEngine->SendPacketToEndpoint(m_ConnectionId, packet);
    return;
  }
  // Show error
}
