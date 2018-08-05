#ifndef EVILENGINE_COREGAMELOGIN_NETWORKPACKETS_LOGINTOKENACCOUNTNETWORKPACKET
#define EVILENGINE_COREGAMELOGIN_NETWORKPACKETS_LOGINTOKENACCOUNTNETWORKPACKET 1

#include "EvilGameEngine/CoreGameLogin/Network/BaseLoginModuleNetworkPacket.h"

class LoginTokenServerAccountNetworkPacket : public BaseLoginModuleNetworkPacket
{
public:

  LoginTokenServerAccountNetworkPacket( const BinaryStream* datastream);
  LoginTokenServerAccountNetworkPacket( uint32 requiredVersion, const String& gameKey, const String& username, const String& token);
    
  static  BaseNetworkPacket*    Create( const BinaryStream* datastream ){ return new LoginTokenServerAccountNetworkPacket( datastream ); }

  virtual bool                  CanExecute()      DF_OVERRIDE;
  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;
  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("LoginTokenServerAccount"); }
  virtual void                  Execute()         DF_OVERRIDE;
  
private:

  /// Minimum version the server must run to allow login
  uint32                        m_MinimumServerVersion;
  /// Must match the game key to be allowed to log in
  String                        m_GameKey;
  /// The account we want to log in with
  String                        m_UserName;
  String                        m_Token;
};

#endif /// EVILENGINE_COREGAMELOGIN_NETWORKPACKETS_LOGINTOKENACCOUNTNETWORKPACKET