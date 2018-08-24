/*************************************************************************************************
* Project     : EvilEngine Multi-platform 2014-2018
* File        : ServerRequestGameDataNetworkPacket
*
* Description : Network packet to request all data for an online game
*
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "ServerPlayerLeaveOnlineGameNetworkPacket.h"

#include "ProjectModules/ProjectOnlineGameModule/Data/OnlineGamePacketData.h"
#include "ProjectModules/ProjectOnlineGameModule/NetworkPackets/ClientNetworkPackets/ClientReceivedGameDataNetworkPacket.h"
#include "ProjectModules/ProjectOnlineGameModule/Module/ProjectOnlineGameServerModule.h"

ServerPlayerLeaveOnlineGameNetworkPacket::ServerPlayerLeaveOnlineGameNetworkPacket(uint32 gameId) :
  ServerBaseOnlineGameNetworkPacket(OnlineGamePacketData::PacketData_ServerCreateOnlineGame),
  m_GameId(gameId)
{
}

ServerPlayerLeaveOnlineGameNetworkPacket::ServerPlayerLeaveOnlineGameNetworkPacket(const BinaryStream* datastream) :
  ServerBaseOnlineGameNetworkPacket(OnlineGamePacketData::PacketData_ServerCreateOnlineGame, datastream)
{
  m_GameId = datastream->ReadUInt32();
}

BinaryStream* ServerPlayerLeaveOnlineGameNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = ServerBaseOnlineGameNetworkPacket::GetDataStream();
  datastream->WriteUInt32(m_GameId);
  return datastream;
}

void ServerPlayerLeaveOnlineGameNetworkPacket::Execute()
{
  Account* account = GetAccount();
  ProjectOnlineGameServerModule* module = GetModule();
  if (module != NULL && account != NULL ) 
  {
    if (module->LeaveOnlineGame(m_GameId, account->GetAccountId()))
    {
      // TODO > Check if game is desolved now
      // SendPacketToOnlineGamePlayers()

      // TODO : Notifuy original client 
      // SendPacketToClient( new )
    }
  }
}
