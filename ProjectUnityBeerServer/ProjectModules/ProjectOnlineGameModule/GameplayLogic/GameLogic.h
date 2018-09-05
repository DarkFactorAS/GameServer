#ifndef PROJECT_PROJECTONLINEGAMEMODULE_GAMEPLAYLOGIC_GAMELOGIC
#define PROJECT_PROJECTONLINEGAMEMODULE_GAMEPLAYLOGIC_GAMELOGIC 1

#include "EvilEngine/CoreDatabase/CoreDatabase.h"

#include "EvilEngine/CoreEngine/CoreEngine/EngineModule.h"
#include "EvilEngine/CoreLib/BasicTypes/Definitions/VolatileArgs.h"

#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngineModule.h"

#include "ProjectModules/ProjectOnlineGameModule/Data/OnlineGameData.hpp"

class OnlineGameData;

class GameLogic
{
public:

  GameLogic();

  uint32          GetNexGameId();

  bool            SaveGameInDatabase(OnlineGameData* onlineGame);

private:
  static uint32   s_CurrentGameId;


#if DATABASE
  CoreDatabase* m_Database;
#endif
};

#endif /// PROJECT_PROJECTONLINEGAMEMODULE_GAMEPLAYLOGIC_GAMELOGIC
