#ifndef PROJECT_PROJECTGAMEMANAGEMENTMODULE_DATA_LOBBYGAMEDATA
#define PROJECT_PROJECTGAMEMANAGEMENTMODULE_DATA_LOBBYGAMEDATA 1

#include "LobbyGamePlayer.hpp"

#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/Data/Playfield.h"

class LobbyGameData
{
public:

  LobbyGameData() :
    m_GameId(0),
    m_Playfield(NULL)
  {
  }

  LobbyGameData(uint32 gameId) :
    m_GameId(gameId),
    m_Playfield(NULL)
  {
  }

  uint32 GetGameId() const{ return m_GameId; }
  void   SetGameId(uint32 gameId){ m_GameId = gameId; }

  Playfield* GetPlayfield() const { return m_Playfield; }
  void   SetPlayfield(Playfield* playfield) { m_Playfield = playfield; }

  uint32 GetPlayfieldId() const 
  {
    return ( m_Playfield != NULL ) ? m_Playfield->GetPlayfieldId() : 0;
  }

  bool HasPlayer(uint32 accountId)
  {
    for (std::vector<LobbyGamePlayer*>::iterator itPlayer = m_PlayerList.begin(); itPlayer != m_PlayerList.end(); itPlayer++)
    {
      LobbyGamePlayer* lobbyPlayer = *itPlayer;
      if (lobbyPlayer->GetAccountId() == accountId)
      {
        return true;
      }
    }
    return false;
  }

  LobbyGamePlayer* GetPlayer(uint32 accountId)
  {
    for (std::vector<LobbyGamePlayer*>::iterator itPlayer = m_PlayerList.begin(); itPlayer != m_PlayerList.end(); itPlayer++)
    {
      LobbyGamePlayer* lobbyPlayer = *itPlayer;
      if (lobbyPlayer->GetAccountId() == accountId)
      {
        return lobbyPlayer;
      }
    }
    return NULL;
  }

  void AddPlayer(uint32 acccountId, const String& playerName, uint32 robotId)
  {
    AddPlayer( new LobbyGamePlayer(acccountId, playerName, robotId) );
  }

  void AddPlayer(LobbyGamePlayer* player)
  {
    m_PlayerList.push_back(player);
  }

  bool RemovePlayer(LobbyGamePlayer* removeLobbyPlayer)
  {
    for (std::vector<LobbyGamePlayer*>::iterator itPlayer = m_PlayerList.begin(); itPlayer != m_PlayerList.end(); itPlayer++)
    {
      LobbyGamePlayer* lobbyPlayer = *itPlayer;
      if (lobbyPlayer->GetAccountId() == removeLobbyPlayer->GetAccountId())
      {
        m_PlayerList.erase(itPlayer);
        return true;
      }
    }
    return false;
  }

  bool RemovePlayer(uint32 accountId)
  {
    for (std::vector<LobbyGamePlayer*>::iterator itPlayer = m_PlayerList.begin(); itPlayer != m_PlayerList.end(); itPlayer++)
    {
      LobbyGamePlayer* lobbyPlayer = *itPlayer;
      if (lobbyPlayer->GetAccountId() == accountId)
      {
        m_PlayerList.erase(itPlayer);
        delete lobbyPlayer;
        return true;
      }
    }
    return false;
  }

  const std::vector<LobbyGamePlayer*> GetPlayerList() const { return m_PlayerList; }

private:

  uint32 m_GameId;  

  Playfield* m_Playfield;
  std::vector< LobbyGamePlayer* > m_PlayerList;
};

#endif /// PROJECT_PROJECTGAMEMANAGEMENTMODULE_DATA_LOBBYGAMEDATA
