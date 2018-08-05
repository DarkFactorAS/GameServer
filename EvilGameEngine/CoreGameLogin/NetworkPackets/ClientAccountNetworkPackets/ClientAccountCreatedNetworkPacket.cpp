/*************************************************************************************************
 * Project     : EvilEngine Multi-platform 2014-2016
 * File        : LoginAccountNetworkPacket
 *
 * Description : Network packet to log in the player on the server. Most of the game related
 *               packets require the player to be logged on to the server.
 *
 * Author      : Thor Richard Hansen
 *************************************************************************************************/

#include "Precompile.h"
#include "ClientAccountCreatedNetworkPacket.h"
#include "EvilEngine/CoreLib/BasicTypes/Definitions/VolatileArgs.h"
#include "EvilEngine/CoreVersion/version.h"
#include "EvilEngine/CoreIO/Logger/Logger.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/GameEnginePacketData.h"
#include "EvilGameEngine/CoreGameLogin/Account/AccountManager.h"
#include "EvilGameEngine/CoreGameLogin/Account/GMFlags.h"
#include "EvilGameEngine/CoreGameLogin/Network/GameLoginPacketData.h"

#include "EvilGameEngine/CoreGameLogin/LoginModule/CoreGameClientLoginModule.h"

ClientAccountCreatedNetworkPacket::ClientAccountCreatedNetworkPacket( const BinaryStream* datastream) :
  BaseNetworkPacket( GameLoginPacketData::PacketData_CreateClientAccount, datastream )
{
  /*uint8 accountVersion = */datastream->ReadUInt8();

  m_Account = new Account();
  m_Account->SetAccountId( datastream->ReadUInt32() );
  m_Account->SetUsername( datastream->ReadString() );
  m_Account->SetToken(datastream->ReadString());
  m_Account->SetPurchaseFlags( datastream->ReadUInt32() );
  m_Account->SetIconId( datastream->ReadUInt32() );
}

ClientAccountCreatedNetworkPacket::ClientAccountCreatedNetworkPacket( Account* account ) :
  BaseNetworkPacket( GameLoginPacketData::PacketData_CreateClientAccount),
  m_Account( account )
{
}

BinaryStream* ClientAccountCreatedNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseNetworkPacket::GetDataStream();

  datastream->WriteUInt8(Account::GetAccountVersion());
  datastream->WriteUInt32(m_Account->GetAccountId());
  datastream->WriteString(m_Account->GetUsername());
  datastream->WriteString(m_Account->GetToken());
  datastream->WriteUInt32(m_Account->GetPurchaseFlags());
  datastream->WriteUInt32(m_Account->GetIconId());

  return datastream;
}

/************************************************************************************************
 * AccountLoggedIn:
 * Received a newly created or logged in Account. Set the account as active and notity client
 *
 * @param  (BinaryStream)     dataStream  - The datastream with the account
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/

void ClientAccountCreatedNetworkPacket::Execute()
{
  CoreGameClientLoginModule* loginModule = safe_cast<CoreGameClientLoginModule*> (GetEngineModule(EngineModule::COREMODULETYPE_CLIENTLOGIN));
  if (loginModule != NULL)
  {
    m_Account->SetAutoLogin(true);
    m_Account->SaveUserConfig();
    m_Account->SetAuthenticated(true);
    loginModule->AddOnlineAccount( m_Account, m_ConnectionId);
    loginModule->SignalCreatedAccount(m_Account);
  }
}
