#ifndef EVILENGINE_COREGAMELOGIN_NETWORK_BASEAUTHENTICATEDNETWORKPACKET
#define EVILENGINE_COREGAMELOGIN_NETWORK_BASEAUTHENTICATEDNETWORKPACKET 1

#include "EvilEngine/CoreLib/BasicTypes/BinaryStream/BinaryStream.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/Packets/BaseNetworkPacket.h"

#include "BaseLoginModuleNetworkPacket.h"

class Account;

class BaseAuthentcatedNetworkPacket : public BaseLoginModuleNetworkPacket
{
public:

  BaseAuthentcatedNetworkPacket( uint32 packetType, const BinaryStream* datastream );
  BaseAuthentcatedNetworkPacket( uint32 packetType );
    
  virtual String                GetPacketName() = 0;

#ifdef SERVER
  virtual bool                  CanExecute() DF_OVERRIDE;
#endif

protected:

  Account*                      GetAccount();
  bool                          IsSenderGM();
  Account*                      m_Account;
};

#endif /// EVILENGINE_COREGAMELOGIN_NETWORK_BASEAUTHENTICATEDNETWORKPACKET