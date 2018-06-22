
#include "PreCompile.h"
#include "ClientCreatedOnlineGameNetworkPacket.h"
#include "ProjectUnityBeerServer/ProjectModules/ProjectOnlineGameModule/Data/OnlineGamePacketData.h"

ClientCreatedOnlineGameNetworkPacket::ClientCreatedOnlineGameNetworkPacket(uint32 lobbyGameId,uint32 onlineGameId) :
  BaseNetworkPacket(OnlineGamePacketData::PacketData_ClientCreatedOnlineGame),
  m_LobbyGameId(lobbyGameId),
  m_OnlineGameId(onlineGameId)
{
}

ClientCreatedOnlineGameNetworkPacket::ClientCreatedOnlineGameNetworkPacket(const BinaryStream* /*datastream*/) :
  BaseNetworkPacket(OnlineGamePacketData::PacketData_ClientCreatedOnlineGame)
{
}

BinaryStream* ClientCreatedOnlineGameNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseNetworkPacket::GetDataStream();
  datastream->WriteUInt32(m_LobbyGameId);
  datastream->WriteUInt32(m_OnlineGameId);
  return datastream;
}

void ClientCreatedOnlineGameNetworkPacket::Execute()
{
}
