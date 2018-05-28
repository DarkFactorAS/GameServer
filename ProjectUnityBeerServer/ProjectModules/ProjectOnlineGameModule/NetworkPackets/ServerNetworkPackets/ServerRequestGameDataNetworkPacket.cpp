/*************************************************************************************************
* Project     : EvilEngine Multi-platform 2014-2018
* File        : ServerRequestGameDataNetworkPacket
*
* Description : Network packet to request all data for an online game
*
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "ServerRequestGameDataNetworkPacket.h"

#include "ProjectModules/ProjectOnlineGameModule/Data/OnlineGamePacketData.h"
//#include "ProjectModules/ProjectOnlineGameModule/NetworkPackets/ClientNetworkPackets/ClientLobbyGameListNetworkPacket.h"

ServerRequestGameDataNetworkPacket::ServerRequestGameDataNetworkPacket(uint32 gameId) :
  BaseAuthentcatedNetworkPacket(OnlineGamePacketData::PacketData_ServerRequestGameData),
  m_GameId(gameId)
{
}

ServerRequestGameDataNetworkPacket::ServerRequestGameDataNetworkPacket(const BinaryStream* datastream) :
  BaseAuthentcatedNetworkPacket(OnlineGamePacketData::PacketData_ServerRequestGameData, datastream)
{
  m_GameId = datastream->ReadUInt32();
}

BinaryStream* ServerRequestGameDataNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseAuthentcatedNetworkPacket::GetDataStream();
  datastream->WriteUInt32(m_GameId);
  return datastream;
}

void ServerRequestGameDataNetworkPacket::Execute()
{
  //Account* account = GetAccount();
  //ProjectGameManagementServerModule* module = GetModule();
  //if (module != NULL && account != NULL)
  //{
  //  module->JoinQuickGame(account->GetAccountId());
  //}
  //else
  //{
  //  SendGameErrorToClient(OnlineGamePacketData2::ErrorCode_FailedToCreateGame);
  //}
}
