/*************************************************************************************************
 * Project     : EvilEngine Multi-platform 2014-2016
 * File        : DeleteGameNetworkPacket
 *
 * Description : Player wants to leave an online game
 *
 * Author      : Thor Richard Hansen
 *************************************************************************************************/

#include "Precompile.h"
#include "DeleteGameNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/Account/AccountManager.h"
#include "EvilGameEngine/CoreBoardGame/GameLogic/Game/BoardGame.h"
#include "EvilGameEngine/CoreBoardGame/GameLogic/Game/BoardGameServer.h"
#include "EvilGameEngine/CoreBoardGame/GameEngine/BoardGameEngine.h"
#include "EvilGameEngine/CoreBoardGame/Network/BoardGamePacketData.h"

DeleteGameNetworkPacket::DeleteGameNetworkPacket( uint32 gameId ) :
  BaseGameNetworkPacket( BoardGamePacketData::PacketData_GameDelete, gameId )
{
}

DeleteGameNetworkPacket::DeleteGameNetworkPacket(const BinaryStream* datastream) :
  BaseGameNetworkPacket(BoardGamePacketData::PacketData_GameDelete, datastream)
{
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
void DeleteGameNetworkPacket::Execute()
{
#ifdef SERVER
  BoardGameServer* gameServer = GetGameServer();
  if ( gameServer != NULL )
  {
     
    BoardGame* game     = GetGame();
    Account*   account  = GetAccount();
    if ( account != NULL && !game->IsGameFinished() )
    {
      // Make sure the other player "wins" since this player deleted the game
      if ( game->IsActivePlayer( account ) )
      {
        game->NextPlayer();
      }
      game->SetWinningCharacterId( game->GetActivePlayer()->GetCharacterId() );
        
      if ( game->FinishGame( BoardGame::GAMESTATUS_GAMEDELETED ) )
      {
        game->SaveToDatabase();
        SendToAllPlayers( new DeleteGameNetworkPacket( m_GameId ) );
      }
    }
      
    gameServer->RemoveGameFromCache( m_GameId );
    delete game;
  }

#endif
}

