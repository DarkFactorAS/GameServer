#pragma once

#include "EvilGameEngine/CoreGameLogin/LoginModule/CoreGameServerLoginModule.h"
#include "EvilGameEngine/CoreGameLogin/Account/AccountManager.h"

class UnittestGameServerLoginModule : public CoreGameServerLoginModule
{
public:

  UnittestGameServerLoginModule() : CoreGameServerLoginModule()
  {
  }

  void ClearAccountList()
  {
    m_AccountList.clear();
  }

  void AddAccountToDb(Account* account)
  {
    m_AccountList.push_back(account);
  }

protected:

  std::vector< Account* > m_AccountList;

  virtual bool                          SaveAccountInDb(Account* /*account*/) DF_OVERRIDE
  {
    return true;
  }

  virtual Account* GetLoginAccount(const String& username, int loginMethod) DF_OVERRIDE
  {
    for (std::vector<Account*>::iterator itAccount = m_AccountList.begin(); itAccount != m_AccountList.end(); ++itAccount)
    {
       Account* account = *itAccount;
       if (account->GetUsername().EqualsWithCase(username) && account->GetLoginMethod() == loginMethod )
       {
         return account;
       }
    }
    return NULL;
  }

};