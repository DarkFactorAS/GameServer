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
#include "GameplayLogic.h"
#include "GameLogic.h"

#include "ProjectModules/ProjectOnlineGameModule/Module/ProjectOnlineGameServerModule.h"
#include "ProjectModules/ProjectOnlineGameModule/Data/OnlineGameData.hpp"

// Other modules
#include "ProjectModules/ProjectLobbyGameModule/Data/LobbyGameData.hpp"
#include "ProjectModules/ProjectLobbyGameModule/Data/LobbyGamePlayer.hpp"

GameplayLogic::GameplayLogic(ProjectOnlineGameServerModule* parentModule) :
  m_Module(parentModule)
{
  m_GameLogic = new GameLogic();
}

GameplayLogic::~GameplayLogic()
{
  delete m_GameLogic;
}

uint32 GameplayLogic::GetNexGameId()
{
  return m_GameLogic->GetNexGameId();
}

void GameplayLogic::AddOnlineGameToCache(OnlineGameData* game)
{
  m_OnlineGameMap[game->GetGameId()] = game;
}

//
// TODO > Add load and save
//
std::vector< uint32 > GameplayLogic::GetOnlineGameListFromAccount(uint32 accountId)
{
  std::vector< uint32 > gameList;
  for (uint32 index = 0; index < m_OnlineGameMap.size(); index++)
  {
    OnlineGameData* onlineGame = m_OnlineGameMap[index];
    if (onlineGame != NULL && onlineGame->HasPlayer(accountId))
    {
      gameList.push_back(onlineGame->GetGameId());
    }
  }
  return gameList;
}

OnlineGameData* GameplayLogic::GetOnlineGame(uint32 gameId)
{
  std::map< uint32, OnlineGameData* >::iterator itOnlineGame = m_OnlineGameMap.find(gameId);
  if (itOnlineGame != m_OnlineGameMap.end())
  {
    return itOnlineGame->second;
  }
  return NULL;
}

OnlineGameData* GameplayLogic::GetOnlineGameWithAccount(uint32 gameId, uint32 accountId)
{
  OnlineGameData* onlineGame = GetOnlineGame(gameId);
  if (onlineGame != NULL && onlineGame->HasPlayer(accountId))
  {
    return onlineGame;
  }
  return NULL;
}

OnlineGameData* GameplayLogic::CreateOnlineGame(uint32 accountId, LobbyGameData* lobbyGame, Playfield* playfield)
{
  OnlineGameData* onlineGame = new OnlineGameData();
  onlineGame->SetPlayfield(playfield);
  onlineGame->SetOwnerAccountId(accountId);

  std::vector<uint32> spawnpointList;

  // Add players
  const std::vector<LobbyGamePlayer*> playerList = lobbyGame->GetPlayerList();
  uint32 numPlayers = (uint32)playerList.size();
  for (std::vector<LobbyGamePlayer*>::const_iterator itLobbyPlayer = playerList.begin(); itLobbyPlayer != playerList.end(); ++itLobbyPlayer)
  {
    uint32 spawnpointId = playfield->GetRandomSpawnpoint(numPlayers, spawnpointList);
    spawnpointList.push_back(spawnpointId);

    const LobbyGamePlayer* lobbyPlayer = *itLobbyPlayer;
    onlineGame->AddPlayer(lobbyPlayer->GetAccountId(), lobbyPlayer->GetPlayerName(), lobbyPlayer->GetRobotId(), spawnpointId);
  }

  if ( m_GameLogic->SaveGameInDatabase(onlineGame))
  {
    AddOnlineGameToCache(onlineGame);
    return onlineGame;
  }

  delete onlineGame;
  return NULL;
}

bool GameplayLogic::LeaveOnlineGame(uint32 gameId, uint32 accountId)
{
  OnlineGameData* onlineGame = GetOnlineGame(gameId);
  if (onlineGame != NULL)
  {
    return onlineGame->Removelayer(accountId);
  }
  return false;
}

std::vector<ActionCard*> GameplayLogic::GetActionCards(uint32 gameId, uint32 accountId)
{
  OnlineGameData* onlineGame = GetOnlineGame(gameId);
  if (onlineGame != NULL)
  {
    OnlineGamePlayer* player = onlineGame->GetPlayer(accountId);
    ActionCardDeck* actionCardDeck = onlineGame->GetActionCardDeck();
    if (actionCardDeck != NULL && player != NULL )
    {
      ActionCardInventory* inventory = player->GetInventory();
      if (inventory != NULL)
      {
        uint32 numCards = player->GetLives();
        std::vector< ActionCard* > freshCards = actionCardDeck->GetCards(numCards);
        inventory->AddItems(freshCards);
        return freshCards;
      }
    }
  }

  std::vector< ActionCard* > deck;
  return deck;
}
