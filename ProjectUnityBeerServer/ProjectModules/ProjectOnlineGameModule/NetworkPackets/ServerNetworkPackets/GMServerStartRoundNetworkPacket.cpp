/*************************************************************************************************
* Project     : EvilEngine Multi-platform 2014-2019
* File        : ServerPlayerStartRoundNetworkPacket
*
* Description : Client notifies the server that the player is ready to start round and receive
*               new cards to place.
*
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "GMServerStartRoundNetworkPacket.h"

#include "ProjectOnlineGameModule/NetworkPackets/ClientNetworkPackets/ClientReceivedActionCardsNetworkPacket.h"
#include "ProjectOnlineGameModule/Data/OnlineGamePacketData.h"
#include "ProjectOnlineGameModule/Data/ActionCard.hpp"

#include "ProjectLobbyGameModule/Data/GameManagementPacketData.h"

GMServerStartRoundNetworkPacket::GMServerStartRoundNetworkPacket(uint32 lobbyGameId) :
  ServerBaseOnlineGameNetworkPacket(OnlineGamePacketData::PacketData_ServerGMStartRound),
  m_GameId(lobbyGameId)
{
}

GMServerStartRoundNetworkPacket::GMServerStartRoundNetworkPacket(const BinaryStream* datastream) :
  ServerBaseOnlineGameNetworkPacket(OnlineGamePacketData::PacketData_ServerGMStartRound, datastream)
{
  m_GameId    = datastream->ReadUInt32();
}

BinaryStream* GMServerStartRoundNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = ServerBaseOnlineGameNetworkPacket::GetDataStream();
  datastream->WriteUInt32(m_GameId);
  return datastream;
}

void GMServerStartRoundNetworkPacket::Execute()
{
  Account* account = GetAccount();
  if (account == NULL || !account->HasServerGMFlag())
  {
    return;
  }

  ProjectOnlineGameServerModule* module = GetModule();
  if (module != NULL  )
  {
    module->SetAllPlayersReady(m_GameId);
    module->DealActionCards(m_GameId);
  }
}
