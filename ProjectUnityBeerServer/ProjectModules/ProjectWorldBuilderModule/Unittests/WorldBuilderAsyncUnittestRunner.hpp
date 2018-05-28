#pragma once

#include "PreCompile.h"

#include "EvilEngine/CoreBugReport/Common/BugReport.h"
#include "EvilEngine/CoreEngine/CoreUnittestManager/CodeUnittest/CoreUnittest.h"
#include "EvilEngine/CoreLib/Assert/DFAssert.h"
#include "EvilEngine/CoreNetwork/Common/NetworkConnection.h"
#include "EvilEngine/CoreNetwork/UnittestFramework/UnittestNetworkClient.hpp"
#include "EvilEngine/CoreNetwork/UnittestFramework/UnittestNetworkServer.hpp"
#include "EvilEngine/CoreEngine/CoreUnittestManager/CoreBaseUnittestModule.h"

#include "EvilGameEngine/CoreGameLogin/LoginModule/CoreGameClientLoginModule.h"
#include "EvilGameEngine/CoreGameLogin/Unittests/UnittestFramework/UnittestGameServerLoginModule.hpp"

#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/WorldBuilderModule/ProjectWorldBuilderClientModule.h"
#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/Unittests/UnittestWorldBuilderServerModule.hpp"
#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/Unittests/AsyncUnittest/DeletePlayfieldUnittest.hpp"
#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/Unittests/AsyncUnittest/SavePlayfieldUnittest.hpp"
#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/Unittests/AsyncUnittest/LoadPlayfieldUnittest.hpp"

class WorldBuilderAsyncUnittestRunner : public CoreBaseUnittestModule
{
public:

  WorldBuilderAsyncUnittestRunner() :
    CoreBaseUnittestModule("WorldBuilderAsync")
  {
    Add(new DeletePlayfieldUnittest());
    Add(new SavePlayfieldUnittest());
    Add(new LoadPlayfieldUnittest());
  }

  virtual bool Start() DF_OVERRIDE
  {
    //
    // Setup client
    //
    UnitTestNetworkClient* networkClient = new UnitTestNetworkClient("GameEngineClientNet", USE_PACKETS);
    UnittestGameEngineClient* engineClient = UnittestGameEngineClient::CreateInstance("GameEngineUTRunner", 1);
    engineClient->SetNetworkBase(networkClient);
    engineClient->AddEngineModule(new CoreGameClientLoginModule());
    engineClient->AddEngineModule(new ProjectWorldBuilderClientModule());

    //
    // Setup server
    //
    UnitTestNetworkServer* networkServer = new UnitTestNetworkServer("GameEngineServerNet", USE_PACKETS);
    UnittestGameEngineServer* engineServer = UnittestGameEngineServer::CreateInstance("GameEngineUTRunner", 1);
    engineServer->SetNetworkBase(networkServer);
    engineServer->AddEngineModule(new UnittestGameServerLoginModule());
    engineServer->AddEngineModule(new UnittestWorldBuilderServerModule());

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
    UnittestGameEngineClient::DestroyInstance();
    UnittestGameEngineServer::DestroyInstance();
    CoreBaseUnittestModule::Cleanup();
    return true;
  }
};
