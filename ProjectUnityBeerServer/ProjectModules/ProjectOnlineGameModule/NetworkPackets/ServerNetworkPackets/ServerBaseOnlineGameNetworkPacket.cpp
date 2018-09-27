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
#include "ProjectModules/ProjectOnlineGameModule/Module/ProjectOnlineGameServerModule.h"

#include "ProjectModules/ProjectOnlineGameModule/Data/OnlineGameData.hpp"
#include "ProjectModules/ProjectOnlineGameModule/Data/OnlineGamePlayer.hpp"

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

ProjectOnlineGameServerModule* ServerBaseOnlineGameNetworkPacket::GetModule()
{
  return ProjectOnlineGameServerModule::GetModule(GetGameEngine());
}

void ServerBaseOnlineGameNetworkPacket::SendPacketToOnlineGamePlayers(OnlineGameData* onlineGame, BaseNetworkPacket* packet)
{
  ProjectOnlineGameServerModule* module = GetModule();
  if (module != NULL)
  {
    module->SendPacketToOnlineGamePlayers(onlineGame, packet);
  }
}

bool ServerBaseOnlineGameNetworkPacket::CanExecuteAsAccount( uint32 accountId )
{
  Account* loggedInAccount = GetAccount();
  if (loggedInAccount == NULL)
  {
    return false;
  }

  if (loggedInAccount->GetAccountId() == accountId)
  {
    return true;
  }

  if (loggedInAccount->IsGMEnabled())
  {
    return true;
  }

  return false;
}


