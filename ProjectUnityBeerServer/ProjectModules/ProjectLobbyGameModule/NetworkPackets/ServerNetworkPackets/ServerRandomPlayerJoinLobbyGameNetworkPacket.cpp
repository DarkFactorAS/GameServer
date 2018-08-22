/*************************************************************************************************
* Project     : EvilEngine Multi-platform 2014-2016
* File        : LoginAccountNetworkPacket
*
* Description : Network packet save playfield on the server
*
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "ServerRandomPlayerJoinLobbyGameNetworkPacket.h"

#include "ProjectLobbyGameModule/Data/GameManagementPacketData.h"
#include "ProjectLobbyGameModule/NetworkPackets/ClientNetworkPackets/ClientPlayerJoinedLobbyGameNetworkPacket.h"
#include "ProjectLobbyGameModule/Data/LobbyGameData.hpp"

#include "EvilGameEngine/CoreGameLogin/LoginModule/CoreGameServerLoginModule.h"

ServerRandomPlayerJoinLobbyGameNetworkPacket::ServerRandomPlayerJoinLobbyGameNetworkPacket(uint32 gameId, uint32 robotId) :
  BaseGameManagementNetworkPacket(GameEnginePacketData::PacketData_ServerRandomJoinLobbyGame),
  m_GameId(gameId),
  m_RobotId(robotId)
{
}

ServerRandomPlayerJoinLobbyGameNetworkPacket::ServerRandomPlayerJoinLobbyGameNetworkPacket(const BinaryStream* datastream) :
  BaseGameManagementNetworkPacket(GameEnginePacketData::PacketData_ServerRandomJoinLobbyGame, datastream)
{
  m_GameId = datastream->ReadUInt32();
  m_RobotId = datastream->ReadUInt32();
}

BinaryStream* ServerRandomPlayerJoinLobbyGameNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseGameManagementNetworkPacket::GetDataStream();
  datastream->WriteUInt32(m_GameId);
  return datastream;
}

void ServerRandomPlayerJoinLobbyGameNetworkPacket::Execute()
{
  Account* account = GetAccount();

  ProjectLobbyGameServerModule* module = GetModule();
  if (module != NULL && account != NULL /*&& account->IsServerGMEnabled()*/)
  {
    CoreGameServerLoginModule* loginModule = module->GetLoginModule();
    if (loginModule != NULL)
    {
      Account* rndAccount = loginModule->GetRandomAccount( account->GetAccountId() ); 
      if (rndAccount != NULL)
      {
        uint32 hardcodedRobotId = 1;
        LobbyGameData* lobbyGame = module->JoinLobbyGame(m_GameId, rndAccount->GetAccountId(), rndAccount->GetName(), hardcodedRobotId);
        if (lobbyGame != NULL)
        {
          LobbyGamePlayer* lobbyPlayer = lobbyGame->GetPlayer(rndAccount->GetAccountId());
          if (lobbyPlayer != NULL)
          {
            // Send to all players in game
            SendPacketToLobbyGamePlayers(lobbyGame, new ClientPlayerJoinedLobbyGameNetworkPacket(lobbyGame->GetGameId(), lobbyPlayer));
            return;
          }
        }
        else
        {
          SendGameErrorToClient(module->GetLastError());
        }
      }
    }
  }
  SendGameErrorToClient(GameEnginePacketData::ErrorCode_FailedToCreateGame);
}
