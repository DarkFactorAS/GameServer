#pragma once

#include "PreCompile.h"
#include "EvilEngine/CoreEngine/CoreUnittestManager/CoreBaseUnittestModule.h"

#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/Unittests/SyncUnittest/VerifyPlayfieldDataUnittest.hpp"

class WorldBuilderSyncUnittestRunner : public CoreBaseUnittestModule
{
public:

  WorldBuilderSyncUnittestRunner() :
    CoreBaseUnittestModule("WorldBuilderSync")
  {
    Add(new VerifyPlayfieldDataUnittest());
  }
};