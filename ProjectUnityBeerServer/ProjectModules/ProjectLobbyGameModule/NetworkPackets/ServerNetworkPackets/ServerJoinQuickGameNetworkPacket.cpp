/*************************************************************************************************
* Project     : EvilEngine Multi-platform 2014-2016
* File        : LoginAccountNetworkPacket
*
* Description : Network packet save playfield on the server
*
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "ServerJoinQuickGameNetworkPacket.h"

#include "ProjectLobbyGameModule/Data/GameManagementPacketData.h"
#include "ProjectLobbyGameModule/NetworkPackets/ClientNetworkPackets/ClientLobbyGameListNetworkPacket.h"
#include "ProjectLobbyGameModule/Data/LobbyGameData.hpp"

ServerJoinQuickGameNetworkPacket::ServerJoinQuickGameNetworkPacket() :
  BaseGameManagementNetworkPacket(GameEnginePacketData::PacketData_ServerJoinQuickGame)
{
}

ServerJoinQuickGameNetworkPacket::ServerJoinQuickGameNetworkPacket(const BinaryStream* datastream) :
  BaseGameManagementNetworkPacket(GameEnginePacketData::PacketData_ServerJoinQuickGame, datastream)
{
}

BinaryStream* ServerJoinQuickGameNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseGameManagementNetworkPacket::GetDataStream();
  return datastream;
}

void ServerJoinQuickGameNetworkPacket::Execute()
{
  Account* account = GetAccount();
  ProjectGameManagementServerModule* module = GetModule();
  if (module != NULL && account != NULL)
  {
    module->JoinQuickGame( account->GetAccountId() );
  }
  else
  {
    SendGameErrorToClient(GameEnginePacketData::ErrorCode_FailedToCreateGame);
  }
}
