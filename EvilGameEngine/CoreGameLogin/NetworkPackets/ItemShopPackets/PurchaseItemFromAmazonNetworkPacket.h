#ifndef EVILENGINE_COREGAMELOGIN_NETWORKPACKETS_ITEMSHOPPACKETS_PURCHASEITEMFROMAMAZONNETWORKPACKET
#define EVILENGINE_COREGAMELOGIN_NETWORKPACKETS_ITEMSHOPPACKETS_PURCHASEITEMFROMAMAZONNETWORKPACKET 1

#include "EvilGameEngine/CoreGameLogin/Network/BaseAuthentcatedNetworkPacket.h"

class PurchaseItemFromAmazonNetworkPacket : public BaseAuthentcatedNetworkPacket
{
public:

  PurchaseItemFromAmazonNetworkPacket( uint32 itemCode, const String& receiptId, const String& receipt);
  PurchaseItemFromAmazonNetworkPacket( const BinaryStream* datastream);

  static  BaseNetworkPacket*    Create( const BinaryStream* datastream ){ return new PurchaseItemFromAmazonNetworkPacket( datastream ); }

  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;
  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("PurchaseItemFromAmazon"); }
  virtual void                  Execute()         DF_OVERRIDE;

private:

  uint32 m_ItemCode;
  String m_ReceiptId;
  String m_Receipt;

};

#endif /// EVILENGINE_COREGAMELOGIN_NETWORKPACKETS_ITEMSHOPPACKETS_PURCHASEITEMFROMAMAZONNETWORKPACKET