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

#include "ProjectOnlineGameModule/NetworkPackets/ClientNetworkPackets/ClientReceivedActionCardsNetworkPacket.h"
#include "ProjectOnlineGameModule/Data/OnlineGamePacketData.h"
#include "ProjectOnlineGameModule/Data/ActionCard.hpp"

#include "ProjectLobbyGameModule/Data/GameManagementPacketData.h"

ServerPlayerStartRoundNetworkPacket::ServerPlayerStartRoundNetworkPacket(uint32 lobbyGameId, uint32 accountId) :
  ServerBaseOnlineGameNetworkPacket(OnlineGamePacketData::PacketData_ServerPlayerStartRound),
  m_GameId(lobbyGameId),
  m_AccountId(accountId)
{
}

ServerPlayerStartRoundNetworkPacket::ServerPlayerStartRoundNetworkPacket(const BinaryStream* datastream) :
  ServerBaseOnlineGameNetworkPacket(OnlineGamePacketData::PacketData_ServerPlayerStartRound, datastream)
{
  m_GameId    = datastream->ReadUInt32();
  m_AccountId = datastream->ReadUInt32();
}

BinaryStream* ServerPlayerStartRoundNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = ServerBaseOnlineGameNetworkPacket::GetDataStream();
  datastream->WriteUInt32(m_GameId);
  datastream->WriteUInt32(m_AccountId);
  return datastream;
}

void ServerPlayerStartRoundNetworkPacket::Execute()
{
  if (!CanExecuteAsAccount(m_AccountId))
  {
    return;
  }

  Account* account = GetAccount();
  ProjectOnlineGameServerModule* module = GetModule();

  if (module != NULL && account != NULL)
  {
    // TODO : Flag player as ready.
    //        If all players are ready - broadcast cards to all
    //        Start timer after first enters this state ?

    std::vector<ActionCard*> actionCardList = module->GetActionCards(m_GameId, m_AccountId);
    SendPacketToClient( new ClientReceivedActionCardsNetworkPacket(m_GameId, m_AccountId, actionCardList) );
  }

  //SendGameErrorToClient(GameEnginePacketData::ErrorCode_FailedToCreateGame);
}
