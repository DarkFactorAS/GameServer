
#include "Precompile.h"
#include "UpdatedAccountFlagsNetworkPacket.h"
#include "EvilEngine/CoreLib/BasicTypes/Definitions/VolatileArgs.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/GameEnginePacketData.h"
#include "EvilGameEngine/CoreGameLogin/Account/AccountManager.h"
#include "EvilGameEngine/CoreGameLogin/Account/GMFlags.h"
#include "EvilGameEngine/CoreGameLogin/Network/GameLoginPacketData.h"

UpdatedAccountFlagsNetworkPacket::UpdatedAccountFlagsNetworkPacket( const BinaryStream* datastream) :
  BaseAuthentcatedNetworkPacket( GameLoginPacketData::PacketData_UpdatedAccountFlags, datastream )
{
  m_AccountFlags = datastream->ReadUInt32();
}

UpdatedAccountFlagsNetworkPacket::UpdatedAccountFlagsNetworkPacket(uint32 accountFlags) :
  BaseAuthentcatedNetworkPacket( GameLoginPacketData::PacketData_UpdatedAccountFlags ),
  m_AccountFlags( accountFlags )
{
}

BinaryStream* UpdatedAccountFlagsNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseNetworkPacket::GetDataStream();
  datastream->WriteUInt32( m_AccountFlags );
  return datastream;
}

void UpdatedAccountFlagsNetworkPacket::Execute()
{
#ifdef CLIENT

  Account* account = Account::GetActiveAccount();
  if (account != NULL)
  {
    account->SetPurchaseFlags( m_AccountFlags );
  }
  
#endif
}
