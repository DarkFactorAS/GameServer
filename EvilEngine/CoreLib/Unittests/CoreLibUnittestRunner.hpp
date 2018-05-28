#pragma once

#include "PreCompile.h"

#include "EvilEngine/CoreBugReport/Common/BugReport.h"
#include "EvilEngine/CoreLib/Assert/DFAssert.h"
#include "EvilEngine/CoreEngine/CoreUnittestManager/CoreBaseUnittestModule.h"

#include "CoreStringVeryLongString.hpp"

class CoreLibUnittestRunner : public CoreBaseUnittestModule
{
public:

  CoreLibUnittestRunner() :
    CoreBaseUnittestModule("CoreLib")
  {
    // Add unittests
    Add(new CoreStringVeryLongString());
  }
};