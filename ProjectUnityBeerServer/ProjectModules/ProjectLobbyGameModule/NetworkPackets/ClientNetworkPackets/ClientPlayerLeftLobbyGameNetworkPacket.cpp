/*************************************************************************************************
* Project     : EvilEngine Multi-platform 2014-2018
* File        : ClientJoinedLobbyGameNetworkPacket
*
* Description : Network packet that a player joined a lobby game
*
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "ClientPlayerLeftLobbyGameNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/Network/BaseAuthentcatedNetworkPacket.h"

#include "ProjectLobbyGameModule/Data/GameManagementPacketData.h"
#include "ProjectLobbyGameModule/Data/LobbyGameData.hpp"
#include "ProjectLobbyGameModule/Data/LobbyGamePlayer.hpp"
#include "ProjectLobbyGameModule/Module/ProjectClientLobbyGameModule.h"

#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/Data/Playfield.h"

ClientPlayerLeftLobbyGameNetworkPacket::ClientPlayerLeftLobbyGameNetworkPacket(uint32 gameId, LobbyGamePlayer* lobbyPlayer) :
  BaseNetworkPacket(GameEnginePacketData::PacketData_ClientPlayerLeftLobbyGame),
  m_GameId(gameId)
{
  m_LobbyPlayer = new LobbyGamePlayer(lobbyPlayer);
}

ClientPlayerLeftLobbyGameNetworkPacket::ClientPlayerLeftLobbyGameNetworkPacket(const BinaryStream* datastream) :
  BaseNetworkPacket(GameEnginePacketData::PacketData_ClientPlayerLeftLobbyGame, datastream)
{
  //uint32 gameId = datastream->ReadUInt32();
  //m_LobbyGame = new LobbyGameData(gameId);

  //Playfield* playfield = Playfield::CreatePlayfieldFromStream(datastream);
  //m_LobbyGame->SetPlayfield(playfield);

  //uint16 numPlayers = datastream->ReadUInt16();
  //for (uint16 index = 0; index < numPlayers; index++)
  //{
  //  uint32 playerId = datastream->ReadUInt32();
  //  String playerName = datastream->ReadString();
  //  uint32 robotId = datastream->ReadUInt32();

  //  m_LobbyGame->AddPlayer( new LobbyGamePlayer(playerId, playerName, robotId) );
  //}

}

ClientPlayerLeftLobbyGameNetworkPacket::~ClientPlayerLeftLobbyGameNetworkPacket()
{
  delete m_LobbyPlayer;
  m_LobbyPlayer = NULL;
}

BinaryStream* ClientPlayerLeftLobbyGameNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseNetworkPacket::GetDataStream();

  datastream->WriteUInt32(m_GameId);
  datastream->WriteUInt32(m_LobbyPlayer->GetAccountId());
  datastream->WriteString(m_LobbyPlayer->GetPlayerName());
  datastream->WriteUInt32(m_LobbyPlayer->GetRobotId());

  return datastream;
}

void ClientPlayerLeftLobbyGameNetworkPacket::Execute()
{
  //ProjectGameManagementClientModule* module = safe_cast<ProjectGameManagementClientModule*> (GetEngineModule(ProjectGameManagementClientModule::PROJECT_MODULETYPE_CLIENTGAMEMANAGEMENT));
  //if (module != NULL)
  //{
  //  module->CreatedGame(m_LobbyGame);
  //}
}
