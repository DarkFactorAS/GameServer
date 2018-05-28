#pragma once

#include "EvilEngine/CoreBugReport/Common/BugReport.h"
#include "EvilEngine/CoreEngine/CoreUnittestManager/CodeUnittest/CoreUnittest.h"
#include "PreCompile.h"
#include "EvilEngine/CoreLib/Assert/DFAssert.h"
#include "EvilEngine/CoreNetwork/Common/NetworkConnection.h"
#include "EvilEngine/CoreNetwork/UnittestFramework/UnittestNetworkClient.hpp"
#include "EvilEngine/CoreNetwork/UnittestFramework/UnittestNetworkServer.hpp"
#include "EvilEngine/CoreEngine/CoreUnittestManager/CoreBaseUnittestModule.h"

#include "EvilEngine/CoreBugReport/Client/BugReportClient.h"
#include "EvilEngine/CoreBugReport/Server/BugReportServer.h"
#include "EvilEngine/CoreBugReport/Unittests/UnittestFramework/UnittestBugReportServer.hpp"
#include "EvilEngine/CoreBugReport/Unittests/DataFlowUnitTest/SingleBugReport.hpp"
#include "EvilEngine/CoreBugReport/Unittests/DataFlowUnitTest/TwoBugReports.hpp"
#include "EvilEngine/CoreBugReport/Unittests/DataFlowUnitTest/BugReportWithEmbeddedFile.hpp"
#include "EvilEngine/CoreBugReport/Unittests/DataFlowUnitTest/DuplicateReportWithNewVersion.hpp"
#include "EvilEngine/CoreBugReport/Unittests/DataFlowUnitTest/BugReportWithFileOnDisk.hpp"
#include "EvilEngine/CoreBugReport/Unittests/DataFlowUnitTest/BugReportWithLargeMessage.hpp"

class BugReportDataflowUnittestRunner : public CoreBaseUnittestModule
{
public:

  BugReportDataflowUnittestRunner() :
    CoreBaseUnittestModule("BugReportAsync")
  {
    Add(new SingleBugReport());
    Add(new TwoBugReports());
    Add(new BugReportWithEmbeddedFile());
    Add(new BugReportWithFileOnDisk());
    Add(new DuplicateReportWithNewVersion());
    Add(new BugReportWithLargeMessage());
  }

  virtual bool Start() DF_OVERRIDE
  {
    //
    // Setup client
    //
    LogDebug("UT-Debug", "Creating BugClientNet" );
    UnitTestNetworkClient* networkClient = new UnitTestNetworkClient("BugClientNet", USE_PACKETS);
    LogDebug("UT-Debug", "Creating BugClientUTRunner ...");
    BugReportClient* bugClient = BugReportClient::CreateInstance("BugClientUTRunner", "1.0", "0.0.0.0", false);
    LogDebug("UT-Debug", "BugClientUTRunner => SetNetworkBase => BugClientNet");
    bugClient->SetNetworkBase( networkClient );

    //
    // Setup server
    //
    LogDebug("UT-Debug", "Creating BugServerNet");
    UnitTestNetworkServer* networkServer = new UnitTestNetworkServer("BugServerNet", USE_PACKETS);
    LogDebug("UT-Debug", "Creating BugServerUTRunner ...");
    UnittestBugReportServer* bugServer = UnittestBugReportServer::CreateInstance("BugServerUTRunner", 1 );
    bugServer->SetNetworkBase(networkServer);
    LogDebug("UT-Debug", "UnittestBugReportServer => SetNetworkBase => BugServerNet");

    // Cross connect the client send with server receive and server send with client receive
    LogDebug("UT-Debug", "Connect signals");
    networkClient->SignalSendData.Connect(networkServer, &UnitTestNetworkServer::QueueData);
    networkServer->SignalSendData.Connect(networkClient, &UnitTestNetworkClient::QueueData);

    // Start server
    LogDebug("UT-Debug", "Start server");
    networkServer->Start();

    // Start client
    LogDebug("UT-Debug", "Init client");
    if (!bugClient->InitClient(false))
    {
      LogFatal("Main", "Could not start bugreport client");
      return false;
    }
    bugClient->DeleteAllBugReports();

    // Start server
    
    return CoreBaseUnittestModule::Start();
  }

  virtual void RunAsync(float deltaTime) DF_OVERRIDE
  {
    BugReportClient* bugClient = BugReportClient::GetInstanceIfAny();
    if (bugClient != NULL)
    {
      bugClient->Run();
    }
    else
    {
      LogFatal("UnitTest", "BugReportClient was NULL!");
      SetStatus(CoreBaseUnittestModule::STATUS_FAILED);
    }


    BugReportServer* bugServer = UnittestBugReportServer::GetInstanceIfAny();
    if (bugServer != NULL)
    {
      bugServer->Run();
    }
    else
    {
      LogFatal("UnitTest", "BugReportServer was NULL!");
      SetStatus(CoreBaseUnittestModule::STATUS_FAILED);
    }

    CoreBaseUnittestModule::RunAsync(deltaTime);
  }

  virtual bool Cleanup() DF_OVERRIDE
  {
    CoreBaseUnittestModule::Cleanup();
    UnittestBugReportServer::DestroyInstance();
    BugReportClient::DestroyInstance();
    return true;
  }
};
