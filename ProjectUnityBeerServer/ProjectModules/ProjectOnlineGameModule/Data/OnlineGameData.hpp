#ifndef PROJECT_ONLINEGAME_DATA_ONLINEGAMEDATA
#define PROJECT_ONLINEGAME_DATA_ONLINEGAMEDATA 1

#include "OnlineGamePlayer.hpp"

#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/Data/Playfield.h"

class OnlineGameData
{
public:

  OnlineGameData() :
    m_GameId(0),
    m_Playfield(NULL)
  {
  }

  OnlineGameData(uint32 gameId) :
    m_GameId(gameId),
    m_Playfield(NULL)
  {
  }

  uint32 GetGameId() const { return m_GameId; }
  void   SetGameId(uint32 gameId) { m_GameId = gameId; }

  Playfield* GetPlayfield() const { return m_Playfield; }
  void   SetPlayfield(Playfield* playfield) { m_Playfield = playfield; }

  void AddPlayer(uint32 acccountId, const String& playerName, uint32 robotId)
  {
    AddPlayer(new OnlineGamePlayer(acccountId, playerName, robotId));
  }

  void AddPlayer(OnlineGamePlayer* player)
  {
    m_PlayerList.push_back(player);
  }

  bool HasPlayer(uint32 accountId)
  {
    for (std::vector<OnlineGamePlayer*>::iterator itPlayer = m_PlayerList.begin(); itPlayer != m_PlayerList.end(); itPlayer++)
    {
      OnlineGamePlayer* onlinePlayer = *itPlayer;
      if (onlinePlayer->GetAccountId() == accountId)
      {
        return true;
      }
    }
    return false;
  }

  const std::vector<OnlineGamePlayer*> GetPlayerList() const { return m_PlayerList; }

private:

  uint32 m_GameId;

  Playfield* m_Playfield;
  std::vector< OnlineGamePlayer* > m_PlayerList;
};

#endif /// PROJECT_ONLINEGAME_DATA_ONLINEGAMEDATA
