/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : GameInfoManager
*
* Description : Holds a list of all games in progress ( used for the overview ) and sends/receives
*               the messages to the server to manage the games.
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"

#include "EvilEngine/CoreLib/Assert/DFAssert.h"
#include "EvilEngine/CoreDatabase/SQLResult.h"
#include "EvilEngine/CoreDatabase/CoreDatabase.h"

#include "EvilGameEngine/CoreBoardGame/GameLogic/Player/Player.h"
#include "EvilGameEngine/CoreBoardGame/Network/BoardGamePacketData.h"
#include "EvilGameEngine/CoreGameLogin/Account/Account.h"
#include "EvilGameEngine/CoreGameLogin/Account/AccountManager.h"

#include "BoardGameServer.h"

BoardGameServer::BoardGameServer()
{
}

BoardGameServer::~BoardGameServer()
{
#if defined SERVER
  FlushGameCache();
  dfAssert( m_GameCache.empty(), "BoardGameServer::Flushing gamelist failed!" );
#endif
}

void BoardGameServer::FlushGameCache()
{
#if defined SERVER
  uint32 savedGames = 0;
  uint32 numGames   = m_GameCache.size();
#endif

  for ( GameMap::iterator itGame = m_GameCache.begin(); itGame != m_GameCache.end(); itGame++ ) 
  {
    BoardGame* game = itGame->second;

    // Save changes to database if needed
#if defined SERVER
    if ( game->IsDirty() )
    {
      if ( !game->SaveToDatabase() )
      {
        ++itGame;
        LogErrorFMT("BoardGameServer", "Could not save game [%d] to the database!", game->GetGameId() );
        continue;
      }
      else
      {
        LogWarningFMT("BoardGameServer", "[%d / %d ] Saved game [%d] to the database!", ++savedGames, numGames, game->GetGameId() );
      }
    }
#endif

    delete game;
  }
  m_GameCache.clear();
}

void BoardGameServer::AddGameToCache( BoardGame* game )
{
  std::map<uint32,BoardGame*>::iterator itGame = m_GameCache.find( game->GetGameId() );
  if ( itGame != m_GameCache.end() )
  {
    LogWarningFMT("BoardGameServer", "Game %d already exist in cache", game->GetGameId() );
  }

  game->UpdateCache();
  m_GameCache[ game->GetGameId() ] = game;
#ifdef CLIENT
  SignalAddedGame( game );
#endif
}

void BoardGameServer::RemoveGameFromCache( uint32 gameId )
{
  // Remove from the map
  std::map<uint32,BoardGame*>::iterator itGame = m_GameCache.find( gameId );
  if ( itGame != m_GameCache.end() )
  {
    m_GameCache.erase( itGame );
  }
#ifdef CLIENT
  SignalRemovedGame(gameId);
#endif
}

/************************************************************************************************
* GetGameFromCache:
* Returns an existing game from its Id
* 
* @param (uint32)           gameId  - ID of the game to find
* @return (BoardGameInfo*)          - The game if found, NULL otherwise
* 
* @author Thor Richard Hansen
*************************************************************************************************/
BoardGame* BoardGameServer::GetGameFromCache( uint32 gameId )
{
  GameMap::iterator itFind = m_GameCache.find( gameId );
  if ( itFind != m_GameCache.end() ) 
  {
    return itFind->second;
  }
  return NULL;
}

/************************************************************************************************
* GetGame:
* Returns an existing game from its Id
* 
* @param (uint32)           gameId  - ID of the game to find
* @return (BoardGameInfo*)          - The game if found, NULL otherwise
* 
* @author Thor Richard Hansen
*************************************************************************************************/
BoardGame* BoardGameServer::GetGame( uint32 gameId )
{
  BoardGame* game = GetGameFromCache( gameId );

#ifdef SERVER
  if ( game == NULL || !game->IsLoaded() )
  {
    game = LoadGameFromDb( gameId );
  }
#endif

  return game;
}

/************************************************************************************************
* CreateNewGame:
* Create a new game with two players and sends it to the players
* 
* @param (BoardGameNetworkConnection)   gameConnection  - The connection to send the data on.
* @param  (BinaryStream)                dataStream      - Data stream from the caller
* 
* @author Thor Richard Hansen
*************************************************************************************************/
#ifdef SERVER
uint32 BoardGameServer::GetNextGameId()
{
  static uint32 nextGameId = 0;
  if ( nextGameId == 0 )
  {
    const SQLResultSet& result = CoreDatabase::GetInstance()->ExecuteSelect( "Select max( GameId ) from games" );
    if( result.Next() )
    {
      nextGameId = result.GetInt();
    }
  }
  return ++nextGameId;
}
#endif

/************************************************************************************************
* FrameProcess:
* Run function that runs every frame. If a game has not been touched in x minutes it will be
* removed from memory ( and saved if needed ). This function will also periodicly save games
* that has changes or wants to be saved
* 
* @param (float)   deltaTime  - Amount of time that has passed since the last call
* 
* @author Thor Richard Hansen
*************************************************************************************************/
#ifdef SERVER
void BoardGameServer::FrameProcess(  float deltaTime )
{
  for ( GameMap::iterator itGame = m_GameCache.begin(); itGame != m_GameCache.end(); ) 
  {
    BoardGame* game = itGame->second;
    if ( !game->FrameProcess( deltaTime ) )
    {
      delete game;
      GameMap::iterator itDel = itGame++;
      m_GameCache.erase( itDel );
    }
    else
    {
      ++itGame;
    }
  }
}
#endif

void BoardGameServer::UpdateGameHistory()
{
#ifdef SERVER
  FlushGameCache();
  CoreDatabase::GetInstance()->ExecuteDelete( "delete from games where gamestatus > 1 and lastupdated < now() - interval 1 day" );
#endif
}

