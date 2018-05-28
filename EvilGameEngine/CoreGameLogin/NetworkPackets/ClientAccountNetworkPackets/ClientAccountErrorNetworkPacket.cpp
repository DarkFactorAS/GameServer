/*************************************************************************************************
 * Project     : EvilEngine Multi-platform 2014-2018
 * File        : LoginClientErrorNetworkPacket
 *
 * Description : Network packet that sends an error to the client. 
 *
 * Author      : Thor Richard Hansen
 *************************************************************************************************/

#include "Precompile.h"
#include "ClientAccountErrorNetworkPacket.h"
#include "EvilEngine/CoreLib/BasicTypes/Definitions/VolatileArgs.h"
#include "EvilEngine/CoreVersion/version.h"
#include "EvilEngine/CoreIO/Logger/Logger.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/GameEnginePacketData.h"
#include "EvilGameEngine/CoreGameLogin/Account/AccountManager.h"
#include "EvilGameEngine/CoreGameLogin/Account/GMFlags.h"
#include "EvilGameEngine/CoreGameLogin/Network/GameLoginPacketData.h"

#include "EvilGameEngine/CoreGameLogin/LoginModule/CoreGameClientLoginModule.h"

ClientAccountErrorNetworkPacket::ClientAccountErrorNetworkPacket( const BinaryStream* datastream) :
  BaseNetworkPacket( GameLoginPacketData::PacketData_LoginClientError, datastream )
{
  m_PacketId      = datastream->ReadUInt32();
  m_PacketError   = datastream->ReadUInt32();
  m_ErrorMessage  = datastream->ReadString();
}

ClientAccountErrorNetworkPacket::ClientAccountErrorNetworkPacket(uint32 packetId, uint32 packetError, const String& errorMessage) :
  BaseNetworkPacket( GameLoginPacketData::PacketData_LoginClientError),
  m_PacketId(packetId),
  m_PacketError(packetError),
  m_ErrorMessage(errorMessage)
{
}

BinaryStream* ClientAccountErrorNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseNetworkPacket::GetDataStream();

  datastream->WriteUInt32(m_PacketId);
  datastream->WriteUInt32(m_PacketError);
  datastream->WriteString(m_ErrorMessage);

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

void ClientAccountErrorNetworkPacket::Execute()
{
  CoreGameClientLoginModule* loginModule = safe_cast<CoreGameClientLoginModule*> (GetEngineModule(EngineModule::COREMODULETYPE_CLIENTLOGIN));
  if (loginModule != NULL)
  {
    loginModule->SignalAccountError(m_PacketError, m_ErrorMessage);
  }
}
