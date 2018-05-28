/*************************************************************************************
* Project     : Valyrian Adventures 2014
* File        : main
* Description : Multi-platform board game that is built for iOS/Android/Windows/Mac.
*               
* Author      : Thor Richard Hansen
*************************************************************************************/

#include "PreCompile.h"
#include "EvilEngine/CoreIO/Logger/Logger.h"
#include "EvilEngine/CoreLib/Utils/CoreUtils.h"
#include "EvilEngine/CoreIO/ConfigReader/ConfigReader.h"
#include "EvilEngine/CoreEngine/CoreFramework/Windows/CoreWindowsServer.h"
#include "EvilEngine/CoreIO/UserConfig/UserConfig.h"

#include "EvilEngine/CoreEngine/CoreUnittestManager/CoreUnittestManager.h"
#include "EvilEngine/CoreLib/Unittests/CoreLibUnittestRunner.hpp"
#include "EvilEngine/CoreIO/Unittests/CoreIOCodeUnittestRunner.hpp"
#include "EvilEngine/CoreBugReport/Unittests/BugReportCodeUnittestRunner.hpp"
#include "EvilEngine/CoreBugReport/Unittests/BugReportDataflowUnittestRunner.hpp"

#include "EvilGameEngine/CoreGameLogin/Unittests/AccountAsyncUnittestRunner.hpp"

#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/Unittests/WorldBuilderSyncUnittestRunner.hpp"
#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/Unittests/WorldBuilderAsyncUnittestRunner.hpp"
#include "ProjectUnityBeerServer/ProjectModules/ProjectGameManagementModule/Unittests/GameManagementAsyncUnittestmRuner.hpp"

#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

#include "resource.h"
#include "windows.h"

static long           s_LastTime      = 0;

int main(int /*argc*/, char* /*argv*/[] )
{
  UserConfig::SetApplicationName("UnityBeerServerUnitTest");

  CoreWindowsServer* windowClient = new CoreWindowsServer("Unittest", "Unity BEER Unittests");
  windowClient->CreateConsole(IDI_ICON3);

  Log::Initialize("", "client.log");
  Log::SetMinimumLogLevel(Log::e_LogUnittest);
  LogInfo("Main", "Started log");

  CoreUnittestManager* unitManager = new CoreUnittestManager();

  unitManager->Add(new WorldBuilderSyncUnittestRunner());
  unitManager->Add(new WorldBuilderAsyncUnittestRunner());
  unitManager->Add(new GameManagementAsyncUnittestmRuner());

  uint32 numUnittests = unitManager->GetNumTests();
  LogUnittestFMT("UnitTest", "Running %d unittests ...", numUnittests);

  unitManager->RunSync();

  // Just halt here while the server is running
  while (CoreWindowsCommon::s_RunEngine)
  {
    long nowTime = CoreUtils::GetTime();
    if (s_LastTime == 0)
    {
      s_LastTime = nowTime;
    }
    long deltaTime = nowTime - s_LastTime;
    float fDelta = deltaTime / 1000.0f;

    if (fDelta < 0.0f)
    {
      LogError("Main", String::FormatString("Deltatime is less than 0! %d, %d, %f", nowTime, s_LastTime, fDelta));
      fDelta = 0.0f;
    }
    s_LastTime = nowTime;

    // Run unittest manager
    CoreWindowsCommon::s_RunEngine = unitManager->RunAsync(fDelta);

    boost::this_thread::sleep(boost::posix_time::microseconds(5000));
  }

  if (unitManager->GetNumFailed() > 0)
  {
    LogFatalFMT("UnitTest", "%d/%d unittests failed", unitManager->GetNumFailed(), numUnittests);
  }
  else
  {
    LogUnittestFMT("UnitTest", "%d unittests executes successfully", numUnittests);
  }

  //CoreDatabase::Destroy();
  LogInfo("Main", "Shutting down server");
  delete unitManager;
  delete windowClient;
  LogInfo("Main", "Server shut down successfully");

  Log::Shutdown();

  return 0;
}
