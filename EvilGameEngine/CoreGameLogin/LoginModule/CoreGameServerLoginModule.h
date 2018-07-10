/*************************************************************************************
 * Project     : EvilEngine Multiplatform 2014
 * File        : EngineModule
 * Description : Abstract interface class for CoreGameEngine modules. This interface
 *               has to be implemented to be able to run as a sub component in the
 *               CoreGameEngine.
 *
 * Author      : Thor Richard Hansen
 *************************************************************************************/

#ifndef EVILENGINE_COREGAMELOGIN_COREGAMESERVERLOGINMODULE
#define EVILENGINE_COREGAMELOGIN_COREGAMESERVERLOGINMODULE 1

#include "EvilEngine/CoreEngine/CoreEngine/EngineModule.h"
#include "EvilEngine/CoreNetwork/Common/NetworkBase.h"

#include "EvilGameEngine/CoreGameLogin/LoginModule/CoreGameBaseLoginModule.h"

#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngineModule.h"

#include "EvilGameEngine/CoreGameLogin/Account/AccountManager.h"
#include "EvilGameEngine/CoreGameLogin/Network/GameLoginPacketData.h"

#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ServerAccountNetworkPackets/CreateServerAccountNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ServerAccountNetworkPackets/LoginServerAccountNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ServerAccountNetworkPackets/LoginGMNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ServerAccountNetworkPackets/UpdatedAccountFlagsNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ServerAccountNetworkPackets/SwitchLoggedInAccountNetworkPacket.h"

#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ItemShopPackets/PurchaseItemFromAmazonNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ItemShopPackets/PurchaseItemFromAppleNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ItemShopPackets/PurchaseItemFromDarkfactorNetworkPacket.h"

class CoreGameServerLoginModule : public CoreGameBaseLoginModule
{
public:
  
  CoreGameServerLoginModule(bool isLive);
  
  static CoreGameServerLoginModule* GetModule(CoreGameEngine* engine);

  virtual String                GetModuleName() DF_OVERRIDE { return StaticStr("CoreGameServerLoginModule"); }
  virtual void                  OnConnectionDisconnected(uint32 connectionId) DF_OVERRIDE;

  virtual bool                  AddOnlineAccount(Account* account, uint32 connectionId) DF_OVERRIDE;
  virtual void                  RemoveOnlineAccount(uint32 /*connectionId*/) DF_OVERRIDE;
  int                           LoginAccount(Account* account);

  bool                          AddAccountToCache(Account* account);
  bool                          RemoveAccountFromCache(Account* account);
  Account*                      GetCachedAccount(AccountID accountId);

  int                           CreateAccount(Account* account);

  bool                          SendPacketToClientAccount(uint32 accountId, BaseNetworkPacket* packet);

protected:

  virtual bool                  SaveAccountInDb(Account* account);
  virtual Account*              GetLoginAccount(const String& username, int loginMethod);
  bool                          VerifyLoginMetod(Account* account, uint8 loginMethod);

  static uint32                 s_AccountId;
};

#endif /// EVILENGINE_COREGAMELOGIN_COREGAMELOGINMODULE
