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
#include "ProjectModules/ProjectOnlineGameModule/NetworkPackets/ServerNetworkPackets/ServerRequestGameDataNetworkPacket.h"

ProjectOnlineGameServerModule::ProjectOnlineGameServerModule() :
  CoreGameEngineModule(PROJECT_MODULETYPE_ONLINEGAME)
{
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
  //Account* account = GetAccount();

  //ProjectGameManagementServerModule* module = GetModule();
  //ProjectOnlineGameServerModule* onlineGameModule = GetOnlineGameModule();

  //if (module != NULL && account != NULL && onlineGameModule != NULL)
  //{
  //  LobbyGameData* lobbyGame = module->GetLobbyGameWithAccountId(account->GetAccountId());
  //  if (lobbyGame != NULL && lobbyGame->GetGameId() == m_LobbyGameId)
  //  {
  //    OnlineGameData* onlineGame = onlineGameModule->CreateOnlineGame(lobbyGame->GetPlayfieldId());
  //    if (onlineGame != NULL)
  //    {
  //      // Add players
  //      const std::vector<LobbyGamePlayer*> playerList = lobbyGame->GetPlayerList();
  //      for (std::vector<LobbyGamePlayer*>::const_iterator itLobbyPlayer = playerList.begin(); itLobbyPlayer != playerList.end(); ++itLobbyPlayer)
  //      {
  //        const LobbyGamePlayer* lobbyPlayer = *itLobbyPlayer;
  //        onlineGame->AddPlayer(lobbyPlayer->GetAccountId(), lobbyPlayer->GetPlayerName(), lobbyPlayer->GetRobotId());
  //      }

  //      // Notify clients
  //      // Broadcast to all game players
  //      SendPacketToClient(new ClientCreatedLobbyGameNetworkPacket(newGame));

  //      return;
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
