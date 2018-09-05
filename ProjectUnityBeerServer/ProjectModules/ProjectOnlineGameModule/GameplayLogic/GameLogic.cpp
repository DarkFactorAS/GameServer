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
#include "GameLogic.h"
#include "EvilEngine/CoreDatabase/CoreDatabase.h"

#include "ProjectModules/ProjectOnlineGameModule/Module/ProjectOnlineGameServerModule.h"

uint32 GameLogic::s_CurrentGameId = 0;

GameLogic::GameLogic()
{
#if DATABASE
  m_Database = CoreDatabase::GetInstance();
#endif
}

uint32 GameLogic::GetNexGameId()
{
#if DATABASE
  if (s_CurrentGameId == 0 && m_Database != NULL )
  {
    const SQLResultSet& result = m_Database->ExecuteSelect("select COALESCE(max(gameid),0) as m from game");
    while (result.Next())
    {
      s_CurrentGameId = result.GetUInt32();
    }
  }
#endif

  return ++s_CurrentGameId;
}

bool GameLogic::SaveGameInDatabase(OnlineGameData* onlineGame)
{
  if (onlineGame != NULL)
  {
    if (onlineGame->GetGameId() != 0)
    {
      int numRows = m_Database->ExecuteUpdate("update game set GameStatus = %d, WinningPlayerId = %d where GameId = %d ",
        0,
        0,
        onlineGame->GetGameId());

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

      return (numRows > 1);
    }
    else
    {
      uint32 gameId = GetNexGameId();
      onlineGame->SetGameId(gameId);

      bool didInsert = CoreDatabase::GetInstance()->ExecuteInsert("insert into game (GameId,PlayfieldId,GameStatus,WinningPlayerId) values(%d,%d,0,0) ",
        gameId,
        onlineGame->GetPlayfieldId());

      // Save players
      const std::vector<OnlineGamePlayer*> playerList = onlineGame->GetPlayerList();
      for (std::vector<OnlineGamePlayer*>::const_iterator itOnlinePlayer = playerList.begin(); itOnlinePlayer != playerList.end(); ++itOnlinePlayer)
      {
        OnlineGamePlayer* onlinePlayer = *itOnlinePlayer;
        if (didInsert && onlinePlayer != NULL)
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
