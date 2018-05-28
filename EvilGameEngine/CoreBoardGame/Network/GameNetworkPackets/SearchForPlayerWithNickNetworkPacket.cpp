/*************************************************************************************************
 * Project     : EvilEngine Multi-platform 2014-2016
 * File        : DeleteGameNetworkPacket
 *
 * Description : Player wants to leave an online game
 *
 * Author      : Thor Richard Hansen
 *************************************************************************************************/

#include "Precompile.h"
#include "SearchForPlayerWithNickNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/Account/AccountManager.h"
#include "EvilGameEngine/CoreBoardGame/GameLogic/Game/BoardGame.h"
#include "EvilGameEngine/CoreBoardGame/GameLogic/Game/BoardGameServer.h"
#include "EvilGameEngine/CoreBoardGame/GameEngine/BoardGameEngine.h"
#include "EvilGameEngine/CoreBoardGame/Network/BoardGamePacketData.h"

#ifdef CLIENT
  Signal1< void, std::vector< Account* > > SearchForPlayerWithNickNetworkPacket::SignalReceivedPlayerSearchResult;
#endif

#ifdef CLIENT
SearchForPlayerWithNickNetworkPacket::SearchForPlayerWithNickNetworkPacket( const String& playerNick ) :
  BaseAuthentcatedNetworkPacket( BoardGamePacketData::PacketData_SearchPlayer ),
  m_PlayerNick( playerNick )
{
}
#endif

#ifdef SERVER
SearchForPlayerWithNickNetworkPacket::SearchForPlayerWithNickNetworkPacket(std::vector< Account* > accountList) :
  BaseAuthentcatedNetworkPacket(BoardGamePacketData::PacketData_SearchPlayer),
  m_SearchResult( accountList )
{
}
#endif

SearchForPlayerWithNickNetworkPacket::SearchForPlayerWithNickNetworkPacket(const BinaryStream* datastream) :
  BaseAuthentcatedNetworkPacket(BoardGamePacketData::PacketData_SearchPlayer, datastream)
{
#ifdef CLIENT

  uint32 numResults = datastream->ReadUInt16();
  for ( uint16 index = 0; index < numResults; index++ )
  {
    Account* account = new Account();
    if (account->CreateFromStream(datastream))
    {
      m_SearchResult.push_back( account );
    }
  }

#else
  m_PlayerNick = datastream->ReadString();
#endif
}

BinaryStream* SearchForPlayerWithNickNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseAuthentcatedNetworkPacket::GetDataStream();

#ifdef CLIENT

  datastream->WriteString( m_PlayerNick );

#else

  datastream->WriteUInt16((uint16) m_SearchResult.size());
  for (std::vector< Account* >::iterator it = m_SearchResult.begin(); it != m_SearchResult.end(); ++it)
  {
    Account* account = *it;
    account->WriteData(datastream);
  }

#endif

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
void SearchForPlayerWithNickNetworkPacket::Execute()
{
#ifdef SERVER

  Account* thisAccount = GetAccount();
  std::vector< Account* > accountList = AccountManager::GetInstance()->GetAccountsWithNick(thisAccount, m_PlayerNick);
  SendPacketToClient( new SearchForPlayerWithNickNetworkPacket( accountList ) );

#else

  SignalReceivedPlayerSearchResult(m_SearchResult);

#endif
}

