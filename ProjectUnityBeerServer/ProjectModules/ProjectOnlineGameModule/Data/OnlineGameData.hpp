#ifndef PROJECT_ONLINEGAME_DATA_ONLINEGAMEDATA
#define PROJECT_ONLINEGAME_DATA_ONLINEGAMEDATA 1

#include "OnlineGamePlayer.hpp"

#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/Data/Playfield.h"
#include "ProjectUnityBeerServer/ProjectModules/ProjectOnlineGameModule/GameplayLogic/ActionCardDeck.h"

class OnlineGameData
{
public:

  enum GameMode
  {
    GameStart,
    StartRound,
    DealCards,
    CompletePlacement,
    ExecuteCards,
    GameCompleted,
  };

  OnlineGameData() :
    m_GameId(0),
    m_OwnerAccountId(0),
    m_Playfield(NULL),
    m_ActionCardDeck(NULL)
  {
    m_ActionCardDeck = new ActionCardDeck();
  }

  OnlineGameData(uint32 gameId) :
    m_GameId(gameId),
    m_OwnerAccountId(0),
    m_Playfield(NULL)
  {
    m_ActionCardDeck = new ActionCardDeck();
  }

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

  OnlineGamePlayer* GetPlayer(uint32 accountId)
  {
    for (std::vector<OnlineGamePlayer*>::iterator itPlayer = m_PlayerList.begin(); itPlayer != m_PlayerList.end(); itPlayer++)
    {
      OnlineGamePlayer* onlinePlayer = *itPlayer;
      if (onlinePlayer->GetAccountId() == accountId)
      {
        return onlinePlayer;
      }
    }
    return NULL;
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

  uint32                                GetGameId() const { return m_GameId; }
  void                                  SetGameId(uint32 gameId) { m_GameId = gameId; }
  Playfield*                            GetPlayfield() const { return m_Playfield; }
  void                                  SetPlayfield(Playfield* playfield) { m_Playfield = playfield; }
  uint32                                GetOwnerAccountId() const  { return m_OwnerAccountId; }
  void                                  SetOwnerAccountId(uint32 accountId){ m_OwnerAccountId = accountId; }
  const std::vector<OnlineGamePlayer*>  GetPlayerList() const { return m_PlayerList; }
  ActionCardDeck*                       GetActionCardDeck() const { return m_ActionCardDeck; }

  void                                  SetGamneMode(GameMode gameMode){ m_GameMode = gameMode; }
  GameMode                              GetGameMode() const { return m_GameMode; }

private:

  uint32                                m_GameId;
  uint32                                m_OwnerAccountId;

  Playfield*                            m_Playfield;
  std::vector< OnlineGamePlayer* >      m_PlayerList;
  ActionCardDeck*                       m_ActionCardDeck;
  GameMode                              m_GameMode;
};

#endif /// PROJECT_ONLINEGAME_DATA_ONLINEGAMEDATA
