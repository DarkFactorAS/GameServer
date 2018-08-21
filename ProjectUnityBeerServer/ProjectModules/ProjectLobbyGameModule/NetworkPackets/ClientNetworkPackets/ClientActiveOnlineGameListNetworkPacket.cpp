/*************************************************************************************************
* Project     : EvilEngine Multi-platform 2014-2018
* File        : ClientCreatedGameNetworkPacket
*
* Description : Network packet to receive a created lobbygame
*
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "ClientActiveOnlineGameListNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/Network/BaseAuthentcatedNetworkPacket.h"

#include "ProjectLobbyGameModule/Data/GameManagementPacketData.h"
#include "ProjectLobbyGameModule/Data/LobbyGameData.hpp"
#include "ProjectLobbyGameModule/Data/LobbyGamePlayer.hpp"
#include "ProjectLobbyGameModule/Module/ProjectLobbyGameClientModule.h"

#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/Data/Playfield.h"

ClientActiveOnlineGameListNetworkPacket::ClientActiveOnlineGameListNetworkPacket(std::vector<uint32> onlineGameList) :
  BaseNetworkPacket(GameEnginePacketData::PacketData_ClientActiveGameList),
  m_OnlineGameList(onlineGameList)
{
}

ClientActiveOnlineGameListNetworkPacket::ClientActiveOnlineGameListNetworkPacket(const BinaryStream* datastream) :
  BaseNetworkPacket(GameEnginePacketData::PacketData_ClientActiveGameList, datastream)
{
}

BinaryStream* ClientActiveOnlineGameListNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseNetworkPacket::GetDataStream();

  datastream->WriteUInt16((uint16)m_OnlineGameList.size());
  for (uint32 index = 0; index < m_OnlineGameList.size(); index++)
  {
    uint32 gameId = m_OnlineGameList[index];
    datastream->WriteUInt32(gameId);
  }

  return datastream;
}

void ClientActiveOnlineGameListNetworkPacket::Execute()
{
  //ProjectGameManagementClientModule* module = safe_cast<ProjectGameManagementClientModule*> (GetEngineModule(ProjectGameManagementClientModule::PROJECT_MODULETYPE_CLIENTGAMEMANAGEMENT));
  //if (module != NULL)
  //{
  //  module->CreatedGame(m_LobbyGame);
  //}
}
