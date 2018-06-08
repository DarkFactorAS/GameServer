/*************************************************************************************************
* Project     : EvilEngine Multi-platform 2014-2018
* File        : ClientCreatedGameNetworkPacket
*
* Description : Network packet to receive a created lobbygame
*
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "ClientCreatedLobbyGameNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/Network/BaseAuthentcatedNetworkPacket.h"

#include "ProjectLobbyGameModule/Data/GameManagementPacketData.h"
#include "ProjectLobbyGameModule/Data/LobbyGameData.hpp"
#include "ProjectLobbyGameModule/Data/LobbyGamePlayer.hpp"
#include "ProjectLobbyGameModule/Logic/GameManagementModule/ProjectGameManagementClientModule.h"

#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/Data/Playfield.h"

ClientCreatedLobbyGameNetworkPacket::ClientCreatedLobbyGameNetworkPacket(LobbyGameData* lobbyGame) :
  BaseNetworkPacket(GameEnginePacketData::PacketData_ClientCreatedLobbyGame),
  m_LobbyGame(lobbyGame)
{
}

ClientCreatedLobbyGameNetworkPacket::ClientCreatedLobbyGameNetworkPacket(const BinaryStream* datastream) :
  BaseNetworkPacket(GameEnginePacketData::PacketData_ClientCreatedLobbyGame, datastream)
{
  uint32 gameId = datastream->ReadUInt32();
  m_LobbyGame = new LobbyGameData(gameId);

  Playfield* playfield = Playfield::CreatePlayfieldFromStream(datastream);
  m_LobbyGame->SetPlayfield(playfield);

  uint16 numPlayers = datastream->ReadUInt16();
  for (uint16 index = 0; index < numPlayers; index++)
  {
    uint32 playerId = datastream->ReadUInt32();
    String playerName = datastream->ReadString();
    uint32 robotId = datastream->ReadUInt32();

    m_LobbyGame->AddPlayer( new LobbyGamePlayer(playerId, playerName, robotId) );
  }

}

BinaryStream* ClientCreatedLobbyGameNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseNetworkPacket::GetDataStream();
  datastream->WriteUInt32(m_LobbyGame->GetGameId());

  // Write playfield header
  Playfield* playfield = m_LobbyGame->GetPlayfield();
  if (playfield != NULL)
  {
    playfield->CreateBinaryStream(datastream, false);
  }
  else
  {
    delete datastream;
    return NULL;
  }

  // Write players
  const std::vector<LobbyGamePlayer*> playerList = m_LobbyGame->GetPlayerList();
  datastream->WriteUInt16((uint16)playerList.size());
  for (uint32 index = 0; index < playerList.size(); index++)
  {
    const LobbyGamePlayer* player = playerList[index];
    datastream->WriteUInt32( player->GetAccountId() );
    datastream->WriteString( player->GetPlayerName() );
    datastream->WriteUInt32( player->GetRobotId() );
  }

  return datastream;
}

void ClientCreatedLobbyGameNetworkPacket::Execute()
{
  ProjectGameManagementClientModule* module = safe_cast<ProjectGameManagementClientModule*> (GetEngineModule(ProjectGameManagementClientModule::PROJECT_MODULETYPE_CLIENTGAMEMANAGEMENT));
  if (module != NULL)
  {
    module->CreatedGame(m_LobbyGame);
  }
}
