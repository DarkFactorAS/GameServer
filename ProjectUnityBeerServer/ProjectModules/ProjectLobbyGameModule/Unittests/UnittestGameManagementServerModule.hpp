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

class UnittestGameManagementServerModule : public ProjectLobbyGameServerModule
{
public:

  void SetGame(LobbyGameData* game)
  {
    m_TestGame = game;
  }

  virtual LobbyGameData* CreateNewGame(uint32 accountId, const String& playerName, Playfield* playfield, uint32 robotId) DF_OVERRIDE
  {
    LobbyGameData* newGame = ProjectLobbyGameServerModule::CreateNewGame(accountId, playerName, playfield, robotId);
    if ( newGame != NULL && m_TestGame != NULL)
    {
      newGame->SetGameId(m_TestGame->GetGameId());
    }
    return newGame;
  }

  LobbyGameData* m_TestGame;

};
