#pragma once
#include "PreCompile.h"

#include "EvilEngine/CoreEngine/CoreUnittestManager/CodeUnittest/CoreAsyncUnittest.h"

#include "EvilGameEngine/CoreGameLogin/Account/Account.h"
#include "EvilGameEngine/CoreGameLogin/Unittests/UnittestFramework/UnittestGameServerLoginModule.hpp"

#include "EvilGameEngine/CoreGameLogin/Unittests/UnittestFramework/UnittestGameServerLoginModule.hpp"

#include "ProjectUnityBeerServer/ProjectModules/ProjectGameManagementModule/Data/LobbyGameData.hpp"
#include "ProjectUnityBeerServer/ProjectModules/ProjectGameManagementModule/Data/LobbyGamePlayer.hpp"
#include "ProjectUnityBeerServer/ProjectModules/ProjectGameManagementModule/Logic/GameManagementModule/ProjectGameManagementClientModule.h"
#include "ProjectUnityBeerServer/ProjectModules/ProjectGameManagementModule/Logic/GameManagementModule/ProjectGameManagementServerModule.h"

class BaseGameUnittest : public CoreAsyncUnittest
{

public:

  Account*          m_TestAccount = NULL;
  LobbyGameData*    m_Game        = NULL;
  LobbyGamePlayer*  m_Player1     = NULL;

  BaseGameUnittest(const String& unittestName) : CoreAsyncUnittest(unittestName)
  {
  }

  virtual bool Cleanup() DF_OVERRIDE
  {
    return CoreAsyncUnittest::Cleanup();
  }

protected:

  ProjectGameManagementClientModule* GetClientModule()
  {
    UnittestGameEngineClient* gameClient = UnittestGameEngineClient::GetInstanceIfAny();
    if (gameClient != NULL)
    {
      return safe_cast<ProjectGameManagementClientModule*> (gameClient->GetEngineModule(ProjectGameManagementClientModule::PROJECT_MODULETYPE_CLIENTGAMEMANAGEMENT));
    }
    return NULL;
  }

  ProjectGameManagementServerModule* GetServerModule()
  {
    UnittestGameEngineServer* gameServer = UnittestGameEngineServer::GetInstanceIfAny();
    if (gameServer != NULL)
    {
      return safe_cast<ProjectGameManagementServerModule*> (gameServer->GetEngineModule(ProjectGameManagementServerModule::PROJECT_MODULETYPE_GAMEMANAGEMENT));
    }
    return NULL;
  }

  bool LoginAutenticatedAccount()
  {
    m_TestAccount = new Account(1337, "Olla Bolla", "TestDewd1", "Password12", 0);
    m_TestAccount->SetAuthenticated(true);
    return LoginAccount(m_TestAccount);
  }

  bool LoginAccount(Account* account)
  {
    UnittestGameEngineServer* gameServer = UnittestGameEngineServer::GetInstanceIfAny();
    if (gameServer != NULL)
    {
      UnitTestNetworkServer* networkServer = safe_cast<UnitTestNetworkServer*>  (gameServer->GetNetwork());
      if (networkServer == NULL)
      {
        Failed("Could not get network server");
        return false;
      }

      uint32 connectionInstance = networkServer->GetConnectionInstance();

      UnittestGameServerLoginModule* serverLoginModule = safe_cast<UnittestGameServerLoginModule*> (gameServer->GetEngineModule(EngineModule::COREMODULETYPE_SERVERLOGIN));
      if (serverLoginModule != NULL)
      {
        serverLoginModule->AddAccountToCache(account);
        serverLoginModule->AddOnlineAccount(account, connectionInstance);
        return true;
      }
      else
      {
        Failed("UnittestGameServerLoginModule is NULL");
      }
    }
    else
    {
      Failed("UnittestGameEngineServer is NULL");
    }
    return false;
  }

  bool AddGame(uint32 playfieldId)
  {
    uint32 accountId = 1;
    uint32 robotId = 1;

    m_Player1 = new LobbyGamePlayer(accountId, "Newbie", robotId);
    m_Game = new LobbyGameData( 123 ); // GameId
    m_Game->AddPlayer(m_Player1);

    Playfield* playfield = GetPlayfield(accountId, playfieldId);
    if (playfield != NULL)
    {
      m_Game->SetPlayfield(playfield);
      return AddGame(m_Game);
    }
    return false;
  }

  bool AddGame(LobbyGameData* /*gameData*/)
  {
    return true;
  }

  bool AddPlayfield(uint32 playfieldId)
  {
    Playfield* playfield = new Playfield();
    playfield->SetPlayfieldId(playfieldId);
    playfield->SetPlayfieldName("Uberduber Playfield");
    playfield->SetBoardSizeX(Playfield::BoardSizes::PlayBoardSize);
    playfield->SetBoardSizeY(Playfield::BoardSizes::PlayBoardSize + Playfield::BoardSizes::StartBoardSize);
    return AddPlayfield(playfield);
  }

  bool AddPlayfield(Playfield* playfield)
  {
    UnittestGameEngineServer* gameServer = UnittestGameEngineServer::GetInstanceIfAny();
    if (gameServer != NULL)
    {
      UnittestWorldBuilderServerModule* serverWorldbuilderModule = safe_cast<UnittestWorldBuilderServerModule*> (gameServer->GetEngineModule(ProjectWorldBuilderServerModule::PROJECT_MODULETYPE_WORLDBUILDER));
      if (serverWorldbuilderModule != NULL)
      {
        serverWorldbuilderModule->AddPlayfield(playfield);
        return true;
      }
      else
      {
        Failed("UnittestWorldBuilderServerModule is NULL");
      }
    }
    else
    {
      Failed("UnittestGameEngineServer is NULL");
    }
    return false;
  }

  Playfield* GetPlayfield(uint32 accountId, uint playfieldId)
  {
    UnittestGameEngineServer* gameServer = UnittestGameEngineServer::GetInstanceIfAny();
    if (gameServer != NULL)
    {
      UnittestWorldBuilderServerModule* serverWorldbuilderModule = safe_cast<UnittestWorldBuilderServerModule*> (gameServer->GetEngineModule(ProjectWorldBuilderServerModule::PROJECT_MODULETYPE_WORLDBUILDER));
      if (serverWorldbuilderModule != NULL)
      {
        return serverWorldbuilderModule->LoadPlayfield(accountId, playfieldId);
      }
    }
    return NULL;
  }

};
