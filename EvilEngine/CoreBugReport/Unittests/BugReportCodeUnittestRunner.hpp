#pragma once

#include "PreCompile.h"
#include "EvilEngine/CoreEngine/CoreUnittestManager/CoreBaseUnittestModule.h"

#include "EvilEngine/CoreBugReport/Unittests/CodeUnitTest/DeleteSavedBugreportUnitTest.hpp"

class BugReportCodeUnittestRunner : public CoreBaseUnittestModule

{
public:

  BugReportCodeUnittestRunner() :
    CoreBaseUnittestModule("BugReportCode")
  {
    Add(new DeleteSavedBugreportUnitTest());
  }
};