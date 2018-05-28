/*************************************************************************************************
* Project     : EvilEngine Multi-platform 2014-2016
* File        : PurchaseItemFromAmazonNetworkPacket
*
* Description : Network packet to purchase an item from the Amazon item store.
*
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "PurchaseItemFromAmazonNetworkPacket.h"
#include "EvilEngine/CoreLib/BasicTypes/Definitions/VolatileArgs.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/GameEnginePacketData.h"
#include "EvilGameEngine/CoreGameLogin/Account/AccountManager.h"
//#include "EvilGameEngine/CoreGameLogin/Account/GMFlags.h"
#include "EvilGameEngine/CoreGameLogin/Network/GameLoginPacketData.h"

PurchaseItemFromAmazonNetworkPacket::PurchaseItemFromAmazonNetworkPacket(uint32 itemCode, const String& receiptId, const String& receipt) :
  BaseAuthentcatedNetworkPacket(GameLoginPacketData::PacketData_AmazonItemPurchase),
  m_ItemCode(itemCode),
  m_ReceiptId(receiptId),
  m_Receipt(receipt)
{
}

PurchaseItemFromAmazonNetworkPacket::PurchaseItemFromAmazonNetworkPacket( const BinaryStream* datastream) :
  BaseAuthentcatedNetworkPacket( GameLoginPacketData::PacketData_AmazonItemPurchase, datastream )
{
  m_ItemCode  = datastream->ReadUInt32();
  m_ReceiptId = datastream->ReadString();
  m_Receipt   = datastream->ReadString();
}

BinaryStream* PurchaseItemFromAmazonNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseNetworkPacket::GetDataStream();
  datastream->WriteUInt32( m_ItemCode );
  datastream->WriteString( m_ReceiptId );
  datastream->WriteString( m_Receipt );
  return datastream;
}

void PurchaseItemFromAmazonNetworkPacket::Execute()
{
#ifdef SERVER

/*
  Account* account = gameConnection->GetAccount();
  if (account != NULL)
  {
    uint32 itemCode = dataStream->ReadUInt32();
    String receiptId = dataStream->ReadString();
    String receipt = dataStream->ReadString();
    if (!dataStream->IsOK())
    {
      CoreGameNetworkCommon::SendError(gameConnection, methodId, GameLoginPacketData::ErrorCode_CodeError);
      return;
    }

    // 1. See if we already have the receipt in the system
    uint32 accountId = GetReceiptWithId(receiptId);

    // If we already have the receipt and it is not on our account -> Send a message
    // to the client to ignore this and send us the next purchase instead.
    if (accountId != 0 && accountId != account->GetAccountId())
    {
      BinaryStream* outData = CoreGameNetworkCommon::CreatePacket(GameLoginPacketData::PacketData_AmazonPurchaseIgnore);
      outData->WriteString(receiptId);
      gameConnection->SendData(outData);
      return;
    }

    // If the receipt is on me, update my membership and notify client to close the purchase
    if (accountId != 0 && accountId == account->GetAccountId())
    {
      UpdateMembership(account, itemCode);

      BinaryStream* outData = CoreGameNetworkCommon::CreatePacket(GameLoginPacketData::PacketData_AmazonItemPurchase);
      outData->WriteString(receiptId);
      gameConnection->SendData(outData);
      return;
    }

    // If the receipt doesn't already exist, but I already have the product ( and it is not recurring )
    if (accountId == 0 && account->HasPurchased(itemCode))
    {
      BinaryStream* outData = CoreGameNetworkCommon::CreatePacket(GameLoginPacketData::PacketData_AmazonPurchaseFailed);
      outData->WriteString(receiptId);
      gameConnection->SendData(outData);
      return;
    }

    // New purchase. Update the membership, add the receipt and notify client to close the purchase
    if (UpdateMembership(account, itemCode))
    {
      if (AddReceipt(account, itemCode, receiptId, receipt))
      {
        BinaryStream* outData = CoreGameNetworkCommon::CreatePacket(GameLoginPacketData::PacketData_AmazonItemPurchase);
        outData->WriteString(receiptId);
        gameConnection->SendData(outData);
        return;
      }
    }

    // Generic error
    CoreGameNetworkCommon::SendError(gameConnection, methodId, GameLoginPacketData::ErrorCode_CodeError);
  }
  
  */

#endif
}
