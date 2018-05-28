/*************************************************************************************************
 * Project     : EvilEngine Multi-platform 2014-2016
 * File        : CreateGameWithPlayerNetworkPacket
 *
 * Description : Player wants to create a new online game
 *
 * Author      : Thor Richard Hansen
 *************************************************************************************************/

#include "Precompile.h"
#include "NewGameNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/Account/AccountManager.h"
#include "EvilGameEngine/CoreBoardGame/GameLogic/Game/BoardGame.h"
#include "EvilGameEngine/CoreBoardGame/GameLogic/Game/BoardGameServer.h"
#include "EvilGameEngine/CoreBoardGame/GameEngine/BoardGameEngine.h"
#include "EvilGameEngine/CoreBoardGame/Network/BoardGamePacketData.h"

NewGameNetworkPacket::NewGameNetworkPacket(const BinaryStream* datastream) :
  BaseAuthentcatedNetworkPacket(BoardGamePacketData::PacketData_GameCreate, datastream)
{
}

BoardGame* NewGameNetworkPacket::CreateGameClass(const BinaryStream* datastream)
{
  return BoardGame::CreateGameFromData(datastream);
}

bool NewGameNetworkPacket::ReadDataStream(const BinaryStream* datastream)
{
  m_NewGame = CreateGameClass(datastream);
  return ( m_NewGame != NULL );
}


#ifdef SERVER

NewGameNetworkPacket::NewGameNetworkPacket(BoardGame* newGame) :
  BaseAuthentcatedNetworkPacket(BoardGamePacketData::PacketData_GameCreate)
{
  m_NewGame = newGame;
}

BinaryStream* NewGameNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseAuthentcatedNetworkPacket::GetDataStream();
  m_NewGame->WriteInfo(*datastream);
  return datastream;
}

#else

/************************************************************************************************
* GetGame:
* Helper function to get the game object for this game network packet
*
* @param  (CoreGameEngine&)     engine          - Pointer to the game engine
* @return (BoardGame*)                          - Return the game this packet belongs to
*
* @author Thor Richard Hansen
*************************************************************************************************/
BoardGameServer* NewGameNetworkPacket::GetGameServer(CoreGameEngine& engine)
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
 * Executes the gamecode on the receiver when receiving this packet. Leave the game and make sure 
 * the other player wins the game
 *
 * @param  (CoreGameEngine&)  engine          - Reference to the engine running this packet
 * @param  (uint32)           networkInstance - ID of the network connection that received this packet
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
void NewGameNetworkPacket::Execute()
{
  BoardGameEngine* gameEngine = safe_cast<BoardGameEngine*> ( GetGameEngine() );
  if (gameEngine != NULL)
  {
    BoardGameServer* gameServer = gameEngine->GetGameServer();
    if (gameServer != NULL)
    {
      gameServer->AddGameToCache(m_NewGame);
    }
  }
}

#endif
