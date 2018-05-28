/*************************************************************************************************
 * Project     : EvilEngine Multi-platform 2014-2016
 * File        : SwitchLoggedInAccountNetworkPacket
 *
 * Description : Network packet switch the logged in playet to a different player. This is done
 *               without knowing the password of the player. The active player must be logged in
 *               and a GM.
 *
 * Author      : Thor Richard Hansen
 *************************************************************************************************/

#include "Precompile.h"
#include "SwitchLoggedInAccountNetworkPacket.h"
#include "UpdatedAccountFlagsNetworkPacket.h"
#include "EvilEngine/CoreLib/BasicTypes/Definitions/VolatileArgs.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/GameEnginePacketData.h"
#include "EvilGameEngine/CoreGameLogin/Account/AccountManager.h"
#include "EvilGameEngine/CoreGameLogin/Account/GMFlags.h"
#include "EvilGameEngine/CoreGameLogin/Network/GameLoginPacketData.h"
#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ServerAccountNetworkPackets/UpdatedAccountFlagsNetworkPacket.h"

#ifdef CLIENT

SwitchLoggedInAccountNetworkPacket::SwitchLoggedInAccountNetworkPacket( const BinaryStream* datastream) :
  BaseAuthentcatedNetworkPacket(GameLoginPacketData::PacketData_SwitchUser, datastream)
{
  m_Account = new Account();
  m_Account->CreateFromStream(datastream);
}

SwitchLoggedInAccountNetworkPacket::SwitchLoggedInAccountNetworkPacket( const String& username) :
  BaseAuthentcatedNetworkPacket( GameLoginPacketData::PacketData_SwitchUser ),
  m_Account( NULL ),
  m_Username( username )
{
}
#endif

#ifdef SERVER

SwitchLoggedInAccountNetworkPacket::SwitchLoggedInAccountNetworkPacket( const BinaryStream* datastream) :
  BaseAuthentcatedNetworkPacket(GameLoginPacketData::PacketData_SwitchUser, datastream)
{
  m_Username = datastream->ReadString();
}

SwitchLoggedInAccountNetworkPacket::SwitchLoggedInAccountNetworkPacket( Account* account ) :
  BaseAuthentcatedNetworkPacket(GameLoginPacketData::PacketData_SwitchUser),
  m_Account(account)
{
}

#endif

BinaryStream* SwitchLoggedInAccountNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseNetworkPacket::GetDataStream();
  datastream->WriteString( m_Username );
  return datastream;
}


/************************************************************************************************
 * SendSwitchUser:
 * Sends a request to switch the active account with a different one. This functionality is only
 * available if the first account is flagged as GM.
 *
 * @param  (BinaryStream)     dataStream  - The datastream with the account
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
void SwitchLoggedInAccountNetworkPacket::Execute()
{
#ifdef SERVER

  AccountManager* accountManager = AccountManager::GetInstance();
  if (accountManager != NULL)
  {
    Account* account = GetAccount();
    if ( account != NULL && account->HasServerGMFlag() )
    {
      std::vector< Account* > accountList = accountManager->GetAccountsWithNick(account, m_Username);
      if (!accountList.empty())
      {
        Account* switchAccount = accountList.front();

        accountManager->AddAccountToCache(switchAccount);
        switchAccount->SetAuthenticated(true);
        switchAccount->AddPurchaseFlags(GM_FLAGS::CLIENTGMFLAG | GM_FLAGS::GMFLAG | GM_FLAGS::NEVER_SAVE);

        SendPacketToClient( new SwitchLoggedInAccountNetworkPacket(switchAccount) );
      }
      else
      {
        SendErrorToClient(  GameLoginPacketData::ErrorCode_LoginNoSuchUser);
      }
    }
    else
    {
      SendErrorToClient(GameLoginPacketData::ErrorCode_RequiresGM);
    }
  }
  else
  {
    SendErrorToClient(GameLoginPacketData::ErrorCode_CodeError);
  }
  
#else

  m_Account->SetAutoLogin(true);
  m_Account->SetAuthenticated(true);

  Account::SetActiveAccount(m_Account);
  Account::s_SignalReceivedAccountInfo();
  
#endif
}
