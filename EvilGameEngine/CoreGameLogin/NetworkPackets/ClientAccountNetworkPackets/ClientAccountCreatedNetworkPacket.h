#ifndef EVILENGINE_COREGAMELOGIN_NETWORKPACKETS_CLIENTACCOUNTCREATEDNETWORKPACKET
#define EVILENGINE_COREGAMELOGIN_NETWORKPACKETS_CLIENTACCOUNTCREATEDNETWORKPACKET 1

#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/Packets/BaseNetworkPacket.h"

class Account;

class ClientAccountCreatedNetworkPacket : public BaseNetworkPacket
{
public:

  ClientAccountCreatedNetworkPacket( const BinaryStream* datastream);
  ClientAccountCreatedNetworkPacket( Account* account);
    
  static  BaseNetworkPacket*    Create( const BinaryStream* datastream ){ return new ClientAccountCreatedNetworkPacket( datastream ); }

  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;
  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("ClientAccountCreated"); }
  virtual void                  Execute()         DF_OVERRIDE;
  
private:

  Account*                      m_Account;

};

#endif /// EVILENGINE_COREGAMELOGIN_NETWORKPACKETS_CLIENTACCOUNTCREATEDNETWORKPACKET