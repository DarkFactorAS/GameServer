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
#include "ClientAccountLoggedOffNetworkPacket.h"
#include "EvilEngine/CoreLib/BasicTypes/Definitions/VolatileArgs.h"
#include "EvilEngine/CoreVersion/version.h"
#include "EvilEngine/CoreIO/Logger/Logger.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/GameEnginePacketData.h"
#include "EvilGameEngine/CoreGameLogin/Account/AccountManager.h"
#include "EvilGameEngine/CoreGameLogin/Account/GMFlags.h"
#include "EvilGameEngine/CoreGameLogin/Network/GameLoginPacketData.h"

#include "EvilGameEngine/CoreGameLogin/LoginModule/CoreGameClientLoginModule.h"

ClientAccountLoggedOffNetworkPacket::ClientAccountLoggedOffNetworkPacket( const BinaryStream* datastream) :
  BaseNetworkPacket( GameLoginPacketData::PacketData_ClientLoggedOffAccount, datastream )
{
  m_AccountId = datastream->ReadUInt32();
}

ClientAccountLoggedOffNetworkPacket::ClientAccountLoggedOffNetworkPacket( uint32 accountId ) :
  BaseNetworkPacket( GameLoginPacketData::PacketData_ClientLoggedOffAccount),
  m_AccountId( accountId )
{
}

BinaryStream* ClientAccountLoggedOffNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseNetworkPacket::GetDataStream();
  datastream->WriteUInt32(m_AccountId);
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

void ClientAccountLoggedOffNetworkPacket::Execute()
{
  //CoreGameClientLoginModule* loginModule = safe_cast<CoreGameClientLoginModule*> (GetEngineModule(EngineModule::COREMODULETYPE_CLIENTLOGIN));
  //if (loginModule != NULL)
  //{
  //  loginModule->AddOnlineAccount( m_Account, m_ConnectionId);
  //  loginModule->SignalAccountLoggedOn(m_Account);
  //}
}
