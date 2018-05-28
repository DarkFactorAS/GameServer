/*************************************************************************************************
* Project     : EvilEngine Multi-platform 2014-2016
* File        : PurchaseItemFromAmazonNetworkPacket
*
* Description : Network packet to purchase an item from the Amazon item store.
*
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "PurchaseItemFromDarkfactorNetworkPacket.h"
#include "EvilEngine/CoreLib/BasicTypes/Definitions/VolatileArgs.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/GameEnginePacketData.h"
#include "EvilGameEngine/CoreGameLogin/Account/AccountManager.h"
#include "EvilGameEngine/CoreGameLogin/Network/GameLoginPacketData.h"
#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ServerAccountNetworkPackets/UpdatedAccountFlagsNetworkPacket.h"

PurchaseItemFromDarkFactorNetworkPacket::PurchaseItemFromDarkFactorNetworkPacket(uint32 itemCode) :
  BaseAuthentcatedNetworkPacket(GameLoginPacketData::PacketData_WindowsItemPurchase),
  m_ItemCode(itemCode)
{
}

PurchaseItemFromDarkFactorNetworkPacket::PurchaseItemFromDarkFactorNetworkPacket( const BinaryStream* datastream) :
  BaseAuthentcatedNetworkPacket( GameLoginPacketData::PacketData_WindowsItemPurchase, datastream )
{
  m_ItemCode  = datastream->ReadUInt32();
}

BinaryStream* PurchaseItemFromDarkFactorNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseNetworkPacket::GetDataStream();
  datastream->WriteUInt32( m_ItemCode );
  return datastream;
}

void PurchaseItemFromDarkFactorNetworkPacket::Execute()
{
#ifdef SERVER

  AccountManager* accountManager = AccountManager::GetInstance();
  if ( accountManager != NULL )
  {
    Account* account = GetAccount();
    if (account != NULL)
    {
      if (accountManager->UpdateMembership(account, m_ItemCode) )
      {
        SendPacketToClient( new UpdatedAccountFlagsNetworkPacket( account->GetPurchaseFlags() ) );
        return;
      }
    }
  }
  SendErrorToClient(GameLoginPacketData::ErrorCode_CodeError);

#endif

}
