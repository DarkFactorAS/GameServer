/*************************************************************************************************
* Project     : EvilEngine Multi-platform 2014-2019
* File        : ServerCreateOnlineGameNetworkPacket
*
* Description : Network packet save playfield on the server
*
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "ServerCreateOnlineGameNetworkPacket.h"

#include "ProjectLobbyGameModule/Data/GameManagementPacketData.h"
#include "ProjectOnlineGameModule/NetworkPackets/ClientNetworkPackets/ClientCreatedOnlineGameNetworkPacket.h"

#include "ProjectOnlineGameModule/Data/OnlineGamePacketData.h"

ServerCreateOnlineGameNetworkPacket::ServerCreateOnlineGameNetworkPacket(uint32 lobbyGameId) :
  ServerBaseOnlineGameNetworkPacket(OnlineGamePacketData::PacketData_ServerCreateOnlineGame),
  m_LobbyGameId(lobbyGameId)
{
}

ServerCreateOnlineGameNetworkPacket::ServerCreateOnlineGameNetworkPacket(const BinaryStream* datastream) :
  ServerBaseOnlineGameNetworkPacket(OnlineGamePacketData::PacketData_ServerCreateOnlineGame, datastream)
{
  m_LobbyGameId = datastream->ReadUInt32();
}

BinaryStream* ServerCreateOnlineGameNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = ServerBaseOnlineGameNetworkPacket::GetDataStream();
  datastream->WriteUInt32(m_LobbyGameId);
  return datastream;
}

void ServerCreateOnlineGameNetworkPacket::Execute()
{
  Account* account = GetAccount();
  ProjectOnlineGameServerModule* module = GetModule();

  if (module != NULL && account != NULL)
  {
    OnlineGameData* newGame = module->CreateOnlineGame( account->GetAccountId(), m_LobbyGameId);
    if (newGame != NULL)
    {
      SendPacketToClient(new ClientCreatedOnlineGameNetworkPacket( m_LobbyGameId, newGame->GetGameId()));
      return;
    }
  }

  SendGameErrorToClient(GameEnginePacketData::ErrorCode_FailedToCreateGame);
}
