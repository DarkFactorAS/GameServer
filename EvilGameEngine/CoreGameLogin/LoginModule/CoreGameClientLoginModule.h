/*************************************************************************************
 * Project     : EvilEngine Multiplatform 2014
 * File        : EngineModule
 * Description : Abstract interface class for CoreGameEngine modules. This interface
 *               has to be implemented to be able to run as a sub component in the
 *               CoreGameEngine.
 *
 * Author      : Thor Richard Hansen
 *************************************************************************************/

#ifndef EVILENGINE_COREGAMELOGIN_COREGAMECLIENTLOGINMODULE
#define EVILENGINE_COREGAMELOGIN_COREGAMECLIENTLOGINMODULE 1

#include "EvilGameEngine/CoreGameLogin/LoginModule/CoreGameBaseLoginModule.h"
#include "EvilEngine/CoreEngine/CoreEngine/EngineModule.h"
#include "EvilEngine/CoreNetwork/Common/NetworkBase.h"

#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngineModule.h"

#include "EvilGameEngine/CoreGameLogin/Network/GameLoginPacketData.h"

#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ClientAccountNetworkPackets/ClientAccountLoggedOnNetworkPacket.h"

class CoreGameClientLoginModule : public CoreGameBaseLoginModule
{
public:
  
  CoreGameClientLoginModule(bool isLive);

  virtual bool                AddOnlineAccount(Account* account, uint32 connectionId) DF_OVERRIDE;
  virtual void                RemoveOnlineAccount(uint32 /*connectionId*/) DF_OVERRIDE;

  void                        SendSignalAccountLoggedOn( Account* account );

  Signal1<void, Account*>                       SignalAccountLoggedOn;
  Signal1<void, Account*>                       SignalCreatedAccount;
  Signal2<void, uint32, const String&>          SignalAccountError;

private:

  Account* m_ClientAccount;

};

#endif /// EVILENGINE_COREGAMELOGIN_COREGAMECLIENTLOGINMODULE
