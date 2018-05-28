#ifndef EVILENGINE_COREGAMELOGIN_NETWORKPACKETS_CREATESERVERACCOUNTNETWORKPACKET
#define EVILENGINE_COREGAMELOGIN_NETWORKPACKETS_CREATESERVERACCOUNTNETWORKPACKET 1

#include "EvilGameEngine/CoreGameLogin/Network/BaseLoginModuleNetworkPacket.h"

class Account;

class CreateServerAccountNetworkPacket : public BaseLoginModuleNetworkPacket
{
public:

  CreateServerAccountNetworkPacket( const BinaryStream* datastream);
  CreateServerAccountNetworkPacket(  uint32 requiredVersion, const String& gameKey, Account* account);
    
  static  BaseNetworkPacket*          Create( const BinaryStream* datastream ){ return new CreateServerAccountNetworkPacket( datastream ); }

  virtual BinaryStream*               GetDataStream()   DF_OVERRIDE;
  virtual String                      GetPacketName()   DF_OVERRIDE { return StaticStr("CreateAccount"); }
  virtual void                        Execute()         DF_OVERRIDE;
  virtual bool                        CanExecute()      DF_OVERRIDE;

private:

  /// Minimum versio the server must run to allow login
  uint32                        m_MinimumServerVersion;
  /// Must match the gamekey to be allowed to log in
  String                        m_GameKey;
  /// The account we want to log in with
  Account*                      m_Account;

};

#endif /// EVILENGINE_COREGAMELOGIN_NETWORKPACKETS_CREATESERVERACCOUNTNETWORKPACKET