/*************************************************************************************************
 * Project     : EvilEngine Multi-platform 2014-2016
 * File        : CreateGameWithPlayerNetworkPacket
 *
 * Description : Player wants to create a new online game
 *
 * Author      : Thor Richard Hansen
 *************************************************************************************************/

#include "Precompile.h"
#include "SavePlayfieldNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/Account/AccountManager.h"
#include "EvilGameEngine/CoreBoardGame/GameLogic/Game/BoardGame.h"
#include "EvilGameEngine/CoreBoardGame/GameLogic/Game/BoardGameServer.h"
#include "EvilGameEngine/CoreBoardGame/GameEngine/BoardGameEngine.h"
#include "EvilGameEngine/CoreBoardGame/Network/BoardGamePacketData.h"

#ifdef SERVER

SavePlayfieldNetworkPacket::SavePlayfieldNetworkPacket(uint32 playfieldId) :
  BaseAuthentcatedNetworkPacket(BoardGamePacketData::PacketData_PlayfieldSave),
  m_PlayfieldId( playfieldId ),
  m_Playfield(NULL)
{
}

SavePlayfieldNetworkPacket::SavePlayfieldNetworkPacket(const BinaryStream* datastream) :
  BaseAuthentcatedNetworkPacket(BoardGamePacketData::PacketData_PlayfieldSave, datastream),
  m_Playfield(NULL)
{
  m_PlayfieldId = datastream->ReadUInt32();
}

BinaryStream* SavePlayfieldNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseAuthentcatedNetworkPacket::GetDataStream();
  datastream->WriteUInt32(m_PlayfieldId);
  return datastream;
}

#else

SavePlayfieldNetworkPacket::SavePlayfieldNetworkPacket(GridTilePlayfield* playfield) :
  BaseAuthentcatedNetworkPacket(BoardGamePacketData::PacketData_PlayfieldLoad),
  m_PlayfieldId(0),
  m_Playfield(playfield)
{
}

SavePlayfieldNetworkPacket::SavePlayfieldNetworkPacket(const BinaryStream* datastream) :
  BaseAuthentcatedNetworkPacket(BoardGamePacketData::PacketData_PlayfieldLoad, datastream),
  m_PlayfieldId(0)
{
  m_Playfield = GridTilePlayfield::CreateFromData( datastream );
}

BinaryStream* SavePlayfieldNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseAuthentcatedNetworkPacket::GetDataStream();
  m_Playfield->WriteData(*datastream);
  return datastream;
}

#endif



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
void SavePlayfieldNetworkPacket::Execute()
{
#ifdef SERVER

  Account* account = GetAccount();
  if (m_Playfield != NULL && account != NULL)
  {
    // TODO: Check owner
    m_Playfield->SetOwnerId(account->GetAccountId());
    if (m_Playfield->SaveToDatabase())
    {
      SendPacketToClient( new SavePlayfieldNetworkPacket( m_Playfield->GetPlayfieldId() ) );
      return;
    }
  }
  SendErrorToClient( BoardGamePacketData::ErrorCode_ErrorWithPlayfield );

#endif
}
