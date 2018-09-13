#ifndef PROJECT_PROJECTONLINEGAMEMODULE_ONLINEGAMESERVErMODULE
#define PROJECT_PROJECTONLINEGAMEMODULE_ONLINEGAMESERVErMODULE 1

#include "EvilEngine/CoreEngine/CoreEngine/EngineModule.h"
#include "EvilEngine/CoreLib/BasicTypes/Definitions/VolatileArgs.h"

#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngineModule.h"

class ProjectLobbyGameServerModule;
class GameplayLogic;
class OnlineGameData;
class ActionCard;

class ProjectOnlineGameServerModule : public CoreGameEngineModule
{
public:

  enum PROJECT_MODULE_TYPE
  {
    PROJECT_MODULETYPE_ONLINEGAME = 104,
  };

  ProjectOnlineGameServerModule();

  virtual void                          FrameProcess(float /*deltaTime*/) {};
  virtual String                        GetModuleName() DF_OVERRIDE { return StaticStr("ProjectOnlineGameServerModule"); }
  static ProjectOnlineGameServerModule* GetModule(CoreGameEngine* gameEngine);

  ProjectLobbyGameServerModule*         GetServerLobbyGameModule();

  OnlineGameData*                       CreateOnlineGame(uint32 accountId, uint32 lobbyGameId);
  bool                                  LeaveOnlineGame(uint32 gameId, uint32 accountId);

  std::vector< uint32 >                 GetOnlineGameListFromAccount(uint32 accountId);
  OnlineGameData*                       GetOnlineGame(uint32 gameId);
  OnlineGameData*                       GetOnlineGameWithAccount(uint32 gameId, uint32 accountId);

  void                                  SetPlayerReady( uint32 gameId, uint32 accountId );
  std::vector<ActionCard*>              GetActionCards( uint32 gameId, uint32 accountId);

private:
  
  GameplayLogic*                        m_GameplayLogic;
};

#endif /// PROJECT_PROJECTONLINEGAMEMODULE_ONLINEGAMESERVErMODULE
