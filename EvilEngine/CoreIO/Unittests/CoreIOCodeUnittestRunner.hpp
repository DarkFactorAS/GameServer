#pragma once

#include "PreCompile.h"

#include "EvilEngine/CoreBugReport/Common/BugReport.h"
#include "EvilEngine/CoreLib/Assert/DFAssert.h"
#include "EvilEngine/CoreEngine/CoreUnittestManager/CoreBaseUnittestModule.h"

#include "EvilEngine/CoreIO/Unittests/CodeUnitTest/CoreIOLogLargeString.hpp"
#include "EvilEngine/CoreIO/Unittests/CodeUnitTest/CoreIOUserConfigTest.hpp"
#include "EvilEngine/CoreIO/Unittests/CodeUnitTest/CoreIOLogManyStrings.hpp"

class CoreIOCodeUnittestRunner : public CoreBaseUnittestModule
{
public:

  CoreIOCodeUnittestRunner() :
    CoreBaseUnittestModule("CoreIO")
  {
    Add(new CoreIOLogLargeString());
    Add(new CoreIOUserConfigTest());
    Add(new CoreIOLogManyStrings());
  }
};