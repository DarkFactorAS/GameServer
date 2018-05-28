/*************************************************************************************************
 * Project     : EvilEngine Multi-platform 2014-2016
 * File        : BaseGameNetworkPacket
 *
 * Description : Common functionality for the game network packets. This class will check that
 *               the game is valid and that the account has access to the game.
 *
 * Author      : Thor Richard Hansen
 *************************************************************************************************/

#include "Precompile.h"
#include "BaseGameNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/Account/AccountManager.h"
#include "EvilGameEngine/CoreBoardGame/GameLogic/Game/BoardGame.h"
#include "EvilGameEngine/CoreBoardGame/GameLogic/Game/BoardGameServer.h"
#include "EvilGameEngine/CoreBoardGame/GameEngine/BoardGameEngine.h"
#include "EvilGameEngine/CoreBoardGame/Network/BoardGamePacketData.h"

BaseGameNetworkPacket::BaseGameNetworkPacket( uint32 packetType, const BinaryStream* datastream) :
  BaseAuthentcatedNetworkPacket( packetType, datastream ),
  m_BoardGame( NULL )
{
  m_GameId = datastream->ReadUInt32();
}

BaseGameNetworkPacket::BaseGameNetworkPacket( uint32 packetType, uint32 gameId ) :
  BaseAuthentcatedNetworkPacket( packetType  ),
  m_GameId( gameId ),
  m_BoardGame( NULL )
{
}

BinaryStream* BaseGameNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseAuthentcatedNetworkPacket::GetDataStream();
  datastream->WriteUInt32( m_GameId );
  return datastream;
}

//void BaseGameNetworkPacket::Execute(CoreGameEngine& engine, uint32 networkInstance) DF_OVERRIDE DF_FINAL
//{
//}

/************************************************************************************************
* GetGame:
* Helper function to get the game object for this game network packet
*
* @return (BoardGame*)                          - Return the game this packet belongs to
*
* @author Thor Richard Hansen
*************************************************************************************************/
BoardGameServer* BaseGameNetworkPacket::GetGameServer()
{
  BoardGameEngine* boardGameEngine = safe_cast<BoardGameEngine*> ( GetGameEngine());
  if (boardGameEngine != NULL)
  {
    BoardGameServer* boardGameServer = boardGameEngine->GetBoardGameServer();
    return boardGameServer;
  }
  return NULL;
}

/************************************************************************************************
* GetGame:
* Helper function to get the game object for this game network packet
*
* @return (BoardGame*)                          - Return the game this packet belongs to
*
* @author Thor Richard Hansen
*************************************************************************************************/
BoardGame* BaseGameNetworkPacket::GetGame()
{
  if ( m_BoardGame != NULL )
  {
    return m_BoardGame;
  }

  if ( m_GameId == 0 )
  {
    return NULL;
  }

  BoardGameServer* boardGameServer = GetGameServer();
  if (boardGameServer != NULL)
  {
    m_BoardGame = boardGameServer->GetGame( m_GameId );
    return m_BoardGame;
  }
  return NULL;
}

/************************************************************************************************
* GetPlayer:
* Helper function to get the player object that this account is playing with
*
* @return (Player*)                             - Return the player this packet belongs to
*
* @author Thor Richard Hansen
*************************************************************************************************/
Player* BaseGameNetworkPacket::GetPlayer()
{
  BoardGame* game  = GetGame();
  Account* account = GetAccount();
  if (game != NULL && account != NULL )
  {
    return game->GetPlayerWithAccountId( account->GetAccountId() );
  }
  return NULL;
}


/************************************************************************************************
 * Execute:
 * Load the mission stars from the database and send the data back to the client
 *
 * @param  (CoreGameEngine&)     engine          - Pointer to the game engine
 * @param  (uint32)              networkInstance - ID of the connection that send the request
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
#ifdef SERVER
bool BaseGameNetworkPacket::CanExecute()
{
  // Check if we are logged in
  if ( !BaseAuthentcatedNetworkPacket::CanExecute() )
  {
    return false;
  }

  // Require a proper game
  BoardGame* game         = GetGame();
  Account* senderAccount  = GetAccount();
  if (senderAccount == NULL || !game->HasAccount( senderAccount->GetAccountId() ))
  {
    SendErrorToClient(BoardGamePacketData::ErrorCode_NotActivePlayer);
    return false;
  }

  return true;
}

/************************************************************************************************
 * IsItMyTurn:
 * Most of the game network packets requite the packet to be received only on the players turn
 * to avoid cheating. An active GM can cheat however.
 *
 * @param  (CoreGameEngine&)     engine          - Pointer to the game engine
 * @param  (uint32)              networkInstance - ID of the connection that send the request
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
bool BaseGameNetworkPacket::IsItMyTurn()
{
  BoardGame* game         = GetGame();
  Account* senderAccount  = GetAccount();
  if ( senderAccount == NULL || ( !game->IsActivePlayer(senderAccount) && !senderAccount->HasServerGMFlag() ) )
  {
    SendNotMyTurnError();
    return false;
  }
  return true;
}

/************************************************************************************************
* SendItMyTurnError:
* Send the error that it is not my turn. Someone is trying to hack!
*
* @param  (CoreGameEngine&)     engine          - Pointer to the game engine
*
* @author Thor Richard Hansen
*************************************************************************************************/
void BaseGameNetworkPacket::SendNotMyTurnError()
{
  CoreGameEngine* gameEngine = GetGameEngine();
  if ( gameEngine != NULL )
  {
    gameEngine->AddErrorPacket(m_ConnectionId, m_PacketType, BoardGamePacketData::ErrorCode_NotActivePlayer);
  }
}

/************************************************************************************************
* SendToAllPlayers:
* Helper function to send a network reply to all online players in this game
*
* @param  (BaseGameNetworkPacket*)  gameNetworkPacket - The packet to send to the clients
*
* @author Thor Richard Hansen
*************************************************************************************************/
void BaseGameNetworkPacket::SendToAllPlayers( BaseNetworkPacket* networkPacket)
{
  BoardGame* game = GetGame();
  if ( game != NULL )
  {
    game->SendToAllPlayers( networkPacket );
  }
}

void BaseGameNetworkPacket::SendToObservers( BaseNetworkPacket* gameNetworkPacket)
{
  BoardGame* game = GetGame();
  if (game != NULL)
  {
    game->SendToObservers( m_Account->GetAccountId(), gameNetworkPacket->GetDataStream());
  }
}

#endif
