
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

  uint playfieldId = datastream->ReadUInt32();


  // Write players
  const std::vector<OnlineGamePlayer*> playerList = m_GameData->GetPlayerList();
  datastream->WriteUInt16(playfieldId.size());

  for (std::vector<OnlineGamePlayer*>::const_iterator itPlayer = playerList.begin(); itPlayer != playerList.end(); ++itPlayer)
  {
    OnlineGamePlayer* onlinePlayer = *itPlayer;

    datastream->WriteUInt32(onlinePlayer->GetAccountId());
    datastream->WriteString(onlinePlayer->GetPlayerName());
    datastream->WriteUInt16(onlinePlayer->GetRobotID());
    datastream->WriteUInt8(onlinePlayer->GetLives());
    datastream->WriteUInt16(onlinePlayer->GetPositionX());
    datastream->WriteUInt16(onlinePlayer->GetPositionY());
  }

  // Read players
  // - Id, name, position, lives
  // - Playerhand/card
  // Game status
  // - Hm think about this
  // Read playfield id

  return datastream;
}

void ClientReceivedGameDataNetworkPacket::Execute()
{
}
