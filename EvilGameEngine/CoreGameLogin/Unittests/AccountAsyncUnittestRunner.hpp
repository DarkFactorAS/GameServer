#pragma once

#include "EvilEngine/CoreBugReport/Common/BugReport.h"
#include "EvilEngine/CoreEngine/CoreUnittestManager/CodeUnittest/CoreUnittest.h"
#include "PreCompile.h"
#include "EvilEngine/CoreLib/Assert/DFAssert.h"
#include "EvilEngine/CoreNetwork/Common/NetworkConnection.h"
#include "EvilEngine/CoreNetwork/UnittestFramework/UnittestNetworkClient.hpp"
#include "EvilEngine/CoreNetwork/UnittestFramework/UnittestNetworkServer.hpp"
#include "EvilEngine/CoreEngine/CoreUnittestManager/CoreBaseUnittestModule.h"

#include "EvilGameEngine/CoreGameLogin/LoginModule/CoreGameClientLoginModule.h"
#include "EvilGameEngine/CoreGameLogin/Unittests/UnittestFramework/UnittestGameServerLoginModule.hpp"

#include "EvilGameEngine/CoreGameEngine/Unittests/UnittestFramework/UnittestGameEngineClient.hpp"
#include "EvilGameEngine/CoreGameEngine/Unittests/UnittestFramework/UnittestGameEngineServer.hpp"

#include "EvilGameEngine/CoreGameLogin/Unittests/AsyncUnittest/CreateAccountUnittest.hpp"
#include "EvilGameEngine/CoreGameLogin/Unittests/AsyncUnittest/LoginAccountUnittest.hpp"
#include "EvilGameEngine/CoreGameLogin/Unittests/AsyncUnittest/AccountErrorUnittest.hpp"

class AccountAsyncUnittestRunner : public CoreBaseUnittestModule
{
public:

  AccountAsyncUnittestRunner() :
    CoreBaseUnittestModule("AccountAsync")
  {
    Add(new CreateAccountUnittest());
    Add(new LoginAccountUnittest());
    Add(new AccountErrorUnittest());
  }

  virtual bool Start() DF_OVERRIDE
  {
    //
    // Setup client
    //
    UnitTestNetworkClient* networkClient = new UnitTestNetworkClient("GameEngineClientNet", USE_PACKETS);
    UnittestGameEngineClient* engineClient = UnittestGameEngineClient::CreateInstance("GameEngineUTRunner", 1);
    engineClient->SetNetworkBase(networkClient);
    engineClient->AddEngineModule( new CoreGameClientLoginModule() );

    //
    // Setup server
    //
    UnitTestNetworkServer* networkServer = new UnitTestNetworkServer("GameEngineServerNet", USE_PACKETS);
    UnittestGameEngineServer* engineServer = UnittestGameEngineServer::CreateInstance("GameEngineUTRunner", 1);
    engineServer->SetGameKey("UnitTestGame");
    engineServer->SetNetworkBase(networkServer);
    engineServer->AddEngineModule(new UnittestGameServerLoginModule());

    // Cross connect the client send with server receive and server send with client receive
    networkClient->SignalSendData.Connect(networkServer, &UnitTestNetworkServer::QueueData);
    networkServer->SignalSendData.Connect(networkClient, &UnitTestNetworkClient::QueueData);

    // Start server
    networkServer->Start();

    if (networkClient->MustReconnect())
    {
      networkClient->ConnectAsync();
    }

    return CoreBaseUnittestModule::Start();
  }

  virtual void RunAsync(float deltaTime) DF_OVERRIDE
  {
    UnittestGameEngineClient* engineClient = UnittestGameEngineClient::GetInstanceIfAny();
    if (engineClient != NULL)
    {
      engineClient->Run();
    }
    else
    {
      LogFatal("UnitTest", "GameEngineClient was NULL!");
      SetStatus(CoreBaseUnittestModule::STATUS_FAILED);
    }

    UnittestGameEngineServer* engineServer = UnittestGameEngineServer::GetInstanceIfAny();
    if (engineServer != NULL)
    {
      engineServer->Run();
    }
    else
    {
      LogFatal("UnitTest", "GameEngineServer was NULL!");
      SetStatus(CoreBaseUnittestModule::STATUS_FAILED);
    }

    CoreBaseUnittestModule::RunAsync(deltaTime);
  }

  virtual bool Cleanup() DF_OVERRIDE
  {
    CoreBaseUnittestModule::Cleanup();
    return true;
  }
};
