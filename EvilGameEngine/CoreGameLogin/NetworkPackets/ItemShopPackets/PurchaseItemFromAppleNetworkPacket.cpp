/*************************************************************************************************
* Project     : EvilEngine Multi-platform 2014-2016
* File        : PurchaseItemFromAmazonNetworkPacket
*
* Description : Network packet to purchase an item from the Amazon item store.
*
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "PurchaseItemFromAppleNetworkPacket.h"
#include "EvilEngine/CoreLib/BasicTypes/Definitions/VolatileArgs.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/GameEnginePacketData.h"
#include "EvilGameEngine/CoreGameLogin/Account/AccountManager.h"
//#include "EvilGameEngine/CoreGameLogin/Account/GMFlags.h"
#include "EvilGameEngine/CoreGameLogin/Network/GameLoginPacketData.h"

PurchaseItemFromAppleNetworkPacket::PurchaseItemFromAppleNetworkPacket(uint32 itemCode, const String& receipt) :
  BaseAuthentcatedNetworkPacket(GameLoginPacketData::PacketData_IOSItemPurchase),
  m_ItemCode(itemCode),
  m_Receipt(receipt)
{
}

PurchaseItemFromAppleNetworkPacket::PurchaseItemFromAppleNetworkPacket( const BinaryStream* datastream) :
  BaseAuthentcatedNetworkPacket( GameLoginPacketData::PacketData_IOSItemPurchase, datastream )
{
  m_ItemCode  = datastream->ReadUInt32();
  m_Receipt   = datastream->ReadString();
}

BinaryStream* PurchaseItemFromAppleNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseNetworkPacket::GetDataStream();
  datastream->WriteUInt32( m_ItemCode );
  datastream->WriteString( m_Receipt );
  return datastream;
}

void PurchaseItemFromAppleNetworkPacket::Execute()
{
#ifdef SERVER

  /*
  Account* account = gameConnection->GetAccount();
  if ( account != NULL )
  {
  uint32 itemCode = dataStream->ReadUInt32();
  if ( !UpdateMembership( account, itemCode ) )
  {
  CoreGameNetworkCommon::SendError( gameConnection, methodId, GameLoginPacketData::ErrorCode_CodeError );
  }
  }

  */

#endif
}
