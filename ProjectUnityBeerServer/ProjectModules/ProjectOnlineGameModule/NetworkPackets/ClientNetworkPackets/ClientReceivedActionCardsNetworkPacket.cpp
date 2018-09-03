
#include "PreCompile.h"
#include "ClientReceivedActionCardsNetworkPacket.h"
#include "ProjectUnityBeerServer/ProjectModules/ProjectOnlineGameModule/Data/OnlineGamePacketData.h"

ClientReceivedActionCardsNetworkPacket::ClientReceivedActionCardsNetworkPacket(uint32 gameId,std::vector<uint32> actionCardList) :
  BaseNetworkPacket(OnlineGamePacketData::PacketData_ClientReceiveActionCards),
  m_GameId(gameId),
  m_ActionCardList(actionCardList)
{
}

ClientReceivedActionCardsNetworkPacket::ClientReceivedActionCardsNetworkPacket(const BinaryStream* /*datastream*/) :
  BaseNetworkPacket(OnlineGamePacketData::PacketData_ClientReceiveActionCards)
{
}

BinaryStream* ClientReceivedActionCardsNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseNetworkPacket::GetDataStream();
  datastream->WriteUInt32(m_GameId);

  uint16 numCards = (uint16) m_ActionCardList.size();
  datastream->WriteUInt16(numCards);
  for (uint16 index = 0; index < numCards; index++)
  {
    datastream->WriteUInt32( m_ActionCardList[index] );
  }

  return datastream;
}

void ClientReceivedActionCardsNetworkPacket::Execute()
{
}
