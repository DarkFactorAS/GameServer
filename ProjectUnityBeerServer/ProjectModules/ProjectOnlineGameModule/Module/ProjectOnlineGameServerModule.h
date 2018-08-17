#ifndef PROJECT_PROJECTONLINEGAMEMODULE_ONLINEGAMESERVErMODULE
#define PROJECT_PROJECTONLINEGAMEMODULE_ONLINEGAMESERVErMODULE 1

#include "EvilEngine/CoreEngine/CoreEngine/EngineModule.h"
#include "EvilEngine/CoreLib/BasicTypes/Definitions/VolatileArgs.h"

#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngineModule.h"

#include "ProjectModules/ProjectOnlineGameModule/Data/OnlineGameData.hpp"

class ProjectLobbyGameServerModule;

class ProjectOnlineGameServerModule : public CoreGameEngineModule
{
public:

  enum PROJECT_MODULE_TYPE
  {
    PROJECT_MODULETYPE_ONLINEGAME = 104,
  };

  ProjectOnlineGameServerModule();

  virtual void                FrameProcess(float /*deltaTime*/) {};
  virtual String              GetModuleName() DF_OVERRIDE { return StaticStr("ProjectOnlineGameServerModule"); }
  static ProjectOnlineGameServerModule* GetModule(CoreGameEngine* gameEngine);

  ProjectLobbyGameServerModule*         GetServerLobbyGameModule();

  OnlineGameData*             CreateOnlineGame(uint32 accountId, uint32 lobbyGameId);

  void                        AddOnlineGameToCache(OnlineGameData* game);
  std::vector< OnlineGameData* > GetOnlineGamesWithAccount(uint32 accountId);
  OnlineGameData*             GetOnlineGame(uint32 accountId, uint32 gameId);
  OnlineGameData*             GetOnlineGame(uint32 gameId);

private:
  
  uint32                      GetNexGameId();
  bool                        SaveGameInDatabase(OnlineGameData* game);

  std::map< uint32, OnlineGameData* > m_OnlineGameMap;

  uint32                      m_CurrentGameId;
};

#endif /// PROJECT_PROJECTONLINEGAMEMODULE_ONLINEGAMESERVErMODULE
