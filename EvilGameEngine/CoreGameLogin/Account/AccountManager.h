#ifndef EVILENGINE_COREGAMELOGIN_ACCOUNT_ACCOUNTMANAGER
#define EVILENGINE_COREGAMELOGIN_ACCOUNT_ACCOUNTMANAGER 1
#pragma once

#include "Account.h"
#include "EvilEngine/CoreLib/BasicTypes/String/StringList.h"
#include <map>

class AccountManager
{
public:

  AccountManager();
  virtual ~AccountManager(){};

  static AccountManager*        GetInstance();

  Account*                      LoadActiveAccount();

  std::vector< Account* >       GetAccountsWithNick( Account* excludeAccount, String searchName );

#ifdef SERVER

  virtual Account*              GetOtherAccount( AccountID accountId );
  virtual Account*              GetRandomAccount(const StringList& accountList);

  bool                          UpdateMembership(Account* account, uint32 itemCode);

  Account*                      GetOnlineAccountFromNetwork(uint32 connectionId);
  uint32                        GetOnlineConnectionIdFromAccountId(AccountID accountId);

#endif

  static uint32                 s_FakeAccountId;
  bool                          AddAccountToCache(Account* account);
  bool                          RemoveAccountFromCache(Account* account);
  Account*                      GetCachedAccount(AccountID accountId);
  Account*                      GetOfflineAccount( const String& username, int loginMethod );

  static int                   VerifyAccountData(Account* account);

private:

  friend class CoreGameClientLoginModule;
  friend class CoreGameServerLoginModule;

#ifdef SERVER
  uint32                        GetReceiptWithId( const String& receiptId );
  bool                          AddReceipt( Account* account, uint32 itemCode, const String& receiptId, const String& receipt );
#endif

  // Moving to login module
  bool                          AddOnlineAccount(Account* account, uint32 connectionId);
  Account*                      RemoveOnlineAccount(uint32 connectionId);

  static bool                   VerifyGameCenterAccount(Account* account);

  static bool                  CanAutocreateAccount(uint8 loginMethod);
  static bool                  HasValidCharacters( const String& validateString, uint32 validator );
  static bool                  IsValidEmailFormat( const String& validateString );
  static int                   IsValidName( const String& name );
  static int                   IsValidUsername( const String& username );
  static int                   IsValidPassword( const String& password );

  static uint32                 s_AccountId;
  std::map< uint32, Account* >  m_AccountList;
  std::map< uint32, AccountID > m_OnlineAccounts;
};

#endif /// EVILENGINE_COREGAMELOGIN_ACCOUNT_ACCOUNTMANAGER
