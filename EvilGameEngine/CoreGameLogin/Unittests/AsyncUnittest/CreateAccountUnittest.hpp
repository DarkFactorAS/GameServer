
#include "PreCompile.h"

#include "EvilGameEngine/CoreGameLogin/Account/Account.h"
#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ServerAccountNetworkPackets/CreateServerAccountNetworkPacket.h"
#include "EvilEngine/CoreEngine/CoreUnittestManager/CodeUnittest/CoreAsyncUnittest.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/Packets/ServerErrorNetworkPacket.h"

class CreateAccountUnittest : public CoreAsyncUnittest, public SignalTarget
{
public:

  Account* m_TestAccount = NULL;

  CreateAccountUnittest() : CoreAsyncUnittest("CreateAccount")
  {
    m_TestAccount = new Account(0, "OllaBolla", "TestDewd1", "Password12", 0);
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

    ServerErrorNetworkPacket::s_SignalPacketError.Connect(this, &CreateAccountUnittest::SlotPacketError);

    clientLoginModule->SignalAccountLoggedOn.Connect( this, &CreateAccountUnittest::SlotAccountLoggedOn);
    clientLoginModule->SignalCreatedAccount.Connect(this, &CreateAccountUnittest::SlotAccountCreated);
    clientLoginModule->SignalAccountError.Connect(this, &CreateAccountUnittest::SlotAccountError);

    //Account(AccountID accountId, const String& name, const String& username, const String& password, uint32 iconId);
    gameClient->SendPacketToServer( new CreateServerAccountNetworkPacket( 1, "UnitTestGame", m_TestAccount) );

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

  void SlotAccountLoggedOn(Account* /*account*/)
  {
    Failed("Received account login");
  }

  void SlotAccountCreated(Account* account)
  {
    if (account->GetUsername() != m_TestAccount->GetUsername())
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
    return CoreAsyncUnittest::Cleanup();
  }
};
