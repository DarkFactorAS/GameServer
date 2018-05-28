/*************************************************************************************************
* Project     : EvilEngine Multi-platform 2014-2016
* File        : LoginAccountNetworkPacket
*
* Description : Network packet save playfield on the server
*
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "ServerPlayerJoinLobbyGameNetworkPacket.h"

#include "ProjectGameManagementModule/Data/GameManagementPacketData.h"
#include "ProjectGameManagementModule/NetworkPackets/ClientNetworkPackets/ClientPlayerJoinedLobbyGameNetworkPacket.h"
#include "ProjectGameManagementModule/Data/LobbyGameData.hpp"

ServerPlayerJoinLobbyGameNetworkPacket::ServerPlayerJoinLobbyGameNetworkPacket(uint32 gameId, uint32 robotId) :
  BaseGameManagementNetworkPacket(GameEnginePacketData::PacketData_ServerJoinLobbyGame),
  m_GameId(gameId),
  m_RobotId(robotId)
{
}

ServerPlayerJoinLobbyGameNetworkPacket::ServerPlayerJoinLobbyGameNetworkPacket(const BinaryStream* datastream) :
  BaseGameManagementNetworkPacket(GameEnginePacketData::PacketData_ServerJoinLobbyGame, datastream)
{
  m_GameId = datastream->ReadUInt32();
  m_RobotId = datastream->ReadUInt32();
}

BinaryStream* ServerPlayerJoinLobbyGameNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseGameManagementNetworkPacket::GetDataStream();
  datastream->WriteUInt32(m_GameId);
  return datastream;
}

void ServerPlayerJoinLobbyGameNetworkPacket::Execute()
{
  Account* account = GetAccount();

  ProjectGameManagementServerModule* module = GetModule();
  if (module != NULL && account != NULL)
  {
    LobbyGameData* lobbyGame = module->JoinLobbyGame(m_GameId, account->GetAccountId(), account->GetName(), m_RobotId);
    if (lobbyGame != NULL)
    {
      LobbyGamePlayer* lobbyPlayer = lobbyGame->GetPlayer(account->GetAccountId());
      if (lobbyPlayer != NULL)
      {
        // Send to all players in game
        SendPacketToLobbyGamePlayers(lobbyGame, new ClientPlayerJoinedLobbyGameNetworkPacket(lobbyGame->GetGameId(), lobbyPlayer));
        return;
      }
    }
    SendGameErrorToClient(module->GetLastError());
    return;
  }
  SendGameErrorToClient(GameEnginePacketData::ErrorCode_FailedToCreateGame);
}
