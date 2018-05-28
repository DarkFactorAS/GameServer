#ifndef EVILENGINE_COREBOARDGAME_GAMELOGIC_GAME_BOARDGAME
#define EVILENGINE_COREBOARDGAME_GAMELOGIC_GAME_BOARDGAME 1
#pragma once

#include "EvilEngine/CoreLib/CoreMath/Vector2.h"
#include "EvilGameEngine/CoreBoardGame/GameLogic/Player/Player.h"
#include "EvilGameEngine/CoreGameLogin/Account/Account.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"

class BinaryStream;
class BaseTile;
class BaseNetworkPacket;
class BoardGameServer;

class BoardGame
{
public:

  enum GAMESTATUS
  {
    GAMESTATUS_NEWGAME = 0,
    GAMESTATUS_GAMEINPROGRESS,
    GAMESTATUS_GAMEFINISHED,
    GAMESTATUS_GAMEDELETED,
    GAMESTATUS_GAMETIMEDOUT
  };

  BoardGame();
  BoardGame( uint32 gameId, uint32 playfieldId );
  virtual ~BoardGame();

  static BoardGame*               CreateGameFromData(const BinaryStream* data);

  uint32                          GetGameId() const { return m_GameId; }

  void                            SetPlayfieldId( uint32 playfieldId ){ m_PlayfieldId = playfieldId; }
  uint32                          GetPlayfieldId() const{ return m_PlayfieldId; }

  void                            SetTimestamp( uint32 timeStamp ){ m_TimeStamp = timeStamp; }

  void                            SetClientPlayer( AccountID accountId );
  void                            AddPlayer( Player* player );
  bool                            HasPlayer( Player* player ) const;
  bool                            HasAccount( uint32 accountId ) const;
  bool                            HasPlayer( uint32 playerId ) const;
  Player*                         GetPlayerWithAccountId( uint32 accountId );
  Player*                         GetPlayerWithCharacterId( uint32 characterId );
  Player*                         GetPlayerWithIndex( size_t index ) const;
  uint32                          GetNumPlayers() const{ return (uint32)m_PlayerList.size(); }
  std::vector< Player* >          GetPlayerList() const{ return m_PlayerList; }
  Player*                         NextPlayer();
  void                            SetActivePlayerId( uint32 playerId );
  void                            SetActiveAccountId( AccountID accountId );
  uint32                          GetActiveCharacterId() const { return m_ActivePlayerId; }
  uint32                          GetActiveAccountId() const;
  Player*                         GetActivePlayer() const;
  int                             GetPlayerIndex( const Player* findPlayer ) const;
  bool                            IsActivePlayer( Account* account );
  uint32                          GetRandomPlayerId( uint32 excludePlayerId = 0 ) const;

#ifdef CLIENT
  void                            SendToServer(BaseNetworkPacket* networkPacket);
#endif
  void                            SendToAllPlayers( BaseNetworkPacket* networkPacket);
  void                            SendToAllPlayers( BinaryStream* binaryStream );
  void                            SendToObservers( AccountID excludeAccountId, BinaryStream* binaryStream );
  void                            SendToClient(uint32 connectionInstanceId, BinaryStream* binaryStream);

  bool                            IsLoaded() const{ return m_IsLoaded; }
  uint32                          GetGamePhase() const{ return m_GamePhaseId; }
  void                            SetGamePhase( uint32 gamePhaseId ){ m_GamePhaseId = gamePhaseId; NeedSave(); }

  uint32                          GetWinningCharacterId() const { return m_CharacterWonId; }
  void                            SetWinningCharacterId( uint32 characterId ){ m_CharacterWonId = characterId; }

  void                            SetGameStatus( uint32 gameStatus ){ m_GameStatus = gameStatus; }
  uint32                          GetGameStatus() const{ return m_GameStatus; }
  bool                            IsGameFinished(){ return ( m_GameStatus >= GAMESTATUS_GAMEFINISHED ); }
  bool                            IsDirty() const{ return m_IsDirty; }
  virtual bool                    FinishGame(uint32 /*gamestatus*/){ return false; }

  virtual void                    UpdateCache();

#ifdef SERVER
  //
  // Some abstract functions that has to be implemented
  //
  virtual void                    WriteInfo( BinaryStream& data ) const = 0;
  virtual void                    WriteData( BinaryStream& data ) const = 0;
  virtual bool                    SaveToDatabase()                      = 0;
  virtual bool                    LoadFromDatabase()                    = 0;

  bool                            IsInDatabase() const { return m_IsInDatabase; }

  void                            SetIsDatabase( bool isInDataBase );
  void                            SetIsLoaded( bool isInDataBase ){ m_IsLoaded = isInDataBase; }
  bool                            ShouldSave( uint32 currentTime );
  bool                            FrameProcess( float deltaTime );

#else

  Signal1< void, uint32 /* CharacterId */ > SignalActivePlayerChanged;

#endif

  void                            NeedSave();
  //void                            MarkDirty();
  void                            ClearDirty();

protected:


  uint32                          m_GameId;
  uint32                          m_GamePhaseId;
  bool                            m_IsDirty;  
  bool                            m_IsLoaded;
  bool                            m_IsInDatabase;

  float                           m_KeepInCache;
  float                           m_LastChangedTime;

  uint32                          m_PlayfieldId;
  uint32                          m_TimeStamp;
  uint32                          m_ActivePlayerId;
  std::vector< Player* >          m_PlayerList;
  uint32                          m_CharacterWonId;
  uint32                          m_GameStatus;

  std::vector< std::pair< uint32, BinaryStream* > >  m_PendingMessages;
};

#endif  /// EVILENGINE_COREBOARDGAME_GAMELOGIC_GAME_BOARDGAMEINFO
