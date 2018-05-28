#ifndef PROJECT_PROJECTONLINEGAMEMODULE_ONLINEGAMESERVErMODULE
#define PROJECT_PROJECTONLINEGAMEMODULE_ONLINEGAMESERVErMODULE 1

#include "EvilEngine/CoreEngine/CoreEngine/EngineModule.h"
#include "EvilEngine/CoreLib/BasicTypes/Definitions/VolatileArgs.h"

#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngineModule.h"

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

};

#endif /// PROJECT_PROJECTONLINEGAMEMODULE_ONLINEGAMESERVErMODULE
