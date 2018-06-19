
#include "PreCompile.h"
#include "ProjectGameManagementServerModule.h"

#include "EvilGameEngine/CoreGameLogin/LoginModule/CoreGameServerLoginModule.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/Packets/BaseNetworkPacket.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"

#include "ProjectLobbyGameModule/Data/GameManagementPacketData.h"
#include "ProjectLobbyGameModule/Data/LobbyGameData.hpp"

#include "ProjectLobbyGameModule/NetworkPackets/ClientNetworkPackets/ClientCreatedQuickGameNetworkPacket.h"

#include "ProjectLobbyGameModule/NetworkPackets/ServerNetworkPackets/ServerCreateLobbyGameNetworkPacket.h"
#include "ProjectLobbyGameModule/NetworkPackets/ServerNetworkPackets/ServerGameListNetworkPacket.h"
#include "ProjectLobbyGameModule/NetworkPackets/ServerNetworkPackets/ServerPlayerJoinLobbyGameNetworkPacket.h"
#include "ProjectLobbyGameModule/NetworkPackets/ServerNetworkPackets/ServerPlayerLeaveLobbyGameNetworkPacket.h"
#include "ProjectLobbyGameModule/NetworkPackets/ServerNetworkPackets/ServerJoinQuickGameNetworkPacket.h"
#include "ProjectLobbyGameModule/NetworkPackets/ServerNetworkPackets/ServerLeaveQuickGameNetworkPacket.h"

#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/Data/Playfield.h"
#include "ProjectUnityBeerServer/ProjectModules/ProjectLobbyGameModule/NetworkPackets/ClientNetworkPackets/ClientPlayerLeftLobbyGameNetworkPacket.h"

uint32 ProjectGameManagementServerModule::s_LobbyGameId = 0;

ProjectGameManagementServerModule::ProjectGameManagementServerModule() :
  CoreGameEngineModule(PROJECT_MODULETYPE_GAMEMANAGEMENT),
  m_LastError(0)
{
  RegisterPacketType(GameEnginePacketData::PacketData_ServerCreateLobbyGame, ServerCreateLobbyGameNetworkPacket::Create);
  RegisterPacketType(GameEnginePacketData::PacketData_ServerGameList, ServerGameListNetworkPacket::Create);
  RegisterPacketType(GameEnginePacketData::PacketData_ServerJoinLobbyGame, ServerPlayerJoinLobbyGameNetworkPacket::Create);
  RegisterPacketType(GameEnginePacketData::PacketData_ServerLeaveLobbyGame, ServerPlayerLeaveLobbyGameNetworkPacket::Create);
  RegisterPacketType(GameEnginePacketData::PacketData_ServerJoinQuickGame, ServerJoinQuickGameNetworkPacket::Create);
  RegisterPacketType(GameEnginePacketData::PacketData_ServerLeaveQuickGame, ServerLeaveQuickGameNetworkPacket::Create);

  AddGameError(GameEnginePacketData::ErrorCode_CodeError, "Code Error");
  AddGameError(GameEnginePacketData::ErrorCode_NotLoggedIn, "User not logged in");
}

ProjectGameManagementServerModule::~ProjectGameManagementServerModule()
{
}

void ProjectGameManagementServerModule::AddGameError(GameEnginePacketData::PacketError errorCode, const String& errorMessage)
{
  AddGameError((uint32)errorCode, errorMessage);
}

void ProjectGameManagementServerModule::AddGameError(uint32 errorCode, const String& errorMessage)
{
  // TODO > Check if the string is the same as well If it is different assert here!
  std::map< uint32, String >::iterator itFound = m_ErrorMessages.find(errorCode);
  if (itFound != m_ErrorMessages.end())
  {
    LogWarningFMT("NetworkBase::AddLoginError", "Error message [%d] is already registered (%s)", errorCode, errorMessage.c_str());
  }
  m_ErrorMessages[errorCode] = errorMessage;
}

const String& ProjectGameManagementServerModule::GetGameError(uint32 errorCode)
{
  std::map< uint32, String >::iterator itFound = m_ErrorMessages.find(errorCode);
  if (itFound != m_ErrorMessages.end())
  {
    return itFound->second;
  }
  LogWarningFMT("NetworkBase::GetLoginError", "Error message [%d] not found in map", errorCode);
  return String::zero;
}

void ProjectGameManagementServerModule::OnAccountDisconnected(uint32 accountId)
{
  LobbyGameData* lobbyGame = GetLobbyGameWithAccountId(accountId);
  if (lobbyGame != NULL)
  {
    LeaveLobbyGame(lobbyGame->GetGameId(),accountId);
  }
}

LobbyGameData* ProjectGameManagementServerModule::CreateNewGame(uint32 accountOwnerId, const String& playerName, Playfield* playfield, uint32 robotId)
{
  m_LastError = 0;

  // 1. Check already created games with this player in it.
  // Return errorcode with 
  // - Already own a game in lobby ( auto delete game when disconnected )
  // - Already joined a game in lobby ( should auto leave when disconnecting ? )
  // - In active game. ( Autojoin game ? )

  // 2. Validate playfieldId and robotId

  // 3. Create a new game in db and return Id

  LobbyGameData* newGame = NULL;
  if (playfield != NULL)
  {
    newGame = new LobbyGameData(playfield->GetPlayfieldId());
    newGame->SetPlayfield(playfield);
    newGame->AddPlayer(accountOwnerId, playerName, robotId);
    newGame->SetGameId(++s_LobbyGameId);

    m_AccountLobbyGameMap[accountOwnerId] = newGame->GetGameId();
    m_LobbyGames.push_back(newGame);
  }
  return newGame;
}

std::vector< LobbyGameData* > ProjectGameManagementServerModule::GetOpenGameList()
{
  // Filter away banned players
  // Sort friends first, then LAN, then other games by date/time.

  return GetLobbyGames();
}

LobbyGameData* ProjectGameManagementServerModule::GetLobbyGameWithGameId(uint32 gameId)
{
  for (uint32 index = 0; index < m_LobbyGames.size(); index++)
  {
    LobbyGameData* lobbyGame = m_LobbyGames[index];
    if (lobbyGame != NULL && lobbyGame->GetGameId() == gameId)
    {
      return lobbyGame;
    }
  }
  return NULL;
}

LobbyGameData* ProjectGameManagementServerModule::GetLobbyGameWithAccountId(uint32 accountId)
{
  std::map< uint32, uint32 >::iterator itGame = m_AccountLobbyGameMap.find(accountId);
  if (itGame != m_AccountLobbyGameMap.end())
  {
    return GetLobbyGameWithGameId(itGame->second);
  }
  return NULL;
}

LobbyGameData* ProjectGameManagementServerModule::JoinLobbyGame(uint32 gameId, uint32 accountId, const String& playerName, uint32 robotId)
{
  LobbyGameData* lobbyGame = GetLobbyGameWithGameId( gameId );
  if (lobbyGame != NULL )
  {
    if (!lobbyGame->HasPlayer(accountId))
    {
      m_AccountLobbyGameMap[accountId] = lobbyGame->GetGameId();
      lobbyGame->AddPlayer(new LobbyGamePlayer(accountId, playerName, robotId));
    }
  }
  return lobbyGame;
}

LobbyGameData* ProjectGameManagementServerModule::LeaveLobbyGame(uint32 gameId, uint32 accountId)
{
  LobbyGameData* lobbyGame = GetLobbyGameWithGameId(gameId);
  if (lobbyGame != NULL)
  {
    LobbyGamePlayer* lobbyPlayer = lobbyGame->GetPlayer(accountId);
    if (lobbyPlayer != NULL )
    {
      // Remove from lookup
      RemoveAccountFromLobbyGame(accountId, gameId);

      // Remove player from game
      lobbyGame->RemovePlayer(lobbyPlayer);

      // TODO : Assign a new team leader if the game leader leaves or disband game ?
      if (lobbyGame->GetPlayerList().size() == 0)
      {
        RemoveLobbyGameWithId(gameId);
        return NULL;
      }
      else
      {
        SendPacketToLobbyGamePlayers(lobbyGame, new ClientPlayerLeftLobbyGameNetworkPacket(lobbyGame->GetGameId(), lobbyPlayer));
        SendPacketToClientAccount(accountId,new ClientPlayerLeftLobbyGameNetworkPacket(lobbyGame->GetGameId(), lobbyPlayer));
      }
    }
  }
  return lobbyGame;
}

bool ProjectGameManagementServerModule::RemoveAccountFromLobbyGame(uint32 accountId, uint32 lobbyGameId)
{
  // ToDO : Remove all players

  std::map< uint32, uint32 >::iterator itGame = m_AccountLobbyGameMap.find(accountId);
  if (itGame != m_AccountLobbyGameMap.end())
  {
    if (itGame->second == lobbyGameId)
    {
      m_AccountLobbyGameMap.erase(itGame);
      return true;
    }
  }
  return false;
}

bool ProjectGameManagementServerModule::RemoveLobbyGameWithId(uint32 gameId)
{
  for (std::vector<LobbyGameData*>::iterator itGame = m_LobbyGames.begin(); itGame != m_LobbyGames.end(); ++itGame)
  {
    LobbyGameData* lobbyGame = *itGame;
    if (lobbyGame->GetGameId() == gameId)
    {
      // Remove from lookup
      const std::vector<LobbyGamePlayer*> playerList = lobbyGame->GetPlayerList();
      for (std::vector<LobbyGamePlayer*>::const_iterator itPlayer = playerList.begin(); itPlayer != playerList.end(); itPlayer++ )
      {
        LobbyGamePlayer* lobbyPlayer = *itPlayer;
        RemoveAccountFromLobbyGame(lobbyPlayer->GetAccountId(), gameId);
      }

      m_LobbyGames.erase(itGame);
      delete lobbyGame;
      return true;
    }
  }
  return false;
}

bool ProjectGameManagementServerModule::SendPacketToClientAccount(uint32 accountId, BaseNetworkPacket* packet)
{
  CoreGameEngine* gameEngine = safe_cast<CoreGameEngine*> (GetEngine());
  if (gameEngine != NULL)
  {
    CoreGameServerLoginModule* loginModule = CoreGameServerLoginModule::GetModule(gameEngine);
    if (loginModule != NULL)
    {
      return loginModule->SendPacketToClientAccount(accountId,packet);
    }
  }
  return false;
}

bool ProjectGameManagementServerModule::SendPacketToLobbyGamePlayers(LobbyGameData* lobbyGame, BaseNetworkPacket* packet)
{
  CoreGameEngine* gameEngine = safe_cast<CoreGameEngine*> (GetEngine());
  if (gameEngine != NULL)
  {
    CoreGameServerLoginModule* loginModule = CoreGameServerLoginModule::GetModule(gameEngine);
    if (loginModule != NULL)
    {
      const std::vector<LobbyGamePlayer*> playerList = lobbyGame->GetPlayerList();
      for (std::vector<LobbyGamePlayer*>::const_iterator itPlayer = playerList.begin(); itPlayer != playerList.end(); ++itPlayer)
      {
        LobbyGamePlayer* lobbyPlayer = *itPlayer;

        Account* account = loginModule->GetCachedAccount(lobbyPlayer->GetAccountId());
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

// TODO : create a lobby player for this ?
void ProjectGameManagementServerModule::JoinQuickGame(uint32 accountId)
{
  // Check if we have a free slot in an open game ?
  // Add to quick list

  if (m_QuickGameAccountId != 0 && m_QuickGameAccountId != accountId)
  {
    // TODO : Send data to Game Module
    // TODO : If account goes offline, leave quick list

    uint32 gameId = 1;

    SendPacketToClientAccount(accountId, new ClientCreatedQuickGameNetworkPacket(gameId));
    SendPacketToClientAccount(m_QuickGameAccountId, new ClientCreatedQuickGameNetworkPacket(gameId));

    m_QuickGameAccountId = 0;
  }
  else
  {
    m_QuickGameAccountId = accountId;
  }
}

void ProjectGameManagementServerModule::LeaveQuickGame(uint32 accountId)
{
  if (m_QuickGameAccountId == accountId)
  {
    m_QuickGameAccountId = 0;
  }
}
