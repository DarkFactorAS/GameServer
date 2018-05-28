/*************************************************************************************************
* Project     : EvilEngine Multi-platform 2014-2018
* File        : ClientCreatedGameNetworkPacket
*
* Description : Network packet to receive a created lobbygame
*
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "ClientCreatedQuickGameNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/Network/BaseAuthentcatedNetworkPacket.h"

#include "ProjectGameManagementModule/Data/GameManagementPacketData.h"
#include "ProjectGameManagementModule/Data/LobbyGameData.hpp"
#include "ProjectGameManagementModule/Data/LobbyGamePlayer.hpp"
#include "ProjectGameManagementModule/Logic/GameManagementModule/ProjectGameManagementClientModule.h"

#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/Data/Playfield.h"

ClientCreatedQuickGameNetworkPacket::ClientCreatedQuickGameNetworkPacket(uint32 gameId) :
  BaseNetworkPacket(GameEnginePacketData::PacketData_ClientCreatedQuickGame),
  m_GameId(gameId)
{
}

ClientCreatedQuickGameNetworkPacket::ClientCreatedQuickGameNetworkPacket(const BinaryStream* datastream) :
  BaseNetworkPacket(GameEnginePacketData::PacketData_ClientCreatedQuickGame, datastream)
{
  m_GameId = datastream->ReadUInt32();
}

BinaryStream* ClientCreatedQuickGameNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseNetworkPacket::GetDataStream();
  datastream->WriteUInt32(m_GameId);

  return datastream;
}

void ClientCreatedQuickGameNetworkPacket::Execute()
{
  //ProjectGameManagementClientModule* module = safe_cast<ProjectGameManagementClientModule*> (GetEngineModule(ProjectGameManagementClientModule::PROJECT_MODULETYPE_CLIENTGAMEMANAGEMENT));
  //if (module != NULL)
  //{
  //  module->Create(m_LobbyGame);
  //}
}
