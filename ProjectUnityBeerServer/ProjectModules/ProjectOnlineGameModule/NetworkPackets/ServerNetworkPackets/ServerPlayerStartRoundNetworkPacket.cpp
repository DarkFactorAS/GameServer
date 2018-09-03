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
#include "ServerPlayerStartRoundNetworkPacket.h"

#include "ProjectLobbyGameModule/Data/GameManagementPacketData.h"
#include "ProjectOnlineGameModule/NetworkPackets/ClientNetworkPackets/ClientReceivedActionCardsNetworkPacket.h"

#include "ProjectOnlineGameModule/Data/OnlineGamePacketData.h"

ServerPlayerStartRoundNetworkPacket::ServerPlayerStartRoundNetworkPacket(uint32 lobbyGameId) :
  ServerBaseOnlineGameNetworkPacket(OnlineGamePacketData::PacketData_ServerPlayerStartRound),
  m_GameId(lobbyGameId)
{
}

ServerPlayerStartRoundNetworkPacket::ServerPlayerStartRoundNetworkPacket(const BinaryStream* datastream) :
  ServerBaseOnlineGameNetworkPacket(OnlineGamePacketData::PacketData_ServerPlayerStartRound, datastream)
{
  m_GameId = datastream->ReadUInt32();
}

BinaryStream* ServerPlayerStartRoundNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = ServerBaseOnlineGameNetworkPacket::GetDataStream();
  datastream->WriteUInt32(m_GameId);
  return datastream;
}

void ServerPlayerStartRoundNetworkPacket::Execute()
{
  Account* account = GetAccount();
  ProjectOnlineGameServerModule* module = GetModule();

  if (module != NULL && account != NULL)
  {
    // TODO : Flag player as ready.
    //        If all players are ready - broadcast cards to all
    //        Start timer after first enters this state ?

    std::vector<uint32> actionCardList;
    actionCardList.push_back(1);
    actionCardList.push_back(1);
    actionCardList.push_back(1);
    actionCardList.push_back(1);
    actionCardList.push_back(1);
    actionCardList.push_back(1);
    actionCardList.push_back(1);

    SendPacketToClient( new ClientReceivedActionCardsNetworkPacket(m_GameId, actionCardList) );
  }

  //SendGameErrorToClient(GameEnginePacketData::ErrorCode_FailedToCreateGame);
}
