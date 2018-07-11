/*************************************************************************************************
* Project     : EvilEngine Multi-platform 2014-2016
* File        : LoginAccountNetworkPacket
*
* Description : Network packet save playfield on the server
*
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "ServerLeaveQuickGameNetworkPacket.h"

#include "ProjectLobbyGameModule/Data/GameManagementPacketData.h"
#include "ProjectLobbyGameModule/NetworkPackets/ClientNetworkPackets/ClientLobbyGameListNetworkPacket.h"
#include "ProjectLobbyGameModule/Data/LobbyGameData.hpp"

ServerLeaveQuickGameNetworkPacket::ServerLeaveQuickGameNetworkPacket() :
  BaseGameManagementNetworkPacket(GameEnginePacketData::PacketData_ServerLeaveQuickGame)
{
}

ServerLeaveQuickGameNetworkPacket::ServerLeaveQuickGameNetworkPacket(const BinaryStream* datastream) :
  BaseGameManagementNetworkPacket(GameEnginePacketData::PacketData_ServerLeaveQuickGame, datastream)
{
}

BinaryStream* ServerLeaveQuickGameNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseGameManagementNetworkPacket::GetDataStream();
  return datastream;
}

void ServerLeaveQuickGameNetworkPacket::Execute()
{
  Account* account = GetAccount();
  ProjectLobbyGameServerModule* module = GetModule();
  if (module != NULL && account != NULL)
  {
    module->LeaveQuickGame( account->GetAccountId() );
  }
}
