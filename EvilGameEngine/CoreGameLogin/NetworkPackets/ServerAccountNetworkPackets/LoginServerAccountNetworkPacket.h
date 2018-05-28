#ifndef EVILENGINE_COREGAMELOGIN_NETWORKPACKETS_LOGINACCOUNTNETWORKPACKET
#define EVILENGINE_COREGAMELOGIN_NETWORKPACKETS_LOGINACCOUNTNETWORKPACKET 1

#include "EvilGameEngine/CoreGameLogin/Network/BaseLoginModuleNetworkPacket.h"

class Account;

class LoginServerAccountNetworkPacket : public BaseLoginModuleNetworkPacket
{
public:

  LoginServerAccountNetworkPacket( const BinaryStream* datastream);
  LoginServerAccountNetworkPacket( uint32 requiredVersion, const String& gameKey, Account* account);
    
  static  BaseNetworkPacket*    Create( const BinaryStream* datastream ){ return new LoginServerAccountNetworkPacket( datastream ); }

  virtual bool                  CanExecute()      DF_OVERRIDE;
  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;
  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("LoginAccount"); }
  virtual void                  Execute()         DF_OVERRIDE;
  
private:

  /// Minimum version the server must run to allow login
  uint32                        m_MinimumServerVersion;
  /// Must match the game key to be allowed to log in
  String                        m_GameKey;
  /// The account we want to log in with
  Account*                      m_Account;

};

#endif /// EVILENGINE_COREGAMELOGIN_NETWORKPACKETS_LOGINACCOUNTNETWORKPACKET