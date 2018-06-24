/*************************************************************************************
 * Project     : EvilEngine Multiplatform 2014
 * File        : EngineModule
 * Description : Abstract interface class for CoreGameEngine modules. This interface
 *               has to be implemented to be able to run as a sub component in the
 *               CoreGameEngine.
 *
 * Author      : Thor Richard Hansen
 *************************************************************************************/
#include "PreCompile.h"

#include "CoreGameClientLoginModule.h"
#include "EvilGameEngine/CoreGameLogin/LoginModule/CoreGameBaseLoginModule.h"
#include "EvilEngine/CoreEngine/CoreEngine/EngineModule.h"
#include "EvilEngine/CoreNetwork/Common/NetworkBase.h"

#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngineModule.h"

#include "EvilGameEngine/CoreGameLogin/Network/GameLoginPacketData.h"

#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ClientAccountNetworkPackets/ClientAccountCreatedNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ClientAccountNetworkPackets/ClientAccountLoggedOnNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ClientAccountNetworkPackets/ClientAccountLoggedOffNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ClientAccountNetworkPackets/ClientAccountErrorNetworkPacket.h"

CoreGameClientLoginModule::CoreGameClientLoginModule(bool isLive) :
  CoreGameBaseLoginModule(EngineModule::COREMODULETYPE_CLIENTLOGIN, isLive), 
  m_ClientAccount(NULL)
{
  RegisterPacketType(GameLoginPacketData::PacketData_CreateClientAccount, ClientAccountCreatedNetworkPacket::Create);
  RegisterPacketType(GameLoginPacketData::PacketData_ClientLoggedOnAccount, ClientAccountLoggedOnNetworkPacket::Create);
  RegisterPacketType(GameLoginPacketData::PacketData_ClientLoggedOffAccount, ClientAccountLoggedOffNetworkPacket::Create);
  RegisterPacketType(GameLoginPacketData::PacketData_LoginClientError, ClientAccountErrorNetworkPacket::Create);
}

bool CoreGameClientLoginModule::AddOnlineAccount(Account* account, uint32 connectionId)
{
  m_ClientAccount = account;
  m_ClientAccount->SetConnectionId( connectionId );

  Account::SetActiveAccount(account);
  Account::s_SignalReceivedAccountInfo();

  return true;
}

void CoreGameClientLoginModule::RemoveOnlineAccount(uint32 connectionId)
{
  if (m_ClientAccount != NULL && m_ClientAccount->GetConnectionId() == connectionId)
  {
    m_ClientAccount = NULL;
  }
}

void CoreGameClientLoginModule::SendSignalAccountLoggedOn(Account* account)
{
  SignalAccountLoggedOn(account);
}


