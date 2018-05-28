#ifndef EVILENGINE_COREGAMELOGIN_NETWORKPACKETS_SWITCHLOGGEDINACCOUNTNETWORKPACKET
#define EVILENGINE_COREGAMELOGIN_NETWORKPACKETS_SWITCHLOGGEDINACCOUNTNETWORKPACKET 1

#include "EvilGameEngine/CoreGameLogin/Network/BaseAuthentcatedNetworkPacket.h"

class Account;

class SwitchLoggedInAccountNetworkPacket : public BaseAuthentcatedNetworkPacket
{
public:

  SwitchLoggedInAccountNetworkPacket( const BinaryStream* datastream);
#ifdef CLIENT
  SwitchLoggedInAccountNetworkPacket( const String& username);
#endif
#ifdef SERVER
  SwitchLoggedInAccountNetworkPacket( Account* account );
#endif
    
  static  BaseNetworkPacket*    Create( const BinaryStream* datastream ){ return new SwitchLoggedInAccountNetworkPacket( datastream ); }

  virtual BinaryStream*         GetDataStream()                 DF_OVERRIDE;
  virtual String                GetPacketName()                 DF_OVERRIDE { return StaticStr("SwitchLoggedInAccount"); }
  virtual void                  Execute()                       DF_OVERRIDE;
  
private:

  Account*  m_Account;
  String    m_Username;

};

#endif /// EVILENGINE_COREGAMELOGIN_NETWORKPACKETS_SWITCHLOGGEDINACCOUNTNETWORKPACKET