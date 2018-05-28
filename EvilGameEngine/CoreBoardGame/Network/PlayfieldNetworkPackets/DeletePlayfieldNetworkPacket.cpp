/*************************************************************************************************
 * Project     : EvilEngine Multi-platform 2014-2016
 * File        : CreateGameWithPlayerNetworkPacket
 *
 * Description : Player wants to create a new online game
 *
 * Author      : Thor Richard Hansen
 *************************************************************************************************/

#include "Precompile.h"
#include "DeletePlayfieldNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/Account/AccountManager.h"
#include "EvilGameEngine/CoreBoardGame/GameLogic/Game/BoardGame.h"
#include "EvilGameEngine/CoreBoardGame/GameLogic/Game/BoardGameServer.h"
#include "EvilGameEngine/CoreBoardGame/GameEngine/BoardGameEngine.h"
#include "EvilGameEngine/CoreBoardGame/Network/BoardGamePacketData.h"

DeletePlayfieldNetworkPacket::DeletePlayfieldNetworkPacket(uint32 playfieldId) :
  BaseAuthentcatedNetworkPacket(BoardGamePacketData::PacketData_PlayfieldDelete),
  m_PlayfieldId( playfieldId )
{
}

DeletePlayfieldNetworkPacket::DeletePlayfieldNetworkPacket(const BinaryStream* datastream) :
  BaseAuthentcatedNetworkPacket(BoardGamePacketData::PacketData_PlayfieldDelete, datastream)
{
  m_PlayfieldId = datastream->ReadUInt32();
}

BinaryStream* DeletePlayfieldNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseAuthentcatedNetworkPacket::GetDataStream();
  datastream->WriteUInt32(m_PlayfieldId);
  return datastream;
}

/************************************************************************************************
 * Execute:
 * Executes the gamecode on the receiver when receiving this packet. Leave the game and make sure 
 * the other player wins the game
 *
 * @param  (CoreGameEngine&)  engine          - Reference to the engine running this packet
 * @param  (uint32)           networkInstance - ID of the network connection that received this packet
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
void DeletePlayfieldNetworkPacket::Execute()
{
#ifdef SERVER

  Account* account = GetAccount();
  if (account != NULL)
  {
    if (GridTilePlayfield::DeleteFromDatabase(account->GetAccountId(), m_PlayfieldId))
    {
      SendPacketToClient( new DeletePlayfieldNetworkPacket( m_PlayfieldId ) );
      return;
    }
  }
  SendErrorToClient( BoardGamePacketData::ErrorCode_ErrorWithPlayfield );

#endif
}
