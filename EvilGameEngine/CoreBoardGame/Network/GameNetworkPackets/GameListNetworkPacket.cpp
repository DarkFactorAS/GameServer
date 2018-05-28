/*************************************************************************************************
 * Project     : EvilEngine Multi-platform 2014-2016
 * File        : GameListNetworkPacket
 *
 * Description : Send a list of all active online games for a player
 *
 * Author      : Thor Richard Hansen
 *************************************************************************************************/

#include "Precompile.h"
#include "GameListNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/Account/AccountManager.h"
#include "EvilGameEngine/CoreBoardGame/GameLogic/Game/BoardGame.h"
#include "EvilGameEngine/CoreBoardGame/GameLogic/Game/BoardGameServer.h"
#include "EvilGameEngine/CoreBoardGame/GameEngine/BoardGameEngine.h"
#include "EvilGameEngine/CoreBoardGame/Network/BoardGamePacketData.h"

Signal1< void, std::vector< BoardGame* >& > GameListNetworkPacket::s_SignalGameList;

GameListNetworkPacket::GameListNetworkPacket() :
  BaseAuthentcatedNetworkPacket( BoardGamePacketData::PacketData_GameList )
{
}

GameListNetworkPacket::GameListNetworkPacket( const BinaryStream* datastream) :
  BaseAuthentcatedNetworkPacket( BoardGamePacketData::PacketData_GameList, datastream )
{
}

GameListNetworkPacket::GameListNetworkPacket( GameList& gameList ) :
  BaseAuthentcatedNetworkPacket( BoardGamePacketData::PacketData_GameList )
{
  m_GameList = gameList;
}

/************************************************************************************************
 * AccountLoggedIn:
 * Received a newly created or logged in Account. Set the account as active and notity client
 *
 * @param  (BinaryStream)     dataStream  - The datastream with the account
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
BinaryStream* GameListNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseAuthentcatedNetworkPacket::GetDataStream();

#ifdef SERVER

  datastream->WriteUInt16( (uint16) m_GameList.size() );
  for ( std::vector< BoardGame* >::iterator it = m_GameList.begin(); it != m_GameList.end(); ++it )
  {
    BoardGame* game = *it;
    game->WriteInfo( *datastream );
  }

#endif
  
  return datastream;
}

BoardGame* GameListNetworkPacket::CreateGameClass(const BinaryStream* datastream)
{
  return BoardGame::CreateGameFromData(datastream);
}

bool GameListNetworkPacket::ReadDataStream( const BinaryStream* datastream )
{
#ifdef CLIENT
  uint16 numGames = datastream->ReadUInt16();
  for (uint16 index = 0; index < numGames; index++)
  {
    BoardGame* boardGame = CreateGameClass(datastream);
    if (boardGame != NULL)
    {
      m_GameList.push_back(boardGame);
    }
    else
    {
      return false;
    }
  }
#else
  VOLATILE_ARG(datastream);
#endif
  return true;
}

/************************************************************************************************
* GetGame:
* Helper function to get the game object for this game network packet
*
* @param  (CoreGameEngine&)     engine          - Pointer to the game engine
* @return (BoardGame*)                          - Return the game this packet belongs to
*
* @author Thor Richard Hansen
*************************************************************************************************/
BoardGameServer* GameListNetworkPacket::GetGameServer(CoreGameEngine& engine)
{
  BoardGameEngine* boardGameEngine = safe_cast<BoardGameEngine*> (&engine);
  if (boardGameEngine != NULL)
  {
    BoardGameServer* boardGameServer = boardGameEngine->GetBoardGameServer();
    return boardGameServer;
  }
  return NULL;
}

/************************************************************************************************
 * Execute:
 * Executes the gamecode on the receiver when receiving this packet. On the server this will fetch
 * all the active games for the player and send to the client.
 *
 * @param  (CoreGameEngine&)  engine          - Reference to the engine running this packet
 * @param  (uint32)           networkInstance - ID of the network connection that received this packet
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
void GameListNetworkPacket::Execute()
{
  BoardGameEngine* gameEngine = safe_cast<BoardGameEngine*> (GetGameEngine());
  if (gameEngine != NULL)
  {
    BoardGameServer* gameServer = gameEngine->GetGameServer();
    if ( gameServer != NULL )
    {
  #ifdef SERVER
      
      GameList gameList = gameServer->GetActiveGamesForAccount( m_Account->GetAccountId() );
      SendPacketToClient( new GameListNetworkPacket( gameList ) );
      
  #else
      
      GameListSorter sorter;
      std::sort( m_GameList.begin(), m_GameList.end(), sorter );
      s_SignalGameList( m_GameList );
      
  #endif
     
    }
  }
}
