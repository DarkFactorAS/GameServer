/*************************************************************************************************
* Project     : EvilEngine Multi-platform 2014-2018
* File        : ClientCreatedGameNetworkPacket
*
* Description : Network packet to receive a created lobbygame
*
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "ClientLobbyGameListNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/Network/BaseAuthentcatedNetworkPacket.h"

#include "ProjectGameManagementModule/Data/GameManagementPacketData.h"
#include "ProjectGameManagementModule/Data/LobbyGameData.hpp"
#include "ProjectGameManagementModule/Data/LobbyGamePlayer.hpp"
#include "ProjectGameManagementModule/Logic/GameManagementModule/ProjectGameManagementClientModule.h"

#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/Data/Playfield.h"

ClientLobbyGameListNetworkPacket::ClientLobbyGameListNetworkPacket(std::vector<LobbyGameData*> lobbyGameList) :
  BaseNetworkPacket(GameEnginePacketData::PacketData_ClientGameList),
  m_LobbyGameList(lobbyGameList)
{
}

ClientLobbyGameListNetworkPacket::ClientLobbyGameListNetworkPacket(const BinaryStream* datastream) :
  BaseNetworkPacket(GameEnginePacketData::PacketData_ClientGameList, datastream)
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

BinaryStream* ClientLobbyGameListNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseNetworkPacket::GetDataStream();

  datastream->WriteUInt16((uint16)m_LobbyGameList.size());
  for (uint32 index = 0; index < m_LobbyGameList.size(); index++)
  {
    LobbyGameData* lobbyGame = m_LobbyGameList[index];

    datastream->WriteUInt32(lobbyGame->GetGameId());

    // Write playfield header
    Playfield* playfield = lobbyGame->GetPlayfield();
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
    const std::vector<LobbyGamePlayer*> playerList = lobbyGame->GetPlayerList();
    datastream->WriteUInt16((uint16)playerList.size());
    for (uint32 pIndex = 0; pIndex < playerList.size(); pIndex++)
    {
      const LobbyGamePlayer* player = playerList[pIndex];
      datastream->WriteUInt32(player->GetAccountId());
      datastream->WriteString(player->GetPlayerName());
      datastream->WriteUInt32(player->GetRobotId());
    }

  }

  return datastream;
}

void ClientLobbyGameListNetworkPacket::Execute()
{
  //ProjectGameManagementClientModule* module = safe_cast<ProjectGameManagementClientModule*> (GetEngineModule(ProjectGameManagementClientModule::PROJECT_MODULETYPE_CLIENTGAMEMANAGEMENT));
  //if (module != NULL)
  //{
  //  module->CreatedGame(m_LobbyGame);
  //}
}
