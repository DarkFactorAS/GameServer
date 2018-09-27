#ifndef PROJECT_PROJECTONLINEGAMEMODULE_GAMEPLAYLOGIC_GAMEPLAYLOGIC
#define PROJECT_PROJECTONLINEGAMEMODULE_GAMEPLAYLOGIC_GAMEPLAYLOGIC 1

#include "EvilEngine/CoreEngine/CoreEngine/EngineModule.h"
#include "EvilEngine/CoreLib/BasicTypes/Definitions/VolatileArgs.h"

#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngineModule.h"

#include "ProjectModules/ProjectOnlineGameModule/Data/OnlineGameData.hpp"
#include "ProjectModules/ProjectOnlineGameModule/Data/ActionCard.hpp"

class ProjectOnlineGameServerModule;

class OnlineGameData;
class LobbyGameData;
class GameLogic;

class GameplayLogic
{
public:

  GameplayLogic(ProjectOnlineGameServerModule* parent);
  ~GameplayLogic();
  uint32  GetNexGameId();

  OnlineGameData*                       CreateOnlineGame(uint32 accountId, LobbyGameData* lobbyGame, Playfield* playfield);
  bool                                  LeaveOnlineGame(uint32 gameId, uint32 accountId);

  OnlineGameData*                       GetOnlineGame(uint32 gameId);
  OnlineGameData*                       GetOnlineGameWithAccount(uint32 gameId, uint32 accountId);
  std::vector< uint32 >                 GetOnlineGameListFromAccount(uint32 accountId);

  void                                  SetPlayerReady(uint32 gameId, uint32 accountId);
  void                                  SetStatusOnAllPlayers(uint32 gameId, OnlineGamePlayer::PlayerState playerStatus);
  bool                                  AreAllPlayersReady(uint32 gameID);

  std::vector<ActionCard*>              GetActionCards(OnlineGameData* onlineGame, OnlineGamePlayer* player);

private:

  void                                  AddOnlineGameToCache(OnlineGameData* game);

  ProjectOnlineGameServerModule*      m_Module;
  GameLogic*                          m_GameLogic;
  std::map< uint32, OnlineGameData* > m_OnlineGameMap;
};

#endif /// PROJECT_PROJECTONLINEGAMEMODULE_GAMEPLAYLOGIC_GAMEPLAYLOGIC
