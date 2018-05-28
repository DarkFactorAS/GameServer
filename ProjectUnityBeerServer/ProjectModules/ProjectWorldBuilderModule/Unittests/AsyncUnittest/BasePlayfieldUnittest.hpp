#ifndef PROJECT_WORLDBUILDERMODULE_UNITTESTS_ASYNCUNITTEST_BASEPLAYFIELDUNITTEST
#define PROJECT_WORLDBUILDERMODULE_UNITTESTS_ASYNCUNITTEST_BASEPLAYFIELDUNITTEST 1

#include "PreCompile.h"

#include "EvilEngine/CoreEngine/CoreUnittestManager/CodeUnittest/CoreAsyncUnittest.h"

#include "EvilGameEngine/CoreGameLogin/Account/Account.h"

#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/Data/Playfield.h"
#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/WorldBuilderModule/ProjectWorldBuilderServerModule.h"
#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/Unittests/UnittestWorldBuilderServerModule.hpp"

class BasePlayfieldUnittest : public CoreAsyncUnittest
{
public:

  uint32 m_PlayfieldId = 1959;
  Playfield* m_Playfield = NULL;
  Account* m_TestAccount = NULL;

  BasePlayfieldUnittest(const String& unittestName) : CoreAsyncUnittest(unittestName)
  {
    m_Playfield = new Playfield();
    m_Playfield->SetPlayfieldId(1959);
    m_Playfield->SetPlayfieldName("Uberduber Playfield");
    m_Playfield->SetBoardSizeX( Playfield::BoardSizes::PlayBoardSize);
    m_Playfield->SetBoardSizeY(Playfield::BoardSizes::PlayBoardSize + Playfield::BoardSizes::StartBoardSize);
  }

  virtual bool Cleanup() DF_OVERRIDE
  {
    return CoreAsyncUnittest::Cleanup();
  }

protected:

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

  bool AddPlayfields()
  {
    return AddPlayfield(m_Playfield);
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

};

#endif /// PROJECT_WORLDBUILDERMODULE_UNITTESTS_ASYNCUNITTEST_BASEPLAYFIELDUNITTEST
