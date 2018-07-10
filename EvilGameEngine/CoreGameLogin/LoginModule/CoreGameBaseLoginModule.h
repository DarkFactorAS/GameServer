/*************************************************************************************
 * Project     : EvilEngine Multiplatform 2014
 * File        : EngineModule
 * Description : Abstract interface class for CoreGameEngine modules. This interface
 *               has to be implemented to be able to run as a sub component in the
 *               CoreGameEngine.
 *
 * Author      : Thor Richard Hansen
 *************************************************************************************/

#ifndef EVILENGINE_COREGAMELOGIN_COREGAMEBASELOGINMODULE
#define EVILENGINE_COREGAMELOGIN_COREGAMEBASELOGINMODULE 1

#include "EvilEngine/CoreEngine/CoreEngine/EngineModule.h"
#include "EvilEngine/CoreNetwork/Common/NetworkBase.h"

#include "EvilEngine/CoreLib/BasicTypes/Definitions/VolatileArgs.h"

#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngineModule.h"

#include "EvilGameEngine/CoreGameLogin/Account/AccountManager.h"
#include "EvilGameEngine/CoreGameLogin/Network/GameLoginPacketData.h"

#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ClientAccountNetworkPackets/ClientAccountLoggedOnNetworkPacket.h"

#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ServerAccountNetworkPackets/CreateServerAccountNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ServerAccountNetworkPackets/LoginServerAccountNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ServerAccountNetworkPackets/LoginGMNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ServerAccountNetworkPackets/UpdatedAccountFlagsNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ServerAccountNetworkPackets/SwitchLoggedInAccountNetworkPacket.h"

#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ItemShopPackets/PurchaseItemFromAmazonNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ItemShopPackets/PurchaseItemFromAppleNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ItemShopPackets/PurchaseItemFromDarkfactorNetworkPacket.h"

class CoreGameBaseLoginModule : public CoreGameEngineModule
{
public:
  
  CoreGameBaseLoginModule(int moduleId, bool isLive);
  
  virtual void                FrameProcess( float /*deltaTime*/ ){};

  virtual bool                AddOnlineAccount(Account* /*account*/, uint32 /*connectionId*/) = 0;
  virtual void                RemoveOnlineAccount(uint32 /*connectionId*/) = 0;

  const String&               GetLoginError(uint32 errorCode);

protected:

  void                        AddLoginError(uint32 errorCode, const String& errorDescription );
  
  std::map<uint32, String>    m_ErrorMessages;
};

#endif /// EVILENGINE_COREGAMELOGIN_COREGAMEBASELOGINMODULE
