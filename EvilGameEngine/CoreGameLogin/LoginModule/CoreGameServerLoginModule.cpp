/*************************************************************************************
 * Project     : EvilEngine Multiplatform 2014
 * File        : EngineModule
 * Description : Abstract interface class for CoreGameEngine modules. This interface
 *               has to be implemented to be able to run as a sub component in the
 *               CoreGameEngine.
 *
 * Author      : Thor Richard Hansen
 *************************************************************************************/

#include "Precompile.h"
#include "CoreGameServerLoginModule.h"
#include "EvilEngine/CoreEngine/CoreEngine/EngineModule.h"
#include "EvilEngine/CoreNetwork/Common/NetworkBase.h"
#include "EvilEngine/CoreNetwork/Common/NetworkConnection.h"
#include "EvilEngine/CoreIO/Logger/Logger.h"

#include "EvilGameEngine/CoreGameLogin/LoginModule/CoreGameBaseLoginModule.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/GameEnginePacketData.h"

#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngineModule.h"

#include "EvilGameEngine/CoreGameLogin/Account/AccountManager.h"
#include "EvilGameEngine/CoreGameLogin/Network/GameLoginPacketData.h"

#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"

#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ServerAccountNetworkPackets/CreateServerAccountNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ServerAccountNetworkPackets/LoginServerAccountNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ServerAccountNetworkPackets/LoginGMNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ServerAccountNetworkPackets/UpdatedAccountFlagsNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ServerAccountNetworkPackets/SwitchLoggedInAccountNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ServerAccountNetworkPackets/LoginTokenServerAccountNetworkPacket.h"

#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ClientAccountNetworkPackets/ClientAccountLoggedOffNetworkPacket.h"

#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ItemShopPackets/PurchaseItemFromAmazonNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ItemShopPackets/PurchaseItemFromAppleNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ItemShopPackets/PurchaseItemFromDarkfactorNetworkPacket.h"

#ifdef DATABASE
  #include "EvilEngine/CoreDatabase/CoreDatabase.h"
#endif

#define PLAYER_START_ACCOUNT 1000

uint32          CoreGameServerLoginModule::s_AccountId = PLAYER_START_ACCOUNT;

CoreGameServerLoginModule::CoreGameServerLoginModule(bool isLive) :
  CoreGameBaseLoginModule(EngineModule::COREMODULETYPE_SERVERLOGIN, isLive)
{
  // Server account packets
  RegisterPacketType( GameLoginPacketData::PacketData_CreateServerAccount, CreateServerAccountNetworkPacket::Create );
  RegisterPacketType( GameLoginPacketData::PacketData_LoginServerAccount, LoginServerAccountNetworkPacket::Create );
  RegisterPacketType( GameLoginPacketData::PacketData_LoginGM, LoginGMNetworkPacket::Create );
  RegisterPacketType( GameLoginPacketData::PacketData_SwitchUser, SwitchLoggedInAccountNetworkPacket::Create);
  RegisterPacketType( GameLoginPacketData::PacketData_UpdatedAccountFlags, UpdatedAccountFlagsNetworkPacket::Create );
  RegisterPacketType(GameLoginPacketData::PacketData_LoginTokenServerAccount, LoginTokenServerAccountNetworkPacket::Create);

  // Itemshop packets
  RegisterPacketType( GameLoginPacketData::PacketData_AmazonItemPurchase, PurchaseItemFromAmazonNetworkPacket::Create);
  RegisterPacketType( GameLoginPacketData::PacketData_IOSItemPurchase, PurchaseItemFromAppleNetworkPacket::Create);
  RegisterPacketType( GameLoginPacketData::PacketData_WindowsItemPurchase, PurchaseItemFromDarkFactorNetworkPacket::Create);
   
}

CoreGameServerLoginModule* CoreGameServerLoginModule::GetModule(CoreEngine* coreEngine)
{
  if (coreEngine != NULL )
  {
    return safe_cast<CoreGameServerLoginModule*> (coreEngine->GetEngineModule(EngineModule::COREMODULETYPE_SERVERLOGIN));
  }
  return NULL;
}

void CoreGameServerLoginModule::OnConnectionDisconnected(uint32 connectionId)
{
  RemoveOnlineAccount( connectionId );
}

bool CoreGameServerLoginModule::AddOnlineAccount(Account* account, uint32 connectionId)
{
  return AccountManager::GetInstance()->AddOnlineAccount(account, connectionId);
}

void CoreGameServerLoginModule::RemoveOnlineAccount(uint32 connectionId)
{
  Account* account = AccountManager::GetInstance()->RemoveOnlineAccount(connectionId);

  // Forward the account disconnect to all modules
  if ( account != NULL )
  {
    CoreGameEngine* gameEngine = safe_cast<CoreGameEngine*>(GetEngine());
    if (gameEngine != NULL)
    {
      gameEngine->DisconnectAccountId(account->GetAccountId());
    }
  }
}

bool CoreGameServerLoginModule::AddAccountToCache(Account* account)
{
  return AccountManager::GetInstance()->AddAccountToCache(account);
}

bool CoreGameServerLoginModule::RemoveAccountFromCache(Account* account)
{
  return AccountManager::GetInstance()->RemoveAccountFromCache(account);
}

Account* CoreGameServerLoginModule::GetCachedAccount(AccountID accountId)
{
  return AccountManager::GetInstance()->GetCachedAccount(accountId);
}

Account* CoreGameServerLoginModule::GetRandomAccount(AccountID excludeAccountId)
{
  StringList accountList;
  accountList.push_back(String::FromInt(excludeAccountId));

  return AccountManager::GetInstance()->GetRandomAccount(accountList);
}

bool CoreGameServerLoginModule::VerifyLoginMetod(Account* account, uint8 loginMethod)
{
  // Only verify iOS Game Center data for now
  switch (loginMethod)
  {
  case Account::LOGIN_METHOD_IOSGAMECENTERCREATE:
    return AccountManager::VerifyGameCenterAccount(account);
  default:
    return true;
  }
}

int CoreGameServerLoginModule::LoginAccount(Account* account)
{
  uint8 loginMethod = account->GetLoginMethod();

  // Make sure that the login method matches
  uint8 loginCheck = loginMethod;
  if (loginMethod == Account::LOGIN_METHOD_IOSGAMECENTERLOGIN)
  {
    loginCheck = Account::LOGIN_METHOD_IOSGAMECENTERCREATE;
  }

  Account* existingAccount = GetLoginAccount(account->GetUsername(), loginCheck);
  if (existingAccount != NULL)
  {
    if (existingAccount->GetPassword().CompareWithCase(account->GetPassword()) == 0)
    {
      if (!VerifyLoginMetod(account, loginMethod))
      {
        return GameLoginPacketData::ErrorCode_BrokenAccountData;
      }

      // Disconnect already logged in account
      Account* cachedAccount = GetCachedAccount(existingAccount->GetAccountId());
      if (cachedAccount != NULL && cachedAccount->GetConnectionId() != account->GetConnectionId())
      {
        // Notify client that login status have been revoked
        SendPacketToClientAccount( cachedAccount->GetAccountId(), new ClientAccountLoggedOffNetworkPacket(cachedAccount->GetAccountId()));
        // Remove the online status on this connection
        RemoveOnlineAccount(cachedAccount->GetConnectionId());
      }

      String token = "1";

      account->Copy(existingAccount);
      account->SetLoginMethod(loginMethod);
      account->SetToken(token);

      RemoveAccountFromCache(existingAccount);
      delete existingAccount;

#ifdef DATABASE
      CoreDatabase::GetInstance()->ExecuteUpdate("update account set LastLogin=now() where AccountId=%d", account->GetAccountId());
      //CoreDatabase::GetInstance()->ExecuteUpdate("delete from accounttoken where AccountId=%d", account->GetAccountId());
      //CoreDatabase::GetInstance()->ExecuteUpdate("insert into accounttoken(AccountId, Username, Token) values(%d,'%s','%s')", 
      //  account->GetAccountId(),
      //  account->GetUsername(),
      //  account->GetToken());
#endif

      if (AddAccountToCache(account))
      {
        account->SetAuthenticated(true);
        LogInfoFMT("CoreGameServerLoginModule", "Logged in account %s ", account->GetDebugInfo().c_str());
        return GameEnginePacketData::ErrorCode_OK;
      }
      else
      {
        return GameLoginPacketData::ErrorCode_AccountAlreadyLoggedIn;
      }
    }
    else
    {
      return GameLoginPacketData::ErrorCode_WrongPassword;
    }
  }
  else if ( AccountManager::CanAutocreateAccount(loginMethod))
  {
    return CreateAccount(account);
  }
  return GameLoginPacketData::ErrorCode_LoginNoSuchUser;
}

Account* CoreGameServerLoginModule::LoginAccountToken(uint32 connectionId, const String& username, const String& token)
{
  Account* account = GetLoginAccount(username, Account::LOGIN_METHOD_DARKFACTOR);
  if (account != NULL)
  {
    if (account->GetToken().CompareWithCase(token) == 0)
    {
      return NULL;
    }

    // Disconnect already logged in account
    Account* cachedAccount = GetCachedAccount(account->GetAccountId());
    if ( cachedAccount == NULL )
    {
      AddAccountToCache(account);
    }
    else if (cachedAccount->GetConnectionId() != connectionId)
    {
      // Notify client that login status have been revoked
      SendPacketToClientAccount(cachedAccount->GetAccountId(), new ClientAccountLoggedOffNetworkPacket(cachedAccount->GetAccountId()));
      // Remove the online status on this connection
      RemoveOnlineAccount(cachedAccount->GetConnectionId());
      // Removed cached account
      RemoveAccountFromCache(cachedAccount);
      // Add newly logged in account to cache
      AddAccountToCache(account);
    }
    else
    {
      delete account;
      account = cachedAccount;
    }

    return account;
  }

  return NULL;
}

int CoreGameServerLoginModule::CreateAccount(Account* account)
{
  // Verify data before creating account
  int accountStatus = AccountManager::VerifyAccountData(account);
  if (accountStatus != 0)
  {
    return accountStatus;
  }

  Account* existingAccount = GetLoginAccount(account->GetUsername(), account->GetLoginMethod());
  if (existingAccount == NULL)
  {
#ifdef DATABASE
    // Check if we have any accounts already to adjust the ID
    if (s_AccountId == PLAYER_START_ACCOUNT)
    {
      const SQLResultSet& result = CoreDatabase::GetInstance()->ExecuteSelect("Select max( AccountId ) from account");
      if (result.Next())
      {
        s_AccountId = result.GetInt();
      }
    }
#endif

    uint32 purchaseFlags = 0;
    account->SetPurchaseFlags(purchaseFlags);
    account->SetAccountId(++s_AccountId);
    account->SetIconId(30064); // GuiResource_IconBoyBlack

    if (SaveAccountInDb(account))
    {
      AddAccountToCache(account);
      account->SetAuthenticated(true);
      return GameEnginePacketData::ErrorCode_OK;
    }
    else
    {
      return GameLoginPacketData::ErrorCode_CodeError;
    }
  }

  return GameLoginPacketData::ErrorCode_UserAlreadyExists;

}

Account* CoreGameServerLoginModule::GetLoginAccount(const String& username, int loginMethod)
{
  // TODO : Report error to client
  if (AccountManager::IsValidUsername(username) != GameEnginePacketData::ErrorCode_OK)
  {
    return NULL;
  }

  Account* account = NULL;

#ifdef DATABASE

  const SQLResultSet& result = CoreDatabase::GetInstance()->ExecuteSelect("Select AccountId, Name, Username, Password, IconId, PurchaseFlags, LoginMethod from account where Username='%s' and LoginMethod=%d", username.c_str(), loginMethod);
  if (result.Next())
  {
    uint32 accountId = result.GetUInt32();

    // Return account from cache if we have it
    account = GetCachedAccount(accountId);
    if (account != NULL)
    {
      return account;
    }

    // Return new instance of the account
    account = new Account();
    account->SetAccountId(accountId);
    account->SetName(result.GetString());
    account->SetUsername(result.GetString());
    account->SetPassword(result.GetString());
    account->SetIconId(result.GetUInt32());
    account->SetPurchaseFlags(result.GetUInt32());
    account->SetLoginMethod((uint8)result.GetUInt32());
    account->SetIsInDatabase(true);
  }

#else
  VOLATILE_ARG(loginMethod);
#endif

  return account;
}

bool CoreGameServerLoginModule::SaveAccountInDb(Account* account)
{
#ifdef DATABASE

  if (!account->IsInDatabase())
  {
    // Insert into the DB
    bool dbOk = CoreDatabase::GetInstance()->ExecuteInsert("Insert into account ( AccountId, Token, Name, Username, Password, IconId,  Platform, LoginMethod, PurchaseFlags, LastLogin ) values( %d, '%s', '%s', '%s', '%s', %d, %d, %d, %d, now() )",
      account->GetAccountId(),
      account->GetToken().c_str(),
      account->GetName().c_str(),
      account->GetUsername().c_str(),
      account->GetPassword().c_str(),
      account->GetIconId(),
      account->GetPlatform(),
      account->GetLoginMethod(),
      account->GetPurchaseFlags()
    );

    account->SetIsInDatabase(dbOk);
    return dbOk;
  }

#else
  VOLATILE_ARG(account);
#endif
  return false;
}


bool CoreGameServerLoginModule::SendPacketToClientAccount(uint32 accountId, BaseNetworkPacket* packet)
{
  CoreGameEngine* gameEngine = safe_cast<CoreGameEngine*> (GetEngine());
  if (gameEngine != NULL)
  {
    CoreGameServerLoginModule* loginModule = CoreGameServerLoginModule::GetModule(gameEngine);
    if (loginModule != NULL)
    {
      Account* account = loginModule->GetCachedAccount(accountId);
      if (account != NULL && account->GetConnectionId() != 0)
      {
        return SendPacketToClient(account->GetConnectionId(), packet);
      }
    }
  }
  return false;
}
