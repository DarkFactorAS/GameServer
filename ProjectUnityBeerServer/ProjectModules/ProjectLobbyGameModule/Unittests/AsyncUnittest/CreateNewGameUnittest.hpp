#pragma once

#include "BaseGameUnittest.hpp"
#include "ProjectUnityBeerServer/ProjectModules/ProjectGameManagementModule/Unittests/UnittestGameManagementServerModule.hpp"
#include "ProjectUnityBeerServer/ProjectModules/ProjectGameManagementModule/NetworkPackets/ServerNetworkPackets/ServerCreateGameNetworkPacket.h"

class CreateNewGameUnittest : public BaseGameUnittest, public SignalTarget
{
public:

  CreateNewGameUnittest() : BaseGameUnittest("CreateNewGame")
  {
  }

  virtual bool Execute() DF_OVERRIDE
  {
    uint32 playfieldId = 1999;

    if (!LoginAutenticatedAccount())
    {
      return false;
    }

    if (!AddPlayfield(playfieldId))
    {
      return false;
    }

    if (!AddGame(playfieldId))
    {
      return false;
    }

    ProjectGameManagementClientModule* clientModule = GetClientModule();
    if (clientModule == NULL)
    {
      Failed("ProjectGameManagementClientModule is NULL");
      return false;
    }

    UnittestGameManagementServerModule* serverModule = safe_cast<UnittestGameManagementServerModule*> (GetServerModule() );
    if (serverModule == NULL)
    {
      Failed("ProjectGameManagementServerModule is NULL");
      return false;
    }

    // Set game to make sure we get correct ID back
    serverModule->SetGame(m_Game);

    clientModule->SignalCreatedGame.Connect(this, &CreateNewGameUnittest::SlotCreatedGame);
    clientModule->SignalGameNetworkError.Connect(this, &CreateNewGameUnittest::SlotGameNetworkError);

    UnittestGameEngineClient* gameClient = UnittestGameEngineClient::GetInstanceIfAny();
    if (gameClient != NULL)
    {
      gameClient->SendPacketToServer(new ServerCreateGameNetworkPacket(playfieldId, m_Player1->GetRobotId() ));
      return true;
    }
    return false;
  }

  void SlotCreatedGame(LobbyGameData* lobbyGame )
  {
    if (m_Game->GetGameId() != lobbyGame->GetGameId()) 
    {
      Failed("Wrong gameId");
    }
    else if (m_Game->GetPlayfield() == NULL || lobbyGame->GetPlayfield() == NULL || m_Game->GetPlayfield()->GetPlayfieldId() != lobbyGame->GetPlayfield()->GetPlayfieldId() )
    {
      Failed("Wrong playfield");
    }
    else
    {
      Succeeded();
    }
  }

  void SlotGameNetworkError(uint32 /*packetType*/, uint32 errorCode, const String& errorMessage)
  {
    Failed(String::FormatString("Received game network error : (%d/%s)", errorCode, errorMessage.c_str()));
  }


};

