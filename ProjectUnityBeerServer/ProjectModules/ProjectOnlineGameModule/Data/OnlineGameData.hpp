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
  uint32 GetPlayfieldId() const
  {
    if (m_Playfield != NULL)
    {
      return m_Playfield->GetPlayfieldId();
    }
    return 0;
  }

  void AddPlayer(uint32 acccountId, const String& playerName, uint32 robotId, uint32 spawnpointId)
  {
    AddPlayer(new OnlineGamePlayer(acccountId, playerName, robotId, spawnpointId));
  }

  void AddPlayer(OnlineGamePlayer* player)
  {
    if (m_OwnerAccountId == 0)
    {
      m_OwnerAccountId = player->GetAccountId();
    }
    m_PlayerList.push_back(player);
  }

  void AssignOwnerId(uint32 excludeAccountId)
  {
    m_OwnerAccountId = 0;
    for (std::vector<OnlineGamePlayer*>::iterator itPlayer = m_PlayerList.begin(); itPlayer != m_PlayerList.end(); itPlayer++)
    {
      OnlineGamePlayer* onlinePlayer = *itPlayer;
      if (onlinePlayer->GetAccountId() != excludeAccountId)
      {
        m_OwnerAccountId = onlinePlayer->GetAccountId();
        return;
      }
    }
  }

  bool Removelayer(uint32 accountId)
  {
    for (std::vector<OnlineGamePlayer*>::iterator itPlayer = m_PlayerList.begin(); itPlayer != m_PlayerList.end(); itPlayer++)
    {
      OnlineGamePlayer* onlinePlayer = *itPlayer;
      if (onlinePlayer->GetAccountId() == accountId)
      {
        m_PlayerList.erase(itPlayer);

        // Assign a new owner if needed
        if (accountId == m_OwnerAccountId)
        {
          AssignOwnerId( accountId );
        }

        return true;
      }
    }
    return false;
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

  uint32 GetOwnerAccountId() const
  {
    return m_OwnerAccountId;
  }

  const std::vector<OnlineGamePlayer*> GetPlayerList() const { return m_PlayerList; }

private:

  uint32 m_GameId;
  uint32 m_OwnerAccountId;

  Playfield* m_Playfield;
  std::vector< OnlineGamePlayer* > m_PlayerList;
};

#endif /// PROJECT_ONLINEGAME_DATA_ONLINEGAMEDATA
