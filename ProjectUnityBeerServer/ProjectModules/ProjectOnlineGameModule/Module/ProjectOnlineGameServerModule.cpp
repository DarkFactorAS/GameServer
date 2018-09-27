/*************************************************************************************
* Project     : EvilEngine Multiplatform 2014-2018
* File        : ProjectOnlineGameServerModule
* Description : Abstract interface class for CoreGameEngine modules. This interface
*               has to be implemented to be able to run as a sub component in the
*               CoreGameEngine.
*
* Author      : Thor Richard Hansen
*************************************************************************************/

#include "PreCompile.h"
#include "ProjectOnlineGameServerModule.h"

#include "EvilEngine/CoreDatabase/CoreDatabase.h"

#include "ProjectModules/ProjectOnlineGameModule/Data/OnlineGamePacketData.h"
#include "ProjectModules/ProjectOnlineGameModule/Data/OnlineGameData.hpp"
#include "ProjectModules/ProjectOnlineGameModule/GameplayLogic/GameplayLogic.h"
#include "ProjectModules/ProjectOnlineGameModule/NetworkPackets/ClientNetworkPackets/ClientReceivedActionCardsNetworkPacket.h"
#include "ProjectModules/ProjectOnlineGameModule/NetworkPackets/ServerNetworkPackets/ServerCreateOnlineGameNetworkPacket.h"
#include "ProjectModules/ProjectOnlineGameModule/NetworkPackets/ServerNetworkPackets/ServerRequestGameDataNetworkPacket.h"
#include "ProjectModules/ProjectOnlineGameModule/NetworkPackets/ServerNetworkPackets/ServerPlayerLeaveOnlineGameNetworkPacket.h"
#include "ProjectModules/ProjectOnlineGameModule/NetworkPackets/ServerNetworkPackets/ServerPlayerStartRoundNetworkPacket.h"

// Other modules
#include "ProjectModules/ProjectLobbyGameModule/Module/ProjectLobbyGameServerModule.h"
#include "ProjectModules/ProjectWorldBuilderModule/WorldBuilderModule/ProjectWorldBuilderServerModule.h"
#include "ProjectModules/ProjectOnlineGameModule/Module/ProjectOnlineGameServerModule.h"
#include "EvilGameEngine/CoreGameLogin/LoginModule/CoreGameServerLoginModule.h"

#include "ProjectLobbyGameModule/Data/LobbyGameData.hpp"
#include "ProjectLobbyGameModule/Data/LobbyGamePlayer.hpp"

#include "ProjectModules/ProjectOnlineGameModule/GameplayLogic/GameplayLogic.h"

ProjectOnlineGameServerModule::ProjectOnlineGameServerModule() :
  CoreGameEngineModule(PROJECT_MODULETYPE_ONLINEGAME)
{
  m_GameplayLogic = new GameplayLogic(this);

  RegisterPacketType(OnlineGamePacketData::PacketData_ServerCreateOnlineGame, ServerCreateOnlineGameNetworkPacket::Create);
  RegisterPacketType(OnlineGamePacketData::PacketData_ServerRequestGameData, ServerRequestGameDataNetworkPacket::Create);
  RegisterPacketType(OnlineGamePacketData::PacketData_ServerLeaveOnlineGame, ServerPlayerLeaveOnlineGameNetworkPacket::Create);
  RegisterPacketType(OnlineGamePacketData::PacketData_ServerPlayerStartRound, ServerPlayerStartRoundNetworkPacket::Create);
}

ProjectOnlineGameServerModule* ProjectOnlineGameServerModule::GetModule(CoreGameEngine* gameEngine)
{
  if (gameEngine != NULL)
  {
    return safe_cast<ProjectOnlineGameServerModule*> (gameEngine->GetEngineModule(ProjectOnlineGameServerModule::PROJECT_MODULETYPE_ONLINEGAME));
  }
  return NULL;
}

ProjectLobbyGameServerModule* ProjectOnlineGameServerModule::GetServerLobbyGameModule()
{
  return ProjectLobbyGameServerModule::GetModule(m_CoreEngine);
}
CoreGameServerLoginModule* ProjectOnlineGameServerModule::GetServerLoginModule()
{
  return CoreGameServerLoginModule::GetModule(m_CoreEngine);
}

OnlineGameData* ProjectOnlineGameServerModule::CreateOnlineGame(uint32 accountId, uint32 lobbyGameId)
{
  ProjectLobbyGameServerModule* lobbyModule = GetServerLobbyGameModule();
  ProjectWorldBuilderServerModule* worldbuilderModule = ProjectWorldBuilderServerModule::GetModule( m_CoreEngine );
  if (lobbyModule != NULL && worldbuilderModule != NULL )
  {
    LobbyGameData* lobbyGame = lobbyModule->GetLobbyGameWithAccountId(accountId);
    if (lobbyGame != NULL && lobbyGame->GetGameId() == lobbyGameId)
    {
      Playfield* playfield = worldbuilderModule->GetPlayfield(lobbyGame->GetPlayfieldId());
      if (playfield != NULL)
      {
        return m_GameplayLogic->CreateOnlineGame(accountId, lobbyGame, playfield);
      }
    }
  }
  return NULL;
}

std::vector< uint32 > ProjectOnlineGameServerModule::GetOnlineGameListFromAccount(uint32 accountId)
{
  return m_GameplayLogic->GetOnlineGameListFromAccount(accountId);
}
OnlineGameData* ProjectOnlineGameServerModule::GetOnlineGameWithAccount(uint32 gameId, uint32 accountId)
{
  return m_GameplayLogic->GetOnlineGameWithAccount(gameId, accountId );
}
OnlineGameData* ProjectOnlineGameServerModule::GetOnlineGame(uint32 gameId)
{
  return m_GameplayLogic->GetOnlineGame(gameId);
}
bool ProjectOnlineGameServerModule::LeaveOnlineGame(uint32 gameId, uint32 accountId)
{
  return m_GameplayLogic->LeaveOnlineGame(gameId,accountId);
}
void ProjectOnlineGameServerModule::SetPlayerReady(uint32 gameId, uint32 accountId)
{
  m_GameplayLogic->SetPlayerReady(gameId, accountId);
}
void ProjectOnlineGameServerModule::SetAllPlayersReady(uint32 gameId)
{
  m_GameplayLogic->SetStatusOnAllPlayers(gameId, OnlineGamePlayer::PlayerState::PlayerState_WaitingForWards);
}
bool ProjectOnlineGameServerModule::AreAllPlayersReady(uint32 gameId)
{
  return m_GameplayLogic->AreAllPlayersReady(gameId);
}

void ProjectOnlineGameServerModule::DealActionCards(uint32 gameId)
{
  OnlineGameData* onlineGame = GetOnlineGame(gameId);
  if (onlineGame == NULL)
  {
    return;
  }

  if (!AreAllPlayersReady(gameId))
  {
    return;
  }

  const std::vector<OnlineGamePlayer*> playerList = onlineGame->GetPlayerList();
  for (std::vector<OnlineGamePlayer*>::const_iterator itPlayer = playerList.begin(); itPlayer != playerList.end(); ++itPlayer)
  {
    OnlineGamePlayer* onlinePlayer = *itPlayer;
    std::vector<ActionCard*> actionCardList = m_GameplayLogic->GetActionCards(onlineGame, onlinePlayer);
    ClientReceivedActionCardsNetworkPacket* networkPacket = new ClientReceivedActionCardsNetworkPacket(gameId, onlinePlayer->GetAccountId(), actionCardList);

    SendPacketToOnlineGamePlayers(onlineGame, networkPacket);
    //SendPacketToOnlineGamePlayer( onlinePlayer, networkPacket);
  }
}

bool ProjectOnlineGameServerModule::SendPacketToOnlineGamePlayers(uint32 gameId, BaseNetworkPacket* packet)
{
  OnlineGameData* onlineGame = m_GameplayLogic->GetOnlineGame(gameId);
  return SendPacketToOnlineGamePlayers( onlineGame, packet );
}

bool ProjectOnlineGameServerModule::SendPacketToOnlineGamePlayers(OnlineGameData* onlineGame, BaseNetworkPacket* packet)
{
  CoreGameEngine* gameEngine = GetGameEngine();
  if (gameEngine != NULL)
  {
    CoreGameServerLoginModule* loginModule = GetServerLoginModule();
    if (loginModule != NULL)
    {
      const std::vector<OnlineGamePlayer*> playerList = onlineGame->GetPlayerList();
      for (std::vector<OnlineGamePlayer*>::const_iterator itPlayer = playerList.begin(); itPlayer != playerList.end(); ++itPlayer)
      {
        OnlineGamePlayer* onlinePlayer = *itPlayer;

        Account* account = loginModule->GetCachedAccount(onlinePlayer->GetAccountId());
        if (account != NULL)
        {
          gameEngine->SendPacketToEndpoint(account->GetConnectionId(), packet);
        }
      }
      return true;
    }
  }
  return false;
}

