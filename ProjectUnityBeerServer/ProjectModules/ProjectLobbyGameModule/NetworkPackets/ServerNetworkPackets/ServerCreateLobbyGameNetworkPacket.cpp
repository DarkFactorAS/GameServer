/*************************************************************************************************
* Project     : EvilEngine Multi-platform 2014-2016
* File        : LoginAccountNetworkPacket
*
* Description : Network packet save playfield on the server
*
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "ServerCreateLobbyGameNetworkPacket.h"

#include "ProjectLobbyGameModule/Data/GameManagementPacketData.h"
#include "ProjectLobbyGameModule/NetworkPackets/ClientNetworkPackets/ClientCreatedLobbyGameNetworkPacket.h"
#include "ProjectLobbyGameModule/Data/LobbyGameData.hpp"

ServerCreateLobbyGameNetworkPacket::ServerCreateLobbyGameNetworkPacket(uint32 playfieldId, uint32 robotId) :
  BaseGameManagementNetworkPacket(GameEnginePacketData::PacketData_ServerCreateLobbyGame),
  m_PlayfieldId(playfieldId),
  m_RobotId(robotId)
{
}

ServerCreateLobbyGameNetworkPacket::ServerCreateLobbyGameNetworkPacket(const BinaryStream* datastream) :
  BaseGameManagementNetworkPacket(GameEnginePacketData::PacketData_ServerCreateLobbyGame, datastream)
{
  m_PlayfieldId = datastream->ReadUInt32();
  m_RobotId = datastream->ReadUInt32();
}

BinaryStream* ServerCreateLobbyGameNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseGameManagementNetworkPacket::GetDataStream();
  datastream->WriteUInt32(m_PlayfieldId);
  datastream->WriteUInt32(m_RobotId);
  return datastream;
}

ProjectWorldBuilderServerModule* ServerCreateLobbyGameNetworkPacket::GetWorldBuilderModule()
{
  CoreGameEngine* gameEngine = GetGameEngine();
  if (gameEngine != NULL)
  {
    return safe_cast<ProjectWorldBuilderServerModule*> (gameEngine->GetEngineModule(ProjectWorldBuilderServerModule::PROJECT_MODULETYPE_WORLDBUILDER));
  }
  return NULL;
}


void ServerCreateLobbyGameNetworkPacket::Execute()
{
  Account* account = GetAccount();

  // TODO : move playfield into its own module
  ProjectLobbyGameServerModule* module = GetModule();
  ProjectWorldBuilderServerModule* worldbuilderModule = GetWorldBuilderModule();

  if (module != NULL && account != NULL && worldbuilderModule != NULL)
  {
    Playfield* playfield = worldbuilderModule->LoadPlayfield(account->GetAccountId(),m_PlayfieldId);
    LobbyGameData* newGame = module->CreateNewGame( account->GetAccountId(), account->GetName(), playfield, m_RobotId );
    if ( newGame != NULL )
    {
      SendPacketToClient(new ClientCreatedLobbyGameNetworkPacket(newGame) );
    }
    else
    {
      SendGameErrorToClient(module->GetLastError());
    }
    return;
  }
  SendGameErrorToClient(GameEnginePacketData::ErrorCode_FailedToCreateGame);
}
