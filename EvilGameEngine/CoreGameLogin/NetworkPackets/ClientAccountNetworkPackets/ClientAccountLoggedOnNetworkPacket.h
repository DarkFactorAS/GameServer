#ifndef EVILENGINE_COREGAMELOGIN_NETWORKPACKETS_CLIENTACCOUNTLOGGEDONNETWORKPACKET
#define EVILENGINE_COREGAMELOGIN_NETWORKPACKETS_CLIENTACCOUNTLOGGEDONNETWORKPACKET 1

#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/Packets/BaseNetworkPacket.h"

class Account;

class ClientAccountLoggedOnNetworkPacket : public BaseNetworkPacket
{
public:

  ClientAccountLoggedOnNetworkPacket( const BinaryStream* datastream);
  ClientAccountLoggedOnNetworkPacket( Account* account);
    
  static  BaseNetworkPacket*    Create( const BinaryStream* datastream ){ return new ClientAccountLoggedOnNetworkPacket( datastream ); }

  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;
  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("ClientAccountLoggedOn"); }
  virtual void                  Execute()         DF_OVERRIDE;
  
private:

  /// The account we want to log in with
  Account*                      m_Account;

};

#endif /// EVILENGINE_COREGAMELOGIN_NETWORKPACKETS_CLIENTACCOUNTLOGGEDONNETWORKPACKET