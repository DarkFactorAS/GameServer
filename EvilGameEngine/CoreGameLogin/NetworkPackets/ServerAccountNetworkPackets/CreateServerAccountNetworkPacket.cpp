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
#include "CreateServerAccountNetworkPacket.h"
#include "EvilEngine/CoreVersion/version.h"
#include "EvilEngine/CoreIO/Logger/Logger.h"
#include "EvilEngine/CoreLib/BasicTypes/Definitions/VolatileArgs.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/GameEnginePacketData.h"
#include "EvilGameEngine/CoreGameLogin/Account/AccountManager.h"
#include "EvilGameEngine/CoreGameLogin/Account/GMFlags.h"
#include "EvilGameEngine/CoreGameLogin/Network/GameLoginPacketData.h"

#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ClientAccountNetworkPackets/ClientAccountCreatedNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/LoginModule/CoreGameServerLoginModule.h"

CreateServerAccountNetworkPacket::CreateServerAccountNetworkPacket( const BinaryStream* datastream) :
  BaseLoginModuleNetworkPacket( GameLoginPacketData::PacketData_CreateServerAccount, datastream )
{
  m_MinimumServerVersion = datastream->ReadUInt32();
  m_GameKey         = datastream->ReadString();

  /*uint8 accountVersion = */datastream->ReadUInt8();
  
  m_Account = new Account();
  m_Account->SetName( datastream->ReadString() );
  m_Account->SetUsername( datastream->ReadString() );
  m_Account->SetPassword( datastream->ReadString() );
  m_Account->SetPlatform( datastream->ReadUInt8() );
  m_Account->SetLoginMethod( datastream->ReadUInt8() );
}

CreateServerAccountNetworkPacket::CreateServerAccountNetworkPacket(uint32 requiredVersion, const String& gameKey,Account* account) :
  BaseLoginModuleNetworkPacket( GameLoginPacketData::PacketData_CreateServerAccount),
  m_MinimumServerVersion(requiredVersion),
  m_GameKey(gameKey),
  m_Account( account )
{
}

BinaryStream* CreateServerAccountNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseLoginModuleNetworkPacket::GetDataStream();
  datastream->WriteUInt32(m_MinimumServerVersion);
  datastream->WriteString(m_GameKey);

  datastream->WriteUInt8( Account::GetAccountVersion());
  datastream->WriteString(m_Account->GetName());
  datastream->WriteString(m_Account->GetUsername());
  datastream->WriteString(m_Account->GetPassword());
  datastream->WriteUInt8(m_Account->GetLoginMethod());
  datastream->WriteUInt8(m_Account->GetPlatform());

  return datastream;
}

bool CreateServerAccountNetworkPacket::CanExecute()
{
  if (m_MinimumServerVersion > BUILD_REVISION)
  {
    SendErrorToClient(GameEnginePacketData::ErrorCode_Version_ServerIsTooOld);
    LogWarningFMT("LoginAccountNetworkPacket", "Server version is too old (%d). Client required minimum %d", BUILD_REVISION, m_MinimumServerVersion);
    return false;
  }

  if (m_GameKey.empty())
  {
    LogWarning("CreateServerAccountNetworkPacket", "Missing gamekey");
    SendErrorToClient(GameEnginePacketData::ErrorCode_WrongGameKey);
    return false;
  }

  // Must be correct gamekey
  CoreGameEngine* gameEngine = GetGameEngine();
  if (gameEngine != NULL)
  {
    String gameKey = gameEngine->GetGameKey();
    if (!m_GameKey.EqualsWithCase(gameKey))
    {
      LogWarningFMT("CreateServerAccountNetworkPacket", "Wrong gamekey '%s'. Expected '%s'", m_GameKey.c_str(), gameKey.c_str());
      SendErrorToClient(GameEnginePacketData::ErrorCode_WrongGameKey);
      return false;
    }
  }
  return true;
}

/************************************************************************************************
 * AccountLoggedIn:
 * Received a newly created or logged in Account. Set the account as active and notity client
 *
 * @param  (BinaryStream)     dataStream  - The datastream with the account
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/

void CreateServerAccountNetworkPacket::Execute()
{
  CoreGameServerLoginModule* loginModule = safe_cast< CoreGameServerLoginModule*> ( GetEngineModule(EngineModule::COREMODULETYPE_SERVERLOGIN) );
  if (loginModule != NULL)
  {
    m_Account->SetConnectionId(m_ConnectionId);
    m_Account->SetIpAddress(GetIpAddress());

    uint32 errorCode = loginModule->CreateAccount( m_Account );
    if ( errorCode == GameEnginePacketData::ErrorCode_OK )
    {
      loginModule->AddOnlineAccount(m_Account, m_ConnectionId);
      SendPacketToClient( new ClientAccountCreatedNetworkPacket( m_Account ) );
    }
    else
    {
      SendLoginErrorToClient(GameLoginPacketData::PacketData_CreateServerAccount, errorCode);
    }
  }
  else
  {
    SendLoginErrorToClient(GameLoginPacketData::PacketData_CreateServerAccount, GameLoginPacketData::ErrorCode_BrokenAccountData);
  }
}
