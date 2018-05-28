/*************************************************************************************
* Project     : Bug Client 2014
* File        : WindowsMain
* Description : Test client to test send a bugreport to the server.
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

#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

#include "resource.h"
#include "windows.h"

#ifndef CLIENT
  #error "CLIENT build not defined! Make sure you have Server-Debug or Server-Release selected"
#endif

static long           s_LastTime      = 0;

int main(int /*argc*/, char* /*argv[]*/ )
{
  UserConfig::SetApplicationName("EvilEngineCodeUnitTest");

  CoreWindowsServer* windowClient = new CoreWindowsServer( "Unittest", "EvilEngine Unittests" );
  windowClient->CreateConsole(IDI_ICON3);

  Log::Initialize( "", "client.log" );
  Log::SetMinimumLogLevel(Log::e_LogDebug);
  LogInfo("Main","Started log");

  CoreUnittestManager* unitManager = new CoreUnittestManager();
  unitManager->Add(new CoreLibUnittestRunner());
  unitManager->Add(new CoreIOCodeUnittestRunner());
  unitManager->Add(new BugReportCodeUnittestRunner());
  unitManager->Add(new BugReportDataflowUnittestRunner());

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
      LogErrorFMT("Main", "Deltatime is less than 0! %d, %d, %f", nowTime, s_LastTime, fDelta);
      fDelta = 0.0f;
    }
    else if (fDelta > 1.0f)
    {
      LogErrorFMT("Main", "Deltatime of %0.2f is > 1sec ", fDelta);
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
  LogInfo("Main","Shutting down server");
  delete unitManager;
  delete windowClient;
  LogInfo("Main","Server shut down successfully");

  Log::Shutdown();

  return 0;
}
