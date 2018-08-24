/*************************************************************************************************
* Project     : EvilEngine Multi-platform 2014-2018
* File        : ServerBaseOnlineGameNetworkPacket
*
* Description : Wrapper class to handle easy access functions for online game network packets
*
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "ServerBaseOnlineGameNetworkPacket.h"

#include "EvilGameEngine/CoreGameLogin/LoginModule/CoreGameServerLoginModule.h"

#include "ProjectModules/ProjectOnlineGameModule/Data/OnlineGamePacketData.h"
//#include "ProjectModules/ProjectOnlineGameModule/NetworkPackets/ClientNetworkPackets/ClientReceivedGameDataNetworkPacket.h"
#include "ProjectModules/ProjectOnlineGameModule/Module/ProjectOnlineGameServerModule.h"

ServerBaseOnlineGameNetworkPacket::ServerBaseOnlineGameNetworkPacket(OnlineGamePacketData::PacketData packetType, const BinaryStream* datastream) :
  BaseAuthentcatedNetworkPacket((uint32)packetType,datastream)
{
}

ServerBaseOnlineGameNetworkPacket::ServerBaseOnlineGameNetworkPacket(OnlineGamePacketData::PacketData packetType) :
  BaseAuthentcatedNetworkPacket((uint32)packetType)
{
}

void ServerBaseOnlineGameNetworkPacket::SendGameErrorToClient(OnlineGamePacketData::PacketError errorId)
{
  SendGameErrorToClient((uint32)errorId);
}

void ServerBaseOnlineGameNetworkPacket::SendGameErrorToClient(uint32 /*errorId*/)
{
  //CoreGameEngine* gameEngine = GetGameEngine();
  //if (gameEngine != NULL)
  //{
  //  LogInfoFMT("BaseGameManagementNetworkPacket", "SendGameErrorToClient::Received error => %d/%d", m_PacketType, errorCodeId);

  //  if (errorCodeId == GameEnginePacketData::ErrorCode_NoError)
  //  {
  //    errorCodeId = GameEnginePacketData::ErrorCode_CodeError;
  //  }

  //  String errorMessage = String::zero;
  //  ProjectGameManagementServerModule* module = GetModule();
  //  if (module != NULL)
  //  {
  //    errorMessage = module->GetGameError(errorCodeId);
  //  }

  //  gameEngine->SendPacketToEndpoint(m_ConnectionId, new ClientGameErrorNetworkPacket(m_PacketType, errorCodeId, errorMessage));
  //}
}

void ServerBaseOnlineGameNetworkPacket::SendPacketToOnlineGamePlayers(OnlineGameData* onlineGame, BaseNetworkPacket* packet)
{
  CoreGameEngine* gameEngine = GetGameEngine();
  if (gameEngine != NULL)
  {
    CoreGameServerLoginModule* loginModule = CoreGameServerLoginModule::GetModule(gameEngine);
    if (loginModule != NULL)
    {
      const std::vector<OnlineGamePlayer*> playerList = onlineGame->GetPlayerList();
      for (std::vector<OnlineGamePlayer*>::const_iterator itPlayer = playerList.begin(); itPlayer != playerList.end(); ++itPlayer)
      {
        OnlineGamePlayer* lobbyPlayer = *itPlayer;

        Account* account = loginModule->GetCachedAccount(lobbyPlayer->GetAccountId());
        if (account != NULL)
        {
          gameEngine->SendPacketToEndpoint(account->GetConnectionId(), packet);
        }
      }
      return;
    }
    gameEngine->SendPacketToEndpoint(m_ConnectionId, packet);
    return;
  }
}

ProjectOnlineGameServerModule* ServerBaseOnlineGameNetworkPacket::GetModule()
{
  return ProjectOnlineGameServerModule::GetModule(GetGameEngine());
}
