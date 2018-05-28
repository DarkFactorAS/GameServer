#ifndef EVILENGINE_COREGAMELOGIN_NETWORKPACKETS_ITEMSHOPPACKETS_PURCHASEITEMFROMDARKFACTORNETWORKPACKET
#define EVILENGINE_COREGAMELOGIN_NETWORKPACKETS_ITEMSHOPPACKETS_PURCHASEITEMFROMDARKFACTORNETWORKPACKET 1

#include "EvilGameEngine/CoreGameLogin/Network/BaseAuthentcatedNetworkPacket.h"

class PurchaseItemFromDarkFactorNetworkPacket : public BaseAuthentcatedNetworkPacket
{
public:

  PurchaseItemFromDarkFactorNetworkPacket( uint32 itemCode);
  PurchaseItemFromDarkFactorNetworkPacket( const BinaryStream* datastream);

  static  BaseNetworkPacket*    Create( const BinaryStream* datastream) { return new PurchaseItemFromDarkFactorNetworkPacket(datastream); }

  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;
  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("PurchaseItemFromApple"); }
  virtual void                  Execute()         DF_OVERRIDE;

private:

  uint32 m_ItemCode;

};

#endif /// EVILENGINE_COREGAMELOGIN_NETWORKPACKETS_ITEMSHOPPACKETS_PURCHASEITEMFROMDARKFACTORNETWORKPACKET