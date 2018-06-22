/*************************************************************************************
* Project     : EvilEngine Multiplatform 2014-2018
* File        : ProjectOnlineGameServerModule
* Description : Abstract interface class for CoreGameEngine modules. This interface
*               has to be implemented to be able to run as a sub component in the
*               CoreGameEngine.
*
* Author      : Thor Richard Hansen
*************************************************************************************/

#include "PreCompile.h"
#include "ProjectOnlineGameServerModule.h"

#include "ProjectModules/ProjectOnlineGameModule/Data/OnlineGamePacketData.h"
#include "ProjectModules/ProjectOnlineGameModule/NetworkPackets/ServerNetworkPackets/ServerCreateOnlineGameNetworkPacket.h"
#include "ProjectModules/ProjectOnlineGameModule/NetworkPackets/ServerNetworkPackets/ServerRequestGameDataNetworkPacket.h"

#include "ProjectModules/ProjectLobbyGameModule/Logic/GameManagementModule/ProjectGameManagementServerModule.h"

#include "ProjectLobbyGameModule/Data/LobbyGameData.hpp"
#include "ProjectLobbyGameModule/Data/LobbyGamePlayer.hpp"

ProjectOnlineGameServerModule::ProjectOnlineGameServerModule() :
  CoreGameEngineModule(PROJECT_MODULETYPE_ONLINEGAME)
{
  RegisterPacketType(OnlineGamePacketData::PacketData_ServerCreateOnlineGame, ServerCreateOnlineGameNetworkPacket::Create);
  RegisterPacketType(OnlineGamePacketData::PacketData_ServerRequestGameData, ServerRequestGameDataNetworkPacket::Create);
}

ProjectOnlineGameServerModule* ProjectOnlineGameServerModule::GetModule(CoreGameEngine* gameEngine)
{
  if (gameEngine != NULL)
  {
    return safe_cast<ProjectOnlineGameServerModule*> (gameEngine->GetEngineModule(ProjectOnlineGameServerModule::PROJECT_MODULETYPE_ONLINEGAME));
  }
  return NULL;
}

OnlineGameData* ProjectOnlineGameServerModule::CreateOnlineGame(uint32 /*accountId*/, uint32 /*lobbyGameId*/)
{
  //ProjectGameManagementServerModule* lobbyModule = safe_cast<ProjectGameManagementServerModule*> (gameEngine->GetEngineModule(ProjectGameManagementServerModule::PROJECT_MODULETYPE_GAMEMANAGEMENT));
  //if (lobbyModule != NULL)
  //{
  //  LobbyGameData* lobbyGame = lobbyModule->GetLobbyGameWithAccountId(accountId);
  //  if (lobbyGame != NULL && lobbyGame->GetGameId() == m_LobbyGameId)
  //  {
  //    OnlineGameData* onlineGame = new OnlineGameData();
  //    if (onlineGame != NULL)
  //    {
  //      onlineGame->SetPlayfield( lobbyGame->GetPlayfieldId() );

  //      // Add players
  //      const std::vector<LobbyGamePlayer*> playerList = lobbyGame->GetPlayerList();
  //      for (std::vector<LobbyGamePlayer*>::const_iterator itLobbyPlayer = playerList.begin(); itLobbyPlayer != playerList.end(); ++itLobbyPlayer)
  //      {
  //        const LobbyGamePlayer* lobbyPlayer = *itLobbyPlayer;
  //        onlineGame->AddPlayer(lobbyPlayer->GetAccountId(), lobbyPlayer->GetPlayerName(), lobbyPlayer->GetRobotId());
  //      }

  //      return onlineGame;
  //    }
  //  }
  //}

  return NULL;
}

void ProjectOnlineGameServerModule::AddOnlineGame(OnlineGameData* game)
{
  // TODO > Check for duplicates
  m_OnlineGameMap[ game->GetGameId() ] = game;
}

//
// TODO > Add load and save
//

OnlineGameData* ProjectOnlineGameServerModule::GetOnlineGame(uint32 accountId, uint32 gameId)
{
  OnlineGameData* onlineGame = GetOnlineGame(gameId);
  if (onlineGame != NULL && onlineGame->HasPlayer(accountId))
  {
    return onlineGame;
  }
  return NULL;
}

OnlineGameData* ProjectOnlineGameServerModule::GetOnlineGame(uint32 gameId)
{
  for (uint32 index = 0; index < m_OnlineGameMap.size(); index++)
  {
    OnlineGameData* lobbyGame = m_OnlineGameMap[index];
    if (lobbyGame != NULL && lobbyGame->GetGameId() == gameId)
    {
      return lobbyGame;
    }
  }
  return NULL;
}
