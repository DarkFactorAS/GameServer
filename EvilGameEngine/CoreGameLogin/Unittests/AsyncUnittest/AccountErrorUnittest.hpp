
#include "PreCompile.h"

#include "EvilGameEngine/CoreGameLogin/Account/Account.h"
#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ServerAccountNetworkPackets/LoginServerAccountNetworkPacket.h"
#include "EvilEngine/CoreEngine/CoreUnittestManager/CodeUnittest/CoreAsyncUnittest.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/Packets/ServerErrorNetworkPacket.h"

class AccountErrorUnittest : public CoreAsyncUnittest, public SignalTarget
{
public:

  Account* m_TestAccount = NULL;

  AccountErrorUnittest() : CoreAsyncUnittest("AccountError")
  {
    m_TestAccount = new Account(0, "Olla Bolla", "TestDewd1", "Password12", 0);
  }

  virtual bool Execute() DF_OVERRIDE
  {
    UnittestGameEngineClient* gameClient = UnittestGameEngineClient::GetInstanceIfAny();
    if (gameClient == NULL)
    {
      return false;
    }

    CoreGameClientLoginModule* clientLoginModule = safe_cast<CoreGameClientLoginModule*> (gameClient->GetEngineModule(EngineModule::COREMODULETYPE_CLIENTLOGIN));
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
        Account* serverDbAccount = new Account(m_TestAccount->GetAccountId(), m_TestAccount->GetName(), m_TestAccount->GetUsername(), "PasswordTwo", m_TestAccount->GetIconId());
        serverDbAccount->SetLoginMethod(1);
        serverDbAccount->SetPlatform(1);

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

    ServerErrorNetworkPacket::s_SignalPacketError.Connect(this, &AccountErrorUnittest::SlotPacketError);

    clientLoginModule->SignalAccountLoggedOn.Connect(this, &AccountErrorUnittest::SlotAccountLoggedOn);
    clientLoginModule->SignalCreatedAccount.Connect(this, &AccountErrorUnittest::SlotAccountCreated);
    clientLoginModule->SignalAccountError.Connect(this, &AccountErrorUnittest::SlotAccountError);

    // pass as argument and not in account data
    m_TestAccount->SetLoginMethod(1);
    m_TestAccount->SetPlatform(1);

    //Account(AccountID accountId, const String& name, const String& username, const String& password, uint32 iconId);
    gameClient->SendPacketToServer(new LoginServerAccountNetworkPacket(1, "UnitTestGame", m_TestAccount));

    return true;
  }

  void SlotPacketError(uint32 /*packetId*/, uint32 errorCode)
  {
    Failed(String::FormatString("Received network packet error : %d", errorCode));
  }

  void SlotAccountLoggedOn(Account* /*account*/)
  {
    Failed("Received account login");
  }

  void SlotAccountCreated(Account* /*account*/)
  {
    Failed("Received account created");
  }

  void SlotAccountError(uint32 errorCode, const String& errorMessage)
  {
    if (errorCode == GameLoginPacketData::ErrorCode_WrongPassword)
    {
      Succeeded();
    }
    else
    {
      Failed(String::FormatString("Received account error : (%d/%s)", errorCode, errorMessage.c_str()));
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
