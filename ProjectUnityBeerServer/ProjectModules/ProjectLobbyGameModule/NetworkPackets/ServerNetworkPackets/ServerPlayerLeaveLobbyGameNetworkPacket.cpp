/*************************************************************************************************
* Project     : EvilEngine Multi-platform 2014-2016
* File        : LoginAccountNetworkPacket
*
* Description : Network packet save playfield on the server
*
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "ServerPlayerLeaveLobbyGameNetworkPacket.h"

#include "ProjectLobbyGameModule/Data/GameManagementPacketData.h"
#include "ProjectLobbyGameModule/NetworkPackets/ClientNetworkPackets/ClientPlayerLeftLobbyGameNetworkPacket.h"
#include "ProjectLobbyGameModule/Data/LobbyGameData.hpp"

ServerPlayerLeaveLobbyGameNetworkPacket::ServerPlayerLeaveLobbyGameNetworkPacket(uint32 gameId) :
  BaseGameManagementNetworkPacket(GameEnginePacketData::PacketData_ServerLeaveLobbyGame),
  m_GameId(gameId)
{
}

ServerPlayerLeaveLobbyGameNetworkPacket::ServerPlayerLeaveLobbyGameNetworkPacket(const BinaryStream* datastream) :
  BaseGameManagementNetworkPacket(GameEnginePacketData::PacketData_ServerLeaveLobbyGame, datastream)
{
  m_GameId = datastream->ReadUInt32();
}

BinaryStream* ServerPlayerLeaveLobbyGameNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseGameManagementNetworkPacket::GetDataStream();
  datastream->WriteUInt32(m_GameId);
  return datastream;
}

void ServerPlayerLeaveLobbyGameNetworkPacket::Execute()
{
  Account* account = GetAccount();

  ProjectLobbyGameServerModule* module = GetModule();
  if (module != NULL && account != NULL)
  {
    // Have to notify players first since player and game might be removed and deleted
    //LobbyGameData* lobbyGame = module->GetLobbyGameWithGameId(m_GameId);
    //if (lobbyGame != NULL)
    //{
    //  LobbyGamePlayer* lobbyPlayer = lobbyGame->GetPlayer(account->GetAccountId());
    //  if (lobbyPlayer != NULL)
    //  {
    //    SendPacketToLobbyGamePlayers(lobbyGame, new ClientPlayerLeftLobbyGameNetworkPacket(lobbyGame->GetGameId(), lobbyPlayer));
    //  }
    //}

    // Do the actual removal
    module->LeaveLobbyGame(m_GameId, account->GetAccountId());
  }
  else
  {
    SendGameErrorToClient(GameEnginePacketData::ErrorCode_FailedToCreateGame);
  }
}
