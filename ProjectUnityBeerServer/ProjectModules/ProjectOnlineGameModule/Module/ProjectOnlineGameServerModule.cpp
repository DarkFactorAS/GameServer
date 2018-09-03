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

#include "EvilEngine/CoreDatabase/CoreDatabase.h"

#include "ProjectModules/ProjectOnlineGameModule/Data/OnlineGamePacketData.h"
#include "ProjectModules/ProjectOnlineGameModule/NetworkPackets/ServerNetworkPackets/ServerCreateOnlineGameNetworkPacket.h"
#include "ProjectModules/ProjectOnlineGameModule/NetworkPackets/ServerNetworkPackets/ServerRequestGameDataNetworkPacket.h"
#include "ProjectModules/ProjectOnlineGameModule/NetworkPackets/ServerNetworkPackets/ServerPlayerLeaveOnlineGameNetworkPacket.h"
#include "ProjectModules/ProjectOnlineGameModule/NetworkPackets/ServerNetworkPackets/ServerPlayerStartRoundNetworkPacket.h"

#include "ProjectModules/ProjectLobbyGameModule/Module/ProjectLobbyGameServerModule.h"
#include "ProjectModules/ProjectWorldBuilderModule/WorldBuilderModule/ProjectWorldBuilderServerModule.h"

#include "ProjectLobbyGameModule/Data/LobbyGameData.hpp"
#include "ProjectLobbyGameModule/Data/LobbyGamePlayer.hpp"

ProjectOnlineGameServerModule::ProjectOnlineGameServerModule() :
  CoreGameEngineModule(PROJECT_MODULETYPE_ONLINEGAME)
{
  m_CurrentGameId = 0;

  RegisterPacketType(OnlineGamePacketData::PacketData_ServerCreateOnlineGame, ServerCreateOnlineGameNetworkPacket::Create);
  RegisterPacketType(OnlineGamePacketData::PacketData_ServerRequestGameData, ServerRequestGameDataNetworkPacket::Create);
  RegisterPacketType(OnlineGamePacketData::PacketData_ServerLeaveOnlineGame, ServerPlayerLeaveOnlineGameNetworkPacket::Create);
  RegisterPacketType(OnlineGamePacketData::PacketData_ServerPlayerStartRound, ServerPlayerStartRoundNetworkPacket::Create);

  
}

ProjectOnlineGameServerModule* ProjectOnlineGameServerModule::GetModule(CoreGameEngine* gameEngine)
{
  if (gameEngine != NULL)
  {
    return safe_cast<ProjectOnlineGameServerModule*> (gameEngine->GetEngineModule(ProjectOnlineGameServerModule::PROJECT_MODULETYPE_ONLINEGAME));
  }
  return NULL;
}

ProjectLobbyGameServerModule* ProjectOnlineGameServerModule::GetServerLobbyGameModule()
{
  return ProjectLobbyGameServerModule::GetModule(m_CoreEngine);
}

uint32 ProjectOnlineGameServerModule::GetNexGameId()
{
#if DATABASE
  if (m_CurrentGameId == 0)
  {
    CoreDatabase* database = CoreDatabase::GetInstance();
    if (database != NULL)
    {
      const SQLResultSet& result = CoreDatabase::GetInstance()->ExecuteSelect("select COALESCE(max(gameid),0) as m from game");
      while (result.Next())
      {
        m_CurrentGameId = result.GetUInt32();
      }
    }
  }
#endif

  return ++m_CurrentGameId;
}

OnlineGameData* ProjectOnlineGameServerModule::CreateOnlineGame(uint32 accountId, uint32 lobbyGameId)
{
  ProjectLobbyGameServerModule* lobbyModule = GetServerLobbyGameModule();
  ProjectWorldBuilderServerModule* worldbuilderModule = ProjectWorldBuilderServerModule::GetModule( m_CoreEngine );
  if (lobbyModule != NULL && worldbuilderModule != NULL )
  {
    LobbyGameData* lobbyGame = lobbyModule->GetLobbyGameWithAccountId(accountId);
    if (lobbyGame != NULL && lobbyGame->GetGameId() == lobbyGameId)
    {
      Playfield* playfield = worldbuilderModule->GetPlayfield(lobbyGame->GetPlayfieldId());
      if (playfield == NULL)
      {
        /// Notify client about missing playfield
        return NULL;
      }

      OnlineGameData* onlineGame = new OnlineGameData();
      onlineGame->SetPlayfield( playfield );

      std::vector<uint32> spawnpointList;

      // Add players
      const std::vector<LobbyGamePlayer*> playerList = lobbyGame->GetPlayerList();
      uint32 numPlayers = (uint32) playerList.size();
      for (std::vector<LobbyGamePlayer*>::const_iterator itLobbyPlayer = playerList.begin(); itLobbyPlayer != playerList.end(); ++itLobbyPlayer)
      {
        uint32 spawnpointId = playfield->GetRandomSpawnpoint(numPlayers, spawnpointList);
        spawnpointList.push_back(spawnpointId);

        const LobbyGamePlayer* lobbyPlayer = *itLobbyPlayer;
        onlineGame->AddPlayer(lobbyPlayer->GetAccountId(), lobbyPlayer->GetPlayerName(), lobbyPlayer->GetRobotId(), spawnpointId);
      }

      if (SaveGameInDatabase(onlineGame))
      {
        AddOnlineGameToCache(onlineGame);
        return onlineGame;
      }

      delete onlineGame;
    }
  }

  return NULL;
}

bool ProjectOnlineGameServerModule::SaveGameInDatabase(OnlineGameData* onlineGame)
{
  if (onlineGame != NULL)
  {
    if (onlineGame->GetGameId() != 0)
    {
      // Update some stuff
      int numRows = CoreDatabase::GetInstance()->ExecuteUpdate("update game set GameStatus = %d, WinningPlayerId = %d where GameId = %d ",
        0,
        0,
        onlineGame->GetGameId() );

      // Save players
      const std::vector<OnlineGamePlayer*> playerList = onlineGame->GetPlayerList();
      for (std::vector<OnlineGamePlayer*>::const_iterator itOnlinePlayer = playerList.begin(); itOnlinePlayer != playerList.end(); ++itOnlinePlayer)
      {
        OnlineGamePlayer* onlinePlayer = *itOnlinePlayer;
        if (onlinePlayer != NULL)
        {
          numRows += CoreDatabase::GetInstance()->ExecuteInsert("update game_player set PlayerPosX=%d, PlayerPosY=%d, SpawnPointId=%d where GameId=%d and PlayerId=%d ",
            onlinePlayer->GetPositionX(),
            onlinePlayer->GetPositionY(),
            onlinePlayer->GetSpawnPointId(),
            onlineGame->GetGameId(),
            onlinePlayer->GetAccountId()
            );
        }
      }

      return ( numRows > 1 );
    }
    else
    {
      uint32 gameId = GetNexGameId();
      onlineGame->SetGameId(gameId);

      bool didInsert = CoreDatabase::GetInstance()->ExecuteInsert("insert into game (GameId,PlayfieldId,GameStatus,WinningPlayerId) values(%d,%d,0,0) ",
        gameId,
        onlineGame->GetPlayfieldId() );

      // Save players
      const std::vector<OnlineGamePlayer*> playerList = onlineGame->GetPlayerList();
      for (std::vector<OnlineGamePlayer*>::const_iterator itOnlinePlayer = playerList.begin(); itOnlinePlayer != playerList.end(); ++itOnlinePlayer)
      {
        OnlineGamePlayer* onlinePlayer = *itOnlinePlayer;
        if (didInsert && onlinePlayer != NULL )
        {
          didInsert = CoreDatabase::GetInstance()->ExecuteInsert("insert into game_player (GameId,PlayerId,PlayerPosX, PlayerPosY, SpawnPointId) values(%d,%d,%d,%d, %d) ",
            gameId,
            onlinePlayer->GetAccountId(),
            onlinePlayer->GetPositionX(),
            onlinePlayer->GetPositionY(),
            onlinePlayer->GetSpawnPointId()
            );
        }
      }

      return didInsert;
    }
  }
  return false;
}

void ProjectOnlineGameServerModule::AddOnlineGameToCache(OnlineGameData* game)
{
  // TODO > Check for duplicates
  m_OnlineGameMap[ game->GetGameId() ] = game;
}

//
// TODO > Add load and save
//
std::vector< uint32 > ProjectOnlineGameServerModule::GetOnlineGamesWithAccount(uint32 accountId)
{
  std::vector< uint32 > gameList;
  for (uint32 index = 0; index < m_OnlineGameMap.size(); index++)
  {
    OnlineGameData* onlineGame = m_OnlineGameMap[index];
    if (onlineGame != NULL && onlineGame->HasPlayer(accountId))
    {
      gameList.push_back( onlineGame->GetGameId() );
    }
  }
  return gameList;
}

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
  std::map< uint32, OnlineGameData* >::iterator itOnlineGame = m_OnlineGameMap.find(gameId);
  if (itOnlineGame != m_OnlineGameMap.end())
  {
    return itOnlineGame->second;
  }
  return NULL;
}

bool ProjectOnlineGameServerModule::LeaveOnlineGame(uint32 gameId, uint32 accountId)
{
  OnlineGameData* onlineGame = GetOnlineGame(gameId);
  if (onlineGame != NULL)
  {
    return onlineGame->Removelayer(accountId);
  }
  return false;
}
