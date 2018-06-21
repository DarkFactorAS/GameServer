/*************************************************************************************************
* Project     : EvilEngine Multi-platform 2014-2019
* File        : ServerCreateOnlineGameNetworkPacket
*
* Description : Network packet save playfield on the server
*
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "ServerCreateOnlineGameNetworkPacket.h"

#include "ProjectLobbyGameModule/Data/GameManagementPacketData.h"
#include "ProjectLobbyGameModule/NetworkPackets/ClientNetworkPackets/ClientCreatedLobbyGameNetworkPacket.h"
#include "ProjectLobbyGameModule/Data/LobbyGameData.hpp"

#include "ProjectOnlineGameModule/Data/OnlineGamePacketData.h"

ServerCreateOnlineGameNetworkPacket::ServerCreateOnlineGameNetworkPacket(uint32 lobbyGameId) :
  BaseGameManagementNetworkPacket(OnlineGamePacketData::PacketData_ServerStartLobbyGame),
  m_LobbyGameId(lobbyGameId)
{
}

ServerCreateOnlineGameNetworkPacket::ServerCreateOnlineGameNetworkPacket(const BinaryStream* datastream) :
  BaseGameManagementNetworkPacket(GameEnginePacketData::PacketData_ServerStartLobbyGame, datastream)
{
  m_LobbyGameId = datastream->ReadUInt32();
}

BinaryStream* ServerCreateOnlineGameNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseGameManagementNetworkPacket::GetDataStream();
  datastream->WriteUInt32(m_LobbyGameId);
  return datastream;
}

ProjectOnlineGameServerModule* ServerCreateOnlineGameNetworkPacket::GetOnlineGameModule()
{
  CoreGameEngine* gameEngine = GetGameEngine();
  if (gameEngine != NULL)
  {
    return safe_cast<ProjectOnlineGameServerModule*> (gameEngine->GetEngineModule(ProjectOnlineGameServerModule::PROJECT_MODULETYPE_ONLINEGAME));
  }
  return NULL;
}


void ServerCreateOnlineGameNetworkPacket::Execute()
{
  //Account* account = GetAccount();

  //ProjectGameManagementServerModule* module = GetModule();
  //ProjectOnlineGameServerModule* onlineGameModule = GetOnlineGameModule();

  //if (onlineGameModule != NULL && account != NULL)
  //{
  //  onlineGameModule->CreateOnlineGame();
  //}

  SendGameErrorToClient(GameEnginePacketData::ErrorCode_FailedToCreateGame);
}
