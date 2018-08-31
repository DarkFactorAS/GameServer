
#include "PreCompile.h"
#include "ClientReceivedGameDataNetworkPacket.h"
#include "ProjectUnityBeerServer/ProjectModules/ProjectOnlineGameModule/Data/OnlineGamePacketData.h"

ClientReceivedGameDataNetworkPacket::ClientReceivedGameDataNetworkPacket(const OnlineGameData* gameData) :
  BaseNetworkPacket(OnlineGamePacketData::PacketData_ClientReceiveGameData),
  m_GameData(gameData)
{
}

ClientReceivedGameDataNetworkPacket::ClientReceivedGameDataNetworkPacket(const BinaryStream* /*datastream*/) :
  BaseNetworkPacket(OnlineGamePacketData::PacketData_ClientReceiveGameData)
{
}

BinaryStream* ClientReceivedGameDataNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseNetworkPacket::GetDataStream();
  datastream->WriteUInt32(m_GameData->GetGameId());
  datastream->WriteUInt32(m_GameData->GetPlayfieldId());
  datastream->WriteUInt32(m_GameData->GetOwnerAccountId());

  // Write players
  const std::vector<OnlineGamePlayer*> playerList = m_GameData->GetPlayerList();
  datastream->WriteUInt16((uint16)playerList.size());

  for (std::vector<OnlineGamePlayer*>::const_iterator itPlayer = playerList.begin(); itPlayer != playerList.end(); ++itPlayer)
  {
    OnlineGamePlayer* onlinePlayer = *itPlayer;

    datastream->WriteUInt32(onlinePlayer->GetAccountId());
    datastream->WriteString(onlinePlayer->GetPlayerName());
    datastream->WriteUInt32(onlinePlayer->GetRobotID());
    datastream->WriteUInt8(onlinePlayer->GetLives());
    datastream->WriteUInt16(onlinePlayer->GetPositionX());
    datastream->WriteUInt16(onlinePlayer->GetPositionY());
    datastream->WriteUInt32(onlinePlayer->GetSpawnPointId());
  }

  return datastream;
}

void ClientReceivedGameDataNetworkPacket::Execute()
{
}
