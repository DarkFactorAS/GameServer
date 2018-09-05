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
#include "ProjectModules/ProjectOnlineGameModule/NetworkPackets/ClientNetworkPackets/ClientReceivedGameDataNetworkPacket.h"
#include "ProjectModules/ProjectOnlineGameModule/Module/ProjectOnlineGameServerModule.h"

ServerRequestGameDataNetworkPacket::ServerRequestGameDataNetworkPacket(uint32 gameId) :
  ServerBaseOnlineGameNetworkPacket(OnlineGamePacketData::PacketData_ServerCreateOnlineGame),
  m_GameId(gameId)
{
}

ServerRequestGameDataNetworkPacket::ServerRequestGameDataNetworkPacket(const BinaryStream* datastream) :
  ServerBaseOnlineGameNetworkPacket(OnlineGamePacketData::PacketData_ServerCreateOnlineGame, datastream)
{
  m_GameId = datastream->ReadUInt32();
}

BinaryStream* ServerRequestGameDataNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = ServerBaseOnlineGameNetworkPacket::GetDataStream();
  datastream->WriteUInt32(m_GameId);
  return datastream;
}

void ServerRequestGameDataNetworkPacket::Execute()
{
  Account* account = GetAccount();
  ProjectOnlineGameServerModule* module = GetModule();
  if (module != NULL && account != NULL ) 
  {
    OnlineGameData* onlineGame = module->GetOnlineGameWithAccount(m_GameId, account->GetAccountId());
    if (onlineGame != NULL)
    {
      SendPacketToClient( new ClientReceivedGameDataNetworkPacket( onlineGame ) );
    }
  }
  //else
  //{
  //  SendGameErrorToClient(OnlineGamePacketData2::ErrorCode_FailedToCreateGame);
  //}
}
