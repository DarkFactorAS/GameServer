/*************************************************************************************************
 * Project     : EvilEngine Multi-platform 2014-2016
 * File        : CreateGameWithPlayerNetworkPacket
 *
 * Description : Player wants to create a new online game
 *
 * Author      : Thor Richard Hansen
 *************************************************************************************************/

#include "Precompile.h"
#include "CreateGameWithRandomPlayerNetworkPacket.h"
#include "NewGameNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/Account/AccountManager.h"
#include "EvilGameEngine/CoreBoardGame/GameLogic/Game/BoardGame.h"
#include "EvilGameEngine/CoreBoardGame/GameLogic/Game/BoardGameServer.h"
#include "EvilGameEngine/CoreBoardGame/GameEngine/BoardGameEngine.h"
#include "EvilGameEngine/CoreBoardGame/Network/BoardGamePacketData.h"

CreateGameWithRandomPlayerNetworkPacket::CreateGameWithRandomPlayerNetworkPacket(const BinaryStream* datastream) :
  BaseAuthentcatedNetworkPacket(BoardGamePacketData::PacketData_GameCreateWithRandomPlayer, datastream)
{
}

#ifdef CLIENT

CreateGameWithRandomPlayerNetworkPacket::CreateGameWithRandomPlayerNetworkPacket() :
  BaseAuthentcatedNetworkPacket(BoardGamePacketData::PacketData_GameCreateWithRandomPlayer)
{
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
BoardGameServer* CreateGameWithRandomPlayerNetworkPacket::GetGameServer(CoreGameEngine& engine)
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
void CreateGameWithRandomPlayerNetworkPacket::Execute()
{
  BoardGameEngine* gameEngine = safe_cast<BoardGameEngine*> (GetGameEngine());
  if ( gameEngine != NULL )
  {
    BoardGameServer* gameServer = gameEngine->GetGameServer();
    Account* thisAccount = GetAccount();
    if (thisAccount != NULL && gameServer != NULL)
    {
      uint32 accountId = thisAccount->GetAccountId();
      StringList accountList = gameServer->GetOpponents(accountId);
      accountList.push_back(accountId);

      Account* otherAccount = AccountManager::GetInstance()->GetRandomAccount(accountList);
      if (otherAccount != NULL)
      {
        BoardGame* newGame = gameServer->CreateNewOnlineGame(thisAccount, otherAccount);
        if (newGame != NULL)
        {
          newGame->SendToAllPlayers(new NewGameNetworkPacket(newGame));
          return;
        }
      }
    }
  }

  SendErrorToClient( BoardGamePacketData::ErrorCode_CouldNotCreateGame);
}

#endif