/*************************************************************************************************
 * Project     : EvilEngine Multi-platform 2014-2016
 * File        : LoginGMNetworkPacket
 *
 * Description : Network packet to log in the player as a Game Master on the server. Beeing a 
 *               Game Master gives the player special access rights.
 *
 * Author      : Thor Richard Hansen
 *************************************************************************************************/

#include "Precompile.h"
#include "LoginGMNetworkPacket.h"
#include "UpdatedAccountFlagsNetworkPacket.h"
#include "EvilEngine/CoreLib/BasicTypes/Definitions/VolatileArgs.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/GameEnginePacketData.h"
#include "EvilGameEngine/CoreGameLogin/Account/AccountManager.h"
#include "EvilGameEngine/CoreGameLogin/Account/GMFlags.h"
#include "EvilGameEngine/CoreGameLogin/Network/GameLoginPacketData.h"
#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ServerAccountNetworkPackets/UpdatedAccountFlagsNetworkPacket.h"

LoginGMNetworkPacket::LoginGMNetworkPacket( const BinaryStream* datastream) :
  BaseAuthentcatedNetworkPacket( GameLoginPacketData::PacketData_LoginGM, datastream )
{
  m_Password = datastream->ReadString();
}

LoginGMNetworkPacket::LoginGMNetworkPacket( String password) :
  BaseAuthentcatedNetworkPacket( GameLoginPacketData::PacketData_LoginGM ),
  m_Password( password )
{
}

BinaryStream* LoginGMNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseNetworkPacket::GetDataStream();
  datastream->WriteString( m_Password );
  return datastream;
}

void LoginGMNetworkPacket::Execute()
{
#ifdef SERVER
  AccountManager* accountManager = AccountManager::GetInstance();
  if ( accountManager != NULL )
  {
    Account* account = GetAccount();
    if ( account != NULL && account->EnableGM(m_Password) )
    {
      if (accountManager->UpdateMembership(account, GM_FLAGS::GMFLAG))
      {
        SendPacketToClient( new UpdatedAccountFlagsNetworkPacket( account->GetPurchaseFlags() ) );
      }
      else
      {
        SendErrorToClient(GameLoginPacketData::ErrorCode_CodeError);
      }
    }
    else
    {
      SendErrorToClient( GameLoginPacketData::ErrorCode_WrongPassword );
    }
  }
#endif
}
