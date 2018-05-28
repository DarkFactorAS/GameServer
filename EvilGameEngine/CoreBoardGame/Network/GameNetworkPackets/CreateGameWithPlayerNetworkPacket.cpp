/*************************************************************************************************
 * Project     : EvilEngine Multi-platform 2014-2016
 * File        : CreateGameWithPlayerNetworkPacket
 *
 * Description : Player wants to create a new online game
 *
 * Author      : Thor Richard Hansen
 *************************************************************************************************/

#include "Precompile.h"
#include "CreateGameWithPlayerNetworkPacket.h"
#include "NewGameNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/Account/AccountManager.h"
#include "EvilGameEngine/CoreBoardGame/GameLogic/Game/BoardGame.h"
#include "EvilGameEngine/CoreBoardGame/GameLogic/Game/BoardGameServer.h"
#include "EvilGameEngine/CoreBoardGame/GameEngine/BoardGameEngine.h"
#include "EvilGameEngine/CoreBoardGame/Network/BoardGamePacketData.h"

CreateGameWithPlayerNetworkPacket::CreateGameWithPlayerNetworkPacket(const BinaryStream* datastream) :
  BaseAuthentcatedNetworkPacket(BoardGamePacketData::PacketData_GameCreateWithPlayer, datastream)
{
}

#ifdef CLIENT

CreateGameWithPlayerNetworkPacket::CreateGameWithPlayerNetworkPacket(AccountID opponentId) :
  BaseAuthentcatedNetworkPacket(BoardGamePacketData::PacketData_GameCreateWithPlayer)
{
  m_OpponentId = opponentId;
}

BinaryStream* CreateGameWithPlayerNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseAuthentcatedNetworkPacket::GetDataStream();
  datastream->WriteUInt32(m_OpponentId);
  return datastream;
}

#else

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
void CreateGameWithPlayerNetworkPacket::Execute()
{
  BoardGameEngine* gameEngine = safe_cast<BoardGameEngine*> (GetGameEngine());
  Account* thisAccount  = GetAccount();
  Account* otherAccount = AccountManager::GetInstance()->GetOtherAccount( m_OpponentId );
  if (otherAccount != NULL && thisAccount != NULL && gameEngine != NULL )
  {
    BoardGameServer* gameServer = gameEngine->GetGameServer();
    if ( gameServer != NULL )
    {
      BoardGame* newGame = gameServer->CreateNewOnlineGame(thisAccount, otherAccount);
      if ( newGame != NULL )
      {
        newGame->SendToAllPlayers( new NewGameNetworkPacket(newGame) );
        return;
      }
    }
  }
  SendErrorToClient(BoardGamePacketData::ErrorCode_CouldNotCreateGame);
}

#endif