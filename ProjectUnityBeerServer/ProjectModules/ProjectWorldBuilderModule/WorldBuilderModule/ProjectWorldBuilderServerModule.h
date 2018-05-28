/*************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : EngineModule
* Description : Abstract interface class for CoreGameEngine modules. This interface
*               has to be implemented to be able to run as a sub component in the
*               CoreGameEngine.
*
* Author      : Thor Richard Hansen
*************************************************************************************/

#ifndef PROJECT_PROJECTWORLDBUILDERMODULE_WORLDBUULDERMODULE
#define PROJECT_PROJECTWORLDBUILDERMODULE_WORLDBUULDERMODULE 1

#include "EvilEngine/CoreEngine/CoreEngine/EngineModule.h"
#include "EvilEngine/CoreNetwork/Common/NetworkBase.h"

#include "EvilEngine/CoreLib/BasicTypes/Definitions/VolatileArgs.h"

#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngineModule.h"

#include "EvilGameEngine/CoreGameLogin/Account/AccountManager.h"
#include "EvilGameEngine/CoreGameLogin/Network/GameLoginPacketData.h"

#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/Data/WorldBuilderPacketData.h"

class Playfield;

class ProjectWorldBuilderServerModule : public CoreGameEngineModule
{
public:

  enum PROJECT_MODULE_TYPE
  {
    PROJECT_MODULETYPE_WORLDBUILDER = 100,
  };

  ProjectWorldBuilderServerModule();

  virtual void                FrameProcess(float /*deltaTime*/) {};
  virtual String              GetModuleName() DF_OVERRIDE { return StaticStr("ProjectWorldBuilderServerModule"); }

  std::vector< Playfield* >   LoadPlayfieldList(uint32 accountId, bool loadPublic);
  virtual Playfield*          LoadPlayfield(uint32 /*accountId*/, uint32 /*playfieldId*/);
  uint32                      SavePlayfieldToDb(uint32 accountId, Playfield* playfield );
  uint32                      DeletePlayfield(uint32 accountId, uint32 playfieldId);

  const String&               GetLoginError(uint32 errorCode);
  static uint32               VerifyPlayfieldData(Playfield* playfield);

protected:

  void                        AddLoginError(WorldBuilderPacketData::PacketError errorCode, const String& errorMessage);
  void                        AddLoginError(uint32 errorCode, const String& errorDescription);
  std::map<uint32, String>    m_ErrorMessages;

private:

  void                        LoadPlayfieldSQL(const String& sql, std::vector<Playfield*>& playfieldList);
  void                        SetLastPlayfieldError(uint32 errorCode);

};

#endif /// PROJECT_PROJECTWORLDBUILDERMODULE_WORLDBUULDERMODULE
