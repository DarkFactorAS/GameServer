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
#include "LoginTokenServerAccountNetworkPacket.h"
#include "UpdatedAccountFlagsNetworkPacket.h"
#include "EvilEngine/CoreLib/BasicTypes/Definitions/VolatileArgs.h"
#include "EvilEngine/CoreVersion/version.h"
#include "EvilEngine/CoreIO/Logger/Logger.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/GameEnginePacketData.h"
#include "EvilGameEngine/CoreGameLogin/Account/AccountManager.h"
#include "EvilGameEngine/CoreGameLogin/Account/GMFlags.h"
#include "EvilGameEngine/CoreGameLogin/Network/GameLoginPacketData.h"
#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ServerAccountNetworkPackets/UpdatedAccountFlagsNetworkPacket.h"

#include "EvilGameEngine/CoreGameLogin/LoginModule/CoreGameServerLoginModule.h"

LoginTokenServerAccountNetworkPacket::LoginTokenServerAccountNetworkPacket( const BinaryStream* datastream) :
  BaseLoginModuleNetworkPacket( GameLoginPacketData::PacketData_LoginServerAccount, datastream )
{
  m_MinimumServerVersion = datastream->ReadUInt32();
  m_GameKey              = datastream->ReadString();

  m_UserName             = datastream->ReadString();
  m_Token                = datastream->ReadString();
}

LoginTokenServerAccountNetworkPacket::LoginTokenServerAccountNetworkPacket( uint32 requiredVersion, const String& gameKey, const String& username, const String& token ) :
  BaseLoginModuleNetworkPacket( GameLoginPacketData::PacketData_LoginServerAccount),
  m_MinimumServerVersion( requiredVersion ),
  m_GameKey( gameKey ),
  m_UserName(username),
  m_Token(token)
{
}

BinaryStream* LoginTokenServerAccountNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseLoginModuleNetworkPacket::GetDataStream();
  datastream->WriteUInt32(m_MinimumServerVersion);
  datastream->WriteString( m_GameKey );

  datastream->WriteString(m_UserName);
  datastream->WriteString(m_Token);

  return datastream;
}

bool LoginTokenServerAccountNetworkPacket::CanExecute()
{
  if (m_MinimumServerVersion > BUILD_REVISION )
  {
    SendErrorToClient(GameEnginePacketData::ErrorCode_Version_ServerIsTooOld);
    LogWarningFMT("LoginAccountNetworkPacket", "Server version is too old (%d). Client required minimum %d", BUILD_REVISION, m_MinimumServerVersion);
    return false;
  }

  if ( m_GameKey.empty() )
  {
    LogWarning("CreateServerAccountNetworkPacket", "Missing gamekey");
    SendErrorToClient(GameEnginePacketData::ErrorCode_WrongGameKey);
    return false;
  }

  // Must be correct gamekey
  CoreGameEngine* gameEngine = GetGameEngine();
  if ( gameEngine != NULL )
  {
    String gameKey = gameEngine->GetGameKey();
    if ( !m_GameKey.EqualsWithCase(gameKey) )
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

void LoginTokenServerAccountNetworkPacket::Execute()
{
  CoreGameServerLoginModule* loginModule = safe_cast<CoreGameServerLoginModule*> (GetEngineModule(EngineModule::COREMODULETYPE_SERVERLOGIN));
  if (loginModule != NULL)
  {
    Account* account = loginModule->LoginAccountToken(m_ConnectionId, m_UserName, m_Token);
    if (account != NULL)
    {
      account->SetConnectionId(m_ConnectionId);
      account->SetIpAddress(GetIpAddress());
      account->SetAuthenticated(true);

      loginModule->AddOnlineAccount(account, m_ConnectionId);
      SendPacketToClient(new ClientAccountLoggedOnNetworkPacket(account));
    }
    else
    {
      SendLoginErrorToClient(GameLoginPacketData::PacketData_LoginTokenServerAccount, GameLoginPacketData::ErrorCode_TokenExpired);
    }
  }
  else
  {
    SendLoginErrorToClient(GameLoginPacketData::PacketData_LoginTokenServerAccount, GameLoginPacketData::ErrorCode_BrokenAccountData);
  }
}
