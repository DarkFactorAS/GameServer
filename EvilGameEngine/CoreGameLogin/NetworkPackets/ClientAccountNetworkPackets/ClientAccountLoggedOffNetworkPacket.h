#ifndef EVILENGINE_COREGAMELOGIN_NETWORKPACKETS_CLIENTACCOUNTLOGGEDOFFNETWORKPACKET
#define EVILENGINE_COREGAMELOGIN_NETWORKPACKETS_CLIENTACCOUNTLOGGEDOFFNETWORKPACKET 1

#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/Packets/BaseNetworkPacket.h"

class Account;

class ClientAccountLoggedOffNetworkPacket : public BaseNetworkPacket
{
public:

  ClientAccountLoggedOffNetworkPacket( const BinaryStream* datastream);
  ClientAccountLoggedOffNetworkPacket( uint32 accountId );
    
  static  BaseNetworkPacket*    Create( const BinaryStream* datastream ){ return new ClientAccountLoggedOffNetworkPacket( datastream ); }

  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;
  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("ClientAccountLoggedOff"); }
  virtual void                  Execute()         DF_OVERRIDE;
  
private:

  /// The account we want to log in with
  uint32                        m_AccountId;

};

#endif /// EVILENGINE_COREGAMELOGIN_NETWORKPACKETS_CLIENTACCOUNTLOGGEDOFFNETWORKPACKET