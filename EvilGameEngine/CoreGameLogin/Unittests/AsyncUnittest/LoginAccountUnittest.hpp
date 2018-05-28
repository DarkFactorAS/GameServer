
#include "PreCompile.h"

#include "EvilGameEngine/CoreGameLogin/Account/Account.h"
#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ServerAccountNetworkPackets/LoginServerAccountNetworkPacket.h"
#include "EvilEngine/CoreEngine/CoreUnittestManager/CodeUnittest/CoreAsyncUnittest.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/Packets/ServerErrorNetworkPacket.h"

class LoginAccountUnittest : public CoreAsyncUnittest, public SignalTarget
{
public:

  Account* m_TestAccount = NULL;

  LoginAccountUnittest() : CoreAsyncUnittest("LoginAccount")
  {
    m_TestAccount = new Account(1001, "Olla Bolla", "TestDewd1", "Password12", 0);
  }

  virtual bool Execute() DF_OVERRIDE
  {
    UnittestGameEngineClient* gameClient = UnittestGameEngineClient::GetInstanceIfAny();
    if (gameClient == NULL)
    {
      return false;
    }

    CoreGameClientLoginModule* clientLoginModule = safe_cast< CoreGameClientLoginModule* > ( gameClient->GetEngineModule(EngineModule::COREMODULETYPE_CLIENTLOGIN) );
    if (clientLoginModule == NULL)
    {
      Failed("CoreGameClientLoginModule is NULL");
      return false;
    }

    UnittestGameEngineServer* gameServer = UnittestGameEngineServer::GetInstanceIfAny();
    if (gameServer != NULL)
    {
      UnittestGameServerLoginModule* serverLoginModule = safe_cast<UnittestGameServerLoginModule*> (gameServer->GetEngineModule(EngineModule::COREMODULETYPE_SERVERLOGIN));
      if (serverLoginModule != NULL)
      {
        Account* serverDbAccount = new Account(m_TestAccount->GetAccountId(), m_TestAccount->GetName(), m_TestAccount->GetUsername(), m_TestAccount->GetPassword(), m_TestAccount->GetIconId() );
        serverDbAccount->SetLoginMethod( 1 ); 
        serverDbAccount->SetPlatform( 1 ); 

        serverLoginModule->AddAccountToDb(serverDbAccount);
      }
      else
      {
        Failed("UnittestGameServerLoginModule is NULL");
        return false;
      }
    }
    else
    {
      Failed("UnittestGameEngineServer is NULL");
      return false;
    }

    ServerErrorNetworkPacket::s_SignalPacketError.Connect(this, &LoginAccountUnittest::SlotPacketError);

    clientLoginModule->SignalAccountLoggedOn.Connect( this, &LoginAccountUnittest::SlotAccountLoggedOn);
    clientLoginModule->SignalCreatedAccount.Connect(this, &LoginAccountUnittest::SlotAccountCreated);
    clientLoginModule->SignalAccountError.Connect(this, &LoginAccountUnittest::SlotAccountError);

    // pass as argument and not in account data
    m_TestAccount->SetLoginMethod(1);
    m_TestAccount->SetPlatform(1);

    //Account(AccountID accountId, const String& name, const String& username, const String& password, uint32 iconId);
    gameClient->SendPacketToServer( new LoginServerAccountNetworkPacket( 1, "UnitTestGame", m_TestAccount) );

    return true;
  }

  void SlotPacketError(uint32 /*packetId*/, uint32 errorCode)
  {
    Failed(String::FormatString("Received network packet error : %d", errorCode));
  }

  void SlotAccountError(uint32 errorCode, const String& errorMessage)
  {
    Failed(String::FormatString("Received account error : (%d/%s)", errorCode, errorMessage.c_str()));
  }

  void SlotAccountCreated(Account* /*account*/)
  {
    Failed("Received account login");
  }
  
  void SlotAccountLoggedOn(Account* account)
  {
    if (account->GetAccountId() != m_TestAccount->GetAccountId())
    {
      Failed("Received wrong account");
    }
    else if (account->GetAccountId() == 0)
    {
      Failed("No account ID");
    }
    else
    {
      Succeeded();
    }
  }

  virtual bool Cleanup() DF_OVERRIDE
  {
    UnittestGameEngineServer* gameServer = UnittestGameEngineServer::GetInstanceIfAny();
    if (gameServer != NULL)
    {
      UnittestGameServerLoginModule* serverLoginModule = safe_cast<UnittestGameServerLoginModule*> (gameServer->GetEngineModule(EngineModule::COREMODULETYPE_SERVERLOGIN));
      if (serverLoginModule != NULL)
      {
        serverLoginModule->ClearAccountList();
      }
    }

    return CoreAsyncUnittest::Cleanup();
  }
};
