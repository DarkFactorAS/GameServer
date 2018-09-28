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
#include "GMServerProgressPlayerStatusNetworkPacket.h"

#include "ProjectOnlineGameModule/NetworkPackets/ClientNetworkPackets/ClientReceivedActionCardsNetworkPacket.h"
#include "ProjectOnlineGameModule/Data/OnlineGamePacketData.h"
#include "ProjectOnlineGameModule/Data/ActionCard.hpp"

#include "ProjectLobbyGameModule/Data/GameManagementPacketData.h"

GMServerProgressPlayerStatusNetworkPacket::GMServerProgressPlayerStatusNetworkPacket(uint32 onlineGameId, uint32 thisStatus) :
  ServerBaseOnlineGameNetworkPacket(OnlineGamePacketData::PacketData_ServerGMProgressPlayerStatus),
  m_GameId(onlineGameId),
  m_StatusId(thisStatus)
{
}

GMServerProgressPlayerStatusNetworkPacket::GMServerProgressPlayerStatusNetworkPacket(const BinaryStream* datastream) :
  ServerBaseOnlineGameNetworkPacket(OnlineGamePacketData::PacketData_ServerGMProgressPlayerStatus, datastream)
{
  m_GameId    = datastream->ReadUInt32();
  m_StatusId  = datastream->ReadUInt32();
}

BinaryStream* GMServerProgressPlayerStatusNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = ServerBaseOnlineGameNetworkPacket::GetDataStream();
  datastream->WriteUInt32(m_GameId);
  datastream->WriteUInt32(m_StatusId);
  return datastream;
}

void GMServerProgressPlayerStatusNetworkPacket::Execute()
{
  Account* account = GetAccount();
  if (account == NULL || !account->HasServerGMFlag())
  {
    return;
  }

  ProjectOnlineGameServerModule* module = GetModule();
  if (module != NULL  )
  {
    module->GMProgressPlayerStatus(m_GameId, m_StatusId);
  }
}
