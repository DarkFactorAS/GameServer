/*************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : EngineModule
* Description : Abstract interface class for CoreGameEngine modules. This interface
*               has to be implemented to be able to run as a sub component in the
*               CoreGameEngine.
*
* Author      : Thor Richard Hansen
*************************************************************************************/

#ifndef PROJECT_PROJECTWORLDBUILDERMODULE_PROJECTWORLDBUULDERCLIENTMODULE
#define PROJECT_PROJECTWORLDBUILDERMODULE_PROJECTWORLDBUULDERCLIENTMODULE 1

#include "EvilEngine/CoreEngine/CoreEngine/EngineModule.h"
#include "EvilEngine/CoreNetwork/Common/NetworkBase.h"

#include "EvilEngine/CoreLib/BasicTypes/Definitions/VolatileArgs.h"

#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngineModule.h"

#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/Data/PlayfieldSource.h"

class Playfield;

class ProjectWorldBuilderClientModule : public CoreGameEngineModule
{
public:

  enum PROJECT_MODULE_TYPE
  {
    PROJECT_MODULETYPE_CLIENTWORLDBUILDER = 101,
  };

  ProjectWorldBuilderClientModule();

  virtual void                FrameProcess(float /*deltaTime*/) {};
  virtual String              GetModuleName() DF_OVERRIDE { return StaticStr("ProjectWorldBuilderClientModule"); }

  void                        DeletedPlayfield(PlayfieldSource pfSource);
  void                        SavedPlayfield(PlayfieldSource pfSource);
  void                        LoadedPlayfield(Playfield* playfield);
  void                        PlayfieldNetworkError(uint32 packetType, uint32 errorCode, const String& errorMessage);

  Signal1<void, Playfield* >      SignalLoadedPlayfield;
  Signal1<void, PlayfieldSource > SignalDeletedPlayfield;
  Signal1<void, PlayfieldSource > SignalSavedPlayfield;
  Signal3<void, uint32,uint32, const String& > SignalPlayfieldNetworkError;
};

#endif /// PROJECT_PROJECTWORLDBUILDERMODULE_PROJECTWORLDBUULDERCLIENTMODULE
