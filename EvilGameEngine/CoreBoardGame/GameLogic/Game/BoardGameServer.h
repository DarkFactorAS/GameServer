#ifndef EVILENGINE_COREBOARDGAME_GAMELOGIC_GAME_BoardGameServer
#define EVILENGINE_COREBOARDGAME_GAMELOGIC_GAME_BoardGameServer 1
#pragma once

#include "EvilEngine/CoreLib/BasicTypes/BinaryStream/BinaryStream.h"
#include "EvilEngine/CoreLib/BasicTypes/String/StringList.h"
#include "BoardGame.h"
#include "EvilGameEngine/CoreBoardGame/GameEngine/BoardGameEngine.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/Packets/BaseNetworkPacket.h"
#include <map>

class Account;

typedef std::map< uint32, BoardGame* > GameMap;
typedef std::vector< BoardGame* >      GameList;

class GameListSorter
{
public:
  GameListSorter(){}
  bool operator()( BoardGame* i,BoardGame* j)
  {
    return i->GetGameStatus() < j->GetGameStatus();
  }
};

class BoardGameServer
{
public:
  BoardGameServer();
  virtual ~BoardGameServer();
  
  BoardGame*                            GetGame( uint32 gameId );
  BoardGame*                            GetGameFromCache( uint32 gameId );
  void                                  UpdateGameHistory();

  void                                  FlushGameCache();
  void                                  AddGameToCache( BoardGame* game );
  void                                  RemoveGameFromCache( uint32 gameId );

#ifdef SERVER

  virtual BoardGame*                    LoadGameFromDb( uint32 gameId ) = 0;
  virtual BoardGame*                    CreateNewOnlineGame( Account* /*player1*/, Account* /*player2*/ ){ return NULL; };
  virtual StringList                    GetOpponents( AccountID /*accountId*/ ){ return StringList::zero; }
  void                                  FrameProcess( float deltaTime );
  virtual GameList                      GetActiveGamesForAccount(uint32 accountId) = 0;

#else

  Signal1< void, BoardGame* >           SignalAddedGame;
  Signal1< void, uint32 /* GameID */ >  SignalRemovedGame;
  Signal0< void >                       SignalClearGameCache;
  Signal1< void, BoardGame* >           SignalReceivedGameData;

#endif
  
protected:

#ifdef SERVER

  uint32                                GetNextGameId();

  virtual void                          DeleteGame( uint32 /*gameId*/ ){};
  
#endif

private:

  GameMap                               m_GameCache;
};

#endif  /// EVILENGINE_COREBOARDGAME_GAMELOGIC_GAME_BoardGameServer
