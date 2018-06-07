
#include "PreCompile.h"
#include "ClientReceivedGameDataNetworkPacket.h"
#include "ProjectUnityBeerServer/ProjectModules/ProjectOnlineGameModule/Data/OnlineGamePacketData.h"

ClientReceivedGameDataNetworkPacket::ClientReceivedGameDataNetworkPacket(const OnlineGameData* gameData) :
  BaseAuthentcatedNetworkPacket(OnlineGamePacketData::PacketData_ClientReceiveGameData),
  m_GameData(gameData)
{
}

ClientReceivedGameDataNetworkPacket::ClientReceivedGameDataNetworkPacket(const BinaryStream* /*datastream*/) :
  BaseAuthentcatedNetworkPacket(OnlineGamePacketData::PacketData_ClientReceiveGameData)
{
}

BinaryStream* ClientReceivedGameDataNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseNetworkPacket::GetDataStream();
  datastream->WriteUInt32(m_GameData->GetGameId());

  // Write players
  const std::vector<OnlineGamePlayer*> playerList = m_GameData->GetPlayerList();
  for (std::vector<OnlineGamePlayer*>::const_iterator itPlayer = playerList.begin(); itPlayer != playerList.end(); ++itPlayer)
  {
    OnlineGamePlayer* onlinePlayer = *itPlayer;

    datastream->WriteUInt32(onlinePlayer->GetAccountId());

    //datastream->WriteString(onlinePlayer->GetName());
    //datastream->WriteUInt16(onlinePlayer->GetBoardPositionX());
    //datastream->WriteUInt16(onlinePlayer->GetBoardPositionY());
    //datastream->WriteUInt16(onlinePlayer->GetLives());
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
