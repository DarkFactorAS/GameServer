#ifndef PROJECT_PROJECTGAMEMANAGEMENTMODULE_CLIENTMODULE
#define PROJECT_PROJECTGAMEMANAGEMENTMODULE_CLIENTMODULE 1

#include "EvilEngine/CoreEngine/CoreEngine/EngineModule.h"
#include "EvilEngine/CoreNetwork/Common/NetworkBase.h"

#include "EvilEngine/CoreLib/BasicTypes/Definitions/VolatileArgs.h"

#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngineModule.h"

//#include "EvilGameEngine/CoreGameLogin/Account/AccountManager.h"
#include "EvilGameEngine/CoreGameLogin/Network/GameLoginPacketData.h"

#include "ProjectUnityBeerServer/ProjectModules/ProjectLobbyGameModule/Data/LobbyGameData.hpp"

//#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/Data/WorldBuilderPacketData.h"

class Playfield;

class ProjectGameManagementClientModule : public CoreGameEngineModule
{
public:

  enum PROJECT_MODULE_TYPE
  {
    PROJECT_MODULETYPE_CLIENTGAMEMANAGEMENT = 103,
  };

  ProjectGameManagementClientModule();

  virtual void                FrameProcess(float /*deltaTime*/) {};
  virtual String              GetModuleName() DF_OVERRIDE { return StaticStr("ProjectClientLobbyGameModule"); }

  void                        CreatedGame(LobbyGameData* lobbyGame);
  void                        GameNetworkError(uint32 packetType, uint32 errorCode, const String& errorMessage);

  /// 
  Signal1<void, LobbyGameData* > SignalCreatedGame;
  Signal3<void, uint32, uint32, const String& > SignalGameNetworkError;
};

#endif /// PROJECT_PROJECTGAMEMANAGEMENTMODULE_CLIENTMODULE
