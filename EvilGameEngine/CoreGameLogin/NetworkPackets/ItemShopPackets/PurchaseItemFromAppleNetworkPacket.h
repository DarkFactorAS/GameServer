#ifndef EVILENGINE_COREGAMELOGIN_NETWORKPACKETS_ITEMSHOPPACKETS_PURCHASEITEMFROMAPPLENETWORKPACKET
#define EVILENGINE_COREGAMELOGIN_NETWORKPACKETS_ITEMSHOPPACKETS_PURCHASEITEMFROMAPPLENETWORKPACKET 1

#include "EvilGameEngine/CoreGameLogin/Network/BaseAuthentcatedNetworkPacket.h"

class PurchaseItemFromAppleNetworkPacket : public BaseAuthentcatedNetworkPacket
{
public:

  PurchaseItemFromAppleNetworkPacket( uint32 itemCode, const String& receipt);
  PurchaseItemFromAppleNetworkPacket( const BinaryStream* datastream);

  static  BaseNetworkPacket*    Create( const BinaryStream* datastream) { return new PurchaseItemFromAppleNetworkPacket(datastream); }

  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;
  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("PurchaseItemFromApple"); }
  virtual void                  Execute()         DF_OVERRIDE;

private:

  uint32 m_ItemCode;
  String m_Receipt;

};

#endif /// EVILENGINE_COREGAMELOGIN_NETWORKPACKETS_ITEMSHOPPACKETS_PURCHASEITEMFROMAPPLENETWORKPACKET