/*************************************************************************************************
 * Project     : EvilEngine Multi-platform 2014-2016
 * File        : CreateGameWithPlayerNetworkPacket
 *
 * Description : Player wants to create a new online game
 *
 * Author      : Thor Richard Hansen
 *************************************************************************************************/

#include "Precompile.h"
#include "LoadPlayfieldNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/Account/AccountManager.h"
#include "EvilGameEngine/CoreBoardGame/GameLogic/Game/BoardGame.h"
#include "EvilGameEngine/CoreBoardGame/GameLogic/Game/BoardGameServer.h"
#include "EvilGameEngine/CoreBoardGame/GameEngine/BoardGameEngine.h"
#include "EvilGameEngine/CoreBoardGame/Network/BoardGamePacketData.h"

#ifdef CLIENT

LoadPlayfieldNetworkPacket::LoadPlayfieldNetworkPacket(uint32 playfieldId) :
  BaseAuthentcatedNetworkPacket(BoardGamePacketData::PacketData_PlayfieldLoad),
  m_PlayfieldId( playfieldId ),
  m_Playfield(NULL)
{
}

LoadPlayfieldNetworkPacket::LoadPlayfieldNetworkPacket(const BinaryStream* datastream) :
  BaseAuthentcatedNetworkPacket(BoardGamePacketData::PacketData_PlayfieldLoad, datastream),
  m_Playfield(NULL)
{
  m_PlayfieldId = datastream->ReadUInt32();
}

BinaryStream* LoadPlayfieldNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseAuthentcatedNetworkPacket::GetDataStream();
  datastream->WriteUInt32(m_PlayfieldId);
  return datastream;
}

#else

LoadPlayfieldNetworkPacket::LoadPlayfieldNetworkPacket(GridTilePlayfield* playfield) :
  BaseAuthentcatedNetworkPacket(BoardGamePacketData::PacketData_PlayfieldLoad),
  m_PlayfieldId(0),
  m_Playfield(playfield)
{
}

LoadPlayfieldNetworkPacket::LoadPlayfieldNetworkPacket(const BinaryStream* datastream) :
  BaseAuthentcatedNetworkPacket(BoardGamePacketData::PacketData_PlayfieldLoad, datastream),
  m_PlayfieldId(0)
{
  m_Playfield = GridTilePlayfield::CreateFromData( datastream );
}

BinaryStream* LoadPlayfieldNetworkPacket::GetDataStream()
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
void LoadPlayfieldNetworkPacket::Execute()
{
#ifdef SERVER
  GridTilePlayfield* playfield = GridTilePlayfield::LoadFromDatabase(m_PlayfieldId);
  if (playfield != NULL)
  {
    SendPacketToClient( new LoadPlayfieldNetworkPacket( playfield ) );
  }
  SendErrorToClient( BoardGamePacketData::ErrorCode_ErrorWithPlayfield );
#endif
}
