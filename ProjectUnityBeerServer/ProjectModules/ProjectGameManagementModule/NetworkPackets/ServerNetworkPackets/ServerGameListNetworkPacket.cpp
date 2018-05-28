/*************************************************************************************************
* Project     : EvilEngine Multi-platform 2014-2016
* File        : LoginAccountNetworkPacket
*
* Description : Network packet save playfield on the server
*
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "ServerGameListNetworkPacket.h"

#include "ProjectGameManagementModule/Data/GameManagementPacketData.h"
#include "ProjectGameManagementModule/NetworkPackets/ClientNetworkPackets/ClientLobbyGameListNetworkPacket.h"
#include "ProjectGameManagementModule/Data/LobbyGameData.hpp"

ServerGameListNetworkPacket::ServerGameListNetworkPacket() :
  BaseGameManagementNetworkPacket(GameEnginePacketData::PacketData_ServerGameList)
{
}

ServerGameListNetworkPacket::ServerGameListNetworkPacket(const BinaryStream* datastream) :
  BaseGameManagementNetworkPacket(GameEnginePacketData::PacketData_ServerGameList, datastream)
{
}

BinaryStream* ServerGameListNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseGameManagementNetworkPacket::GetDataStream();
  return datastream;
}

void ServerGameListNetworkPacket::Execute()
{
  Account* account = GetAccount();
  ProjectGameManagementServerModule* module = GetModule();
  if (module != NULL && account != NULL)
  {
    std::vector< LobbyGameData* > gameList = module->GetOpenGameList();
    SendPacketToClient(new ClientLobbyGameListNetworkPacket(gameList) );
    return;
  }
  SendGameErrorToClient(GameEnginePacketData::ErrorCode_FailedToCreateGame);
}
