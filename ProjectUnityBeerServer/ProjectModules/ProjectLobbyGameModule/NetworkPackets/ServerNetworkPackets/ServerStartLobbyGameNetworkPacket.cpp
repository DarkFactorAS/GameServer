/*************************************************************************************************
* Project     : EvilEngine Multi-platform 2014-2016
* File        : LoginAccountNetworkPacket
*
* Description : Network packet save playfield on the server
*
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "ServerStartLobbyGameNetworkPackett.h"

#include "ProjectLobbyGameModule/Data/GameManagementPacketData.h"
#include "ProjectLobbyGameModule/NetworkPackets/ClientNetworkPackets/ClientCreatedLobbyGameNetworkPacket.h"
#include "ProjectLobbyGameModule/Data/LobbyGameData.hpp"

ServerStartLobbyGameNetworkPackett::ServerStartLobbyGameNetworkPackett(uint32 lobbyGameId) :
  BaseGameManagementNetworkPacket(GameEnginePacketData::PacketData_ServerStartLobbyGame),
  m_LobbyGameId(lobbyGameId)
{
}

ServerStartLobbyGameNetworkPackett::ServerStartLobbyGameNetworkPackett(const BinaryStream* datastream) :
  BaseGameManagementNetworkPacket(GameEnginePacketData::PacketData_ServerStartLobbyGame, datastream)
{
  m_LobbyGameId = datastream->ReadUInt32();
}

BinaryStream* ServerStartLobbyGameNetworkPackett::GetDataStream()
{
  BinaryStream* datastream = BaseGameManagementNetworkPacket::GetDataStream();
  datastream->WriteUInt32(m_LobbyGameId);
  return datastream;
}

ProjectOnlineGameServerModule* ServerStartLobbyGameNetworkPackett::GetOnlineGameModule()
{
  CoreGameEngine* gameEngine = GetGameEngine();
  if (gameEngine != NULL)
  {
    return safe_cast<ProjectOnlineGameServerModule*> (gameEngine->GetEngineModule(ProjectOnlineGameServerModule::PROJECT_MODULETYPE_ONLINEGAME));
  }
  return NULL;
}


void ServerStartLobbyGameNetworkPackett::Execute()
{
  Account* account = GetAccount();

  ProjectGameManagementServerModule* module = GetModule();
  ProjectOnlineGameServerModule* onlineGameModule = GetOnlineGameModule();

  if (module != NULL && account != NULL && onlineGameModule != NULL)
  {
    LobbyGameData* lobbyGame = module->GetLobbyGameWithAccountId(account->GetAccountId());
    if (lobbyGame != NULL && lobbyGame->GetGameId() == m_LobbyGameId)
    {
      OnlineGameData* onlineGame = onlineGameModule->CreateOnlineGame( lobbyGame->GetPlayfieldId() );
      if (onlineGame != NULL)
      {
        // Add players

        // Notify clients
        // Broadcast to all game players
        //SendPacketToClient(new ClientCreatedLobbyGameNetworkPacket(newGame));

        return;
      }
    }
  }
  SendGameErrorToClient(GameEnginePacketData::ErrorCode_FailedToCreateGame);
}
