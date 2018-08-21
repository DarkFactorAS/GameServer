/*************************************************************************************************
* Project     : EvilEngine Multi-platform 2014-2016
* File        : LoginAccountNetworkPacket
*
* Description : Network packet save playfield on the server
*
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "ServerRequestActiveOnlineGameNetworkPacket.h"

#include "ProjectLobbyGameModule/Data/GameManagementPacketData.h"
#include "ProjectLobbyGameModule/Data/LobbyGameData.hpp"
#include "ProjectLobbyGameModule/NetworkPackets/ClientNetworkPackets/ClientActiveOnlineGameListNetworkPacket.h"
#include "ProjectOnlineGameModule/Module/ProjectOnlineGameServerModule.h"

ServerRequestActiveOnlineGameNetworkPacket::ServerRequestActiveOnlineGameNetworkPacket() :
  BaseGameManagementNetworkPacket(GameEnginePacketData::PacketData_ServerActiveGameList)
{
}

ServerRequestActiveOnlineGameNetworkPacket::ServerRequestActiveOnlineGameNetworkPacket(const BinaryStream* datastream) :
  BaseGameManagementNetworkPacket(GameEnginePacketData::PacketData_ServerActiveGameList, datastream)
{
}

void ServerRequestActiveOnlineGameNetworkPacket::Execute()
{
  Account* account = GetAccount();

  ProjectLobbyGameServerModule* lobbyModule = GetModule();
  if (lobbyModule != NULL && account != NULL)
  {
    ProjectOnlineGameServerModule* onlineGameModule = lobbyModule->GetOnlineGameModule();
    if (onlineGameModule != NULL)
    {
      std::vector< uint32 > gameList = onlineGameModule->GetOnlineGamesWithAccount(account->GetAccountId());
      SendPacketToClient(new ClientActiveOnlineGameListNetworkPacket(gameList));
    }
  }
  else
  {
    SendGameErrorToClient(GameEnginePacketData::ErrorCode_FailedToCreateGame);
  }
}
