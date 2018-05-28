/*************************************************************************************************
 * Project     : EvilEngine Multi-platform 2014-2016
 * File        : CreateGameWithPlayerNetworkPacket
 *
 * Description : Player wants to create a new online game
 *
 * Author      : Thor Richard Hansen
 *************************************************************************************************/

#include "Precompile.h"
#include "PlayfieldListNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/Account/AccountManager.h"
#include "EvilGameEngine/CoreBoardGame/GameLogic/Game/BoardGame.h"
#include "EvilGameEngine/CoreBoardGame/GameLogic/Game/BoardGameServer.h"
#include "EvilGameEngine/CoreBoardGame/GameEngine/BoardGameEngine.h"
#include "EvilGameEngine/CoreBoardGame/Network/BoardGamePacketData.h"
#include "EvilGameEngine/CoreGameLogin/Network/GameLoginPacketData.h"

#ifdef CLIENT

Signal1< void, PlayfieldInfoMap /* plauyfieldList */ > PlayfieldListNetworkPacket::s_SignalPlayfieldList;

PlayfieldListNetworkPacket::PlayfieldListNetworkPacket() :
  BaseAuthentcatedNetworkPacket(BoardGamePacketData::PacketData_PlayfieldList)
{
}

PlayfieldListNetworkPacket::PlayfieldListNetworkPacket(const BinaryStream* datastream) :
  BaseAuthentcatedNetworkPacket(BoardGamePacketData::PacketData_PlayfieldList, datastream)
{
}

#else

PlayfieldListNetworkPacket::PlayfieldListNetworkPacket(const BinaryStream* datastream) :
  BaseAuthentcatedNetworkPacket(BoardGamePacketData::PacketData_PlayfieldList, datastream)
{
}

PlayfieldListNetworkPacket::PlayfieldListNetworkPacket( PlayfieldInfoMap playfieldList ) :
  BaseAuthentcatedNetworkPacket(BoardGamePacketData::PacketData_PlayfieldList)
{
}

BinaryStream* PlayfieldListNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseAuthentcatedNetworkPacket::GetDataStream();

  datastream->WriteUInt16((uint16) m_PlayfieldList.size());
  for (PlayfieldInfoMap::const_iterator it = m_PlayfieldList.begin(); it != m_PlayfieldList.end(); ++it)
  {
    const PlayfieldInfo* info = it->second;
    info->WriteData(*datastream);
  }

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
void PlayfieldListNetworkPacket::Execute()
{
#ifdef SERVER

  Account* account = GetAccount();
  if (account != NULL)
  {
    PlayfieldInfoMap infoMap = GridTilePlayfield::LoadPlayfieldListFromDatabase(account->GetAccountId());
    SendPacketToClient( new PlayfieldListNetworkPacket( infoMap ) );
    return;
  }
  SendErrorToClient( GameLoginPacketData::ErrorCode_NotLoggedIn );

#endif
}
