/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : BoardGame
*
* Description : Holds all the information about a game. 
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "BoardGame.h"
#include "EvilEngine/CoreLib/BasicTypes/BinaryStream/BinaryStream.h"
#include "EvilGameEngine/CoreGameLogin/Account/AccountManager.h"
#include "EvilGameEngine/CoreBoardGame/Playfield/Tiles/BaseTile.h"
#include "EvilGameEngine/CoreBoardGame/GameLogic/Game/BoardGameServer.h"
#include "EvilGameEngine/CoreGameLogin/Account/Account.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/Packets/BaseNetworkPacket.h"

#define CACHE_TIMEOUT 15*60 // 15 minutes until it 
#define SAVE_PERIOD   300.0 //  5 minutes between saves

BoardGame::BoardGame() :
  m_GameId( 0 ),
  m_GamePhaseId( 0 ),
  m_IsDirty( true ),
  m_IsLoaded( false ),
  m_IsInDatabase( false ),
  m_KeepInCache( CACHE_TIMEOUT ),
  m_LastChangedTime( 0.0f ),
  m_PlayfieldId( 0 ),
  m_TimeStamp( 0 ),
  m_ActivePlayerId( 0 ),
  m_CharacterWonId( 0 ),
  m_GameStatus( 0 )
{
}

BoardGame::BoardGame( uint32 gameId, uint32 playfieldId ) :
  m_GameId( gameId ),
  m_GamePhaseId( 0 ),
  m_IsDirty( true ),
  m_IsLoaded( false ),
  m_IsInDatabase( false ),
  m_KeepInCache( CACHE_TIMEOUT ),
  m_LastChangedTime( 0.0f ),
  m_PlayfieldId( playfieldId ),
  m_TimeStamp( 0 ),
  m_ActivePlayerId( 0 ),
  m_CharacterWonId( 0 ),
  m_GameStatus( 0 )
{
}

BoardGame::~BoardGame()
{
  for ( size_t index = 0; index < m_PlayerList.size(); index++ )
  {
    delete m_PlayerList[ index ];
  }
}

BoardGame* BoardGame::CreateGameFromData(const BinaryStream* /*data*/)
{
  dfBugreport( false, "BoardGame::CreateGameFromData::Tried to create game with baseclass!");
  return NULL;
}

void BoardGame::NeedSave()
{
  if ( !m_IsDirty )
  {
    m_IsDirty         = true;
    m_LastChangedTime = SAVE_PERIOD;
  }
}

void BoardGame::ClearDirty()
{
  m_IsDirty   = false;
  m_LastChangedTime = 0;
}

#ifdef SERVER
bool BoardGame::ShouldSave( uint32 currentTime )
{
  if ( m_IsDirty && ( currentTime - m_LastChangedTime > SAVE_PERIOD ) )
  {
    return true;
  }
  return false;
}
#endif

void BoardGame::SetClientPlayer( AccountID accountId )
{
  for ( size_t index = 0; index < m_PlayerList.size(); index++ )
  {
    Player* player = m_PlayerList[ index ];
    if ( player->GetAccountId() == accountId )
    {
      player->SetClient( true );

      // Client player is always at position 0
      if ( index > 0 )
      {
        Player* swapPlayer    = m_PlayerList[ 0 ];
        m_PlayerList[ 0 ]     = player;
        m_PlayerList[ index ] = swapPlayer;
      }
    }
    else
    {
      player->SetClient( false );
    }
  }
}

/************************************************************************************************
* AddPlayer:
* Add a plauer this this game
*
* @param  (Player*)            player    - The player to add to the game
*
* @author Thor Richard Hansen
*************************************************************************************************/
void BoardGame::AddPlayer( Player* player )
{
  //if ( m_ActivePlayerId == 0 )
  //{
  //  m_ActivePlayerId = player->GetId();
  //}
  m_PlayerList.push_back( player );
}

/************************************************************************************************
* HasPlayer:
* Returns true if the game has this player
*
* @param  (Player*)            player     - The player to check
* @return (bool)                          - Return true if the player is in this game
*
* @author Thor Richard Hansen
*************************************************************************************************/
bool BoardGame::HasPlayer( Player* player ) const
{
  return HasPlayer( player->GetCharacterId() );
}

/************************************************************************************************
* HasPlayer:
* Returns true if the game has this player
*
* @param  (uint32)            playerId    - ID of the player to check
* @return (bool)                          - Return true if the player is in this game
*
* @author Thor Richard Hansen
*************************************************************************************************/
bool BoardGame::HasPlayer( uint32 playerId ) const
{
  for ( size_t index = 0; index < m_PlayerList.size(); index++ )
  {
    const Player* player = m_PlayerList[ index ];
    if ( player->GetCharacterId() == playerId )
    {
      return true;
    }
  }
  return false;
}

/************************************************************************************************
* HasAccount:
* Returns true if the game has this player account
*
* @param  (uint32)            playerId    - ID of the player to check
* @return (bool)                          - Return true if the player is in this game
*
* @author Thor Richard Hansen
*************************************************************************************************/
bool BoardGame::HasAccount( uint32 accountId ) const
{
  for ( size_t index = 0; index < m_PlayerList.size(); index++ )
  {
    const Player* player = m_PlayerList[ index ];
    if ( player->GetAccountId() == accountId )
    {
      return true;
    }
  }
  return false;
}

/************************************************************************************************
* GetPlayer:
* Return a player from its ID 
*
* @param  (uint32)            playerId    - ID of the player to check
* @return (Player*)                       - The player ( if found )
*
* @author Thor Richard Hansen
*************************************************************************************************/
Player* BoardGame::GetPlayerWithAccountId( uint32 accountId )
{
  for ( size_t index = 0; index < m_PlayerList.size(); index++ )
  {
    Player* player = m_PlayerList[ index ];
    if ( player->GetAccountId() == accountId )
    {
      return player;
    }
  }
  return NULL;
}
Player* BoardGame::GetPlayerWithCharacterId( uint32 characterId )
{
  for ( size_t index = 0; index < m_PlayerList.size(); index++ )
  {
    Player* player = m_PlayerList[ index ];
    if ( player->GetCharacterId() == characterId )
    {
      return player;
    }
  }
  return NULL;
}

/************************************************************************************************
* GetPlayer:
* Return a player with a specific index
*
* @param  (size_t)             index      - Index of the player
* @return (Player*)                       - The player ( if within bounds of the list )
*
* @author Thor Richard Hansen
*************************************************************************************************/
Player* BoardGame::GetPlayerWithIndex( size_t index ) const
{
  if ( index < m_PlayerList.size() )
  {
    return m_PlayerList[ index ];
  }
  return NULL;
}

/************************************************************************************************
* NextPlayer:
* Changes the active player to the next one in the playerlist. Also return the new active player
*
* @return (Player*)                       - The new active player
*
* @author Thor Richard Hansen
*************************************************************************************************/
Player* BoardGame::NextPlayer()
{
  bool setNextPlayer = ( m_ActivePlayerId == 0 ) ? true : false;

  for ( size_t index = 0; index < m_PlayerList.size(); index++ )
  {
    Player* player = m_PlayerList[ index ];
    if ( setNextPlayer )
    {
      m_ActivePlayerId  = player->GetCharacterId();
      m_IsDirty         = true;
      return player;
    }
    else if ( player->GetCharacterId() == m_ActivePlayerId )
    {
      setNextPlayer = true;
    }
  }
  if ( !m_PlayerList.empty() )
  {
    Player* player    = m_PlayerList[ 0 ];
    m_ActivePlayerId  = player->GetCharacterId();
    return player;
  }
  return NULL;
}

void BoardGame::SetActiveAccountId( AccountID accountId )
{
  Player* player = GetPlayerWithAccountId( accountId );
  if ( player != NULL )
  {
    m_ActivePlayerId = player->GetCharacterId();
  }
  else
  {
    m_ActivePlayerId = 0;
  }
}

void BoardGame::SetActivePlayerId(uint32 characterId )
{ 
  m_ActivePlayerId = characterId;
#ifdef CLIENT
  SignalActivePlayerChanged( characterId );
#endif
}

/************************************************************************************************
* GetActivePlayer:
* Return the active player
*
* @return (Player*)                       - The active player
*
* @author Thor Richard Hansen
*************************************************************************************************/
uint32 BoardGame::GetActiveAccountId() const
{
  for (size_t index = 0; index < m_PlayerList.size(); index++)
  {
    Player* player = m_PlayerList[index];
    if (player->GetCharacterId() == m_ActivePlayerId)
    {
      return player->GetAccountId();
    }
  }
  return 0;
}

Player* BoardGame::GetActivePlayer() const
{
  for ( size_t index = 0; index < m_PlayerList.size(); index++ )
  {
    Player* player = m_PlayerList[ index ];
    if ( player->GetCharacterId() == m_ActivePlayerId )
    {
      return player;
    }
  }
  return NULL;
}

bool BoardGame::IsActivePlayer(Account* account)
{
  Player* player = GetActivePlayer();
  if ( player != NULL && account != NULL && player->GetAccountId() == account->GetAccountId() )
  {
    return true;
  }
  return false;
}

int BoardGame::GetPlayerIndex( const Player* findPlayer ) const
{
  for ( size_t index = 0; index < m_PlayerList.size(); index++ )
  {
    Player* player = m_PlayerList[ index ];
    if ( player == findPlayer )
    {
      return (int)index;
    }
  }
  return -1;
}

uint32 BoardGame::GetRandomPlayerId( uint32 excludePlayerId ) const
{
  if ( !m_PlayerList.empty() )
  {
    uint32 maxTries = 100;
    while ( maxTries-- )
    {
      uint nIndex = (uint32)(rand() % m_PlayerList.size());
      Player *player = m_PlayerList[nIndex];
      if ( player->GetCharacterId() != excludePlayerId )
      {
        return player->GetCharacterId();
      }
    }
  }
  return 0;
}

#ifdef CLIENT
/************************************************************************************************
* SendToServer:
* Send a network packet to the server. Only the client can send this packet
*
* @param (BaseNetworkPacket*) networkPacket - The packet to send 
*
* @author Thor Richard Hansen
*************************************************************************************************/
void BoardGame::SendToServer(BaseNetworkPacket* networkPacket)
{
  m_PendingMessages.push_back(std::pair< uint32, BinaryStream* >(0, networkPacket->GetDataStream() ));
}
#endif

#ifdef SERVER
/************************************************************************************************
 * SendToAllPlayers:
 * Send a network packet to all players in this game. Only the server can send this packet
 *
 * @param (CoreGameEngine*)     engine        - The packet to send 
 * @param (BaseNetworkPacket*)  networkPacket - The packet to send 
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
void BoardGame::SendToAllPlayers( BaseNetworkPacket* networkPacket )
{
  if ( networkPacket != NULL )
  {
    SendToAllPlayers( networkPacket->GetDataStream() );
  }
  else
  {
    dfBugreport( networkPacket != NULL, "SendToAllPlayers:NetworkPacket is NULL" );
  }
}
#endif

#ifdef SERVER
/************************************************************************************************
 * SendToAllPlayers:
 * Send a network packet to all players in this game. Only the server can send this packet
 *
 * @param (CoreGameEngine*)     engine        - The packet to send 
 * @param (BinaryStream*)       binaryStream  - The data to send 
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
void BoardGame::SendToAllPlayers(BinaryStream* binaryStream)
{
  AccountManager* accountManager  = AccountManager::GetInstance();

  bool mustClone = false;
  for (size_t index = 0; index < m_PlayerList.size(); index++)
  {
    const Player*   player  = m_PlayerList[index];
    uint32 connectionId     = accountManager->GetOnlineConnectionIdFromAccountId( player->GetAccountId() );
    if ( connectionId != 0 )
    {
      if (!mustClone)
      {
        mustClone = true;
      }
      else
      {
        binaryStream = new BinaryStream(binaryStream);
      }
      SendToClient( connectionId, binaryStream );
    }
  }
}
#endif

#ifdef SERVER
/************************************************************************************************
 * SendToObservers:
 * Send a network packet to all players in this game except the specified account. 
 * Only the server can send this packet
 *
 * @param (CoreGameEngine&)     engine            - The packet to send 
 * @param (AccountID*)          excludeAccountId  - The account to not send this packet to 
 * @param (BinaryStream*)       binaryStream      - The data to send 
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
void BoardGame::SendToObservers( AccountID excludeAccountId, BinaryStream* binaryStream)
{
  AccountManager* accountManager  = AccountManager::GetInstance();

  bool mustClone = false;
  for (size_t index = 0; index < m_PlayerList.size(); index++)
  {
    const Player*   player = m_PlayerList[index];
    if ( player->GetAccountId() != excludeAccountId )
    {
      uint32 connectionId = accountManager->GetOnlineConnectionIdFromAccountId(player->GetAccountId());
      if ( connectionId != 0 )
      {
        if (!mustClone)
        {
          mustClone = true;
        }
        else
        {
          binaryStream = new BinaryStream(binaryStream);
        }
        SendToClient( connectionId, binaryStream );
      }
    }
  }
}
#endif

#ifdef SERVER
void BoardGame::SendToClient(uint32 /*connectionInstanceId*/, BinaryStream* /*binaryStream*/)
{
  dfBugreport(false,"SendToClient is not supported. Rewrite this!");
  //CoreGameEngine* gameEngine = GetGameEngine();
  //if (gameEngine != NULL )
  //{
  //  gameEngine->SendBinaryStreamToEndpoint(connectionInstanceId, binaryStream);
  //}
}
#endif

/************************************************************************************************
* UpdateCache:
* Update the cache timer for this game. When the timer runs out ( noone has touched this game )
* the game is flushed from memory.
*
* @author Thor Richard Hansen
*************************************************************************************************/
void BoardGame::UpdateCache()
{
  m_KeepInCache = CACHE_TIMEOUT;
}

/************************************************************************************************
* SetIsDatabase:
* Set if this game is new ( not in the database ) or already exists in the database.
*
* @param  (bool)                isInDataBase            - Set to true if this game exists in db
*
* @author Thor Richard Hansen
*************************************************************************************************/
#ifdef SERVER
void BoardGame::SetIsDatabase( bool isInDataBase )
{
  m_IsInDatabase = isInDataBase;
}
#endif

#ifdef SERVER
bool BoardGame::FrameProcess( float deltaTime )
{
  // Send all messages
  for ( std::vector< std::pair< uint32, BinaryStream* > >::const_iterator itMessage = m_PendingMessages.begin(); itMessage != m_PendingMessages.end(); ++itMessage )
  {
    const std::pair< uint32, BinaryStream* >& msg = *itMessage;

    // Only fetch the online accounts
    const Account*  account = AccountManager::GetInstance()->GetCachedAccount( (AccountID) msg.first );
    if ( account != NULL && account->GetConnectionId() != 0 )
    {
      SendToClient( account->GetConnectionId(),  msg.second );
    }
  }
  m_PendingMessages.clear();

  m_KeepInCache -= deltaTime;
  if ( m_KeepInCache <= 0.0f )
  {
    SaveToDatabase();
    LogInfoFMT("BoardGame", "FrameProcess: Removing game[%d] from memory/cache [Timeout %d min]", m_GameId, CACHE_TIMEOUT );
    return false;
  }
  else if ( m_LastChangedTime > 0.0f )
  {
    m_LastChangedTime -= deltaTime;
    if ( m_LastChangedTime <= 0.0f )
    {
      SaveToDatabase();
    }
  }
  return true;
}
#endif
