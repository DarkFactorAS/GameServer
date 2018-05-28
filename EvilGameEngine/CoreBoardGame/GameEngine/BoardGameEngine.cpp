
#include "Precompile.h"
#include "BoardGameEngine.h"

#include "EvilEngine/CoreBugReport/Client/BugReportClient.h"
#include "EvilEngine/CoreBugReport/Common/BugReport.h"

#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"
#include "EvilGameEngine/CoreGameLogin/Account/AccountManager.h"
#include "EvilGameEngine/CoreGameLogin/Account/Account.h"
#include "EvilGameEngine/CoreBoardGame/GameLogic/Game/BoardGameServer.h"
#include "EvilGameEngine/CoreBoardGame/Network/BoardGamePacketData.h"
#include "EvilGameEngine/CoreBoardGame/Network/GameNetworkPackets/DeleteGameNetworkPacket.h"
#include "EvilGameEngine/CoreBoardGame/Network/GameNetworkPackets/CreateGameWithPlayerNetworkPacket.h"
#include "EvilGameEngine/CoreBoardGame/Network/GameNetworkPackets/CreateGameWithRandomPlayerNetworkPacket.h"
#include "EvilGameEngine/CoreBoardGame/Network/GameNetworkPackets/NewGameNetworkPacket.h"
#include "EvilGameEngine/CoreBoardGame/Network/GameNetworkPackets/GameListNetworkPacket.h"
#include "EvilGameEngine/CoreBoardGame/Network/GameNetworkPackets/SearchForPlayerWithNickNetworkPacket.h"
#include "EvilGameEngine/CoreBoardGame/Network/PlayfieldNetworkPackets/LoadPlayfieldNetworkPacket.h"
#include "EvilGameEngine/CoreBoardGame/Network/PlayfieldNetworkPackets/SavePlayfieldNetworkPacket.h"
#include "EvilGameEngine/CoreBoardGame/Network/PlayfieldNetworkPackets/DeletePlayfieldNetworkPacket.h"
#include "EvilGameEngine/CoreBoardGame/Network/PlayfieldNetworkPackets/PlayfieldListNetworkPacket.h"

#ifdef CLIENT
  #include "EvilEngine/CoreRendering/OpenGLRender/CoreOpenGLRender.h"
  #include "EvilEngine/CoreGuiToolkit/CoreGuiToolkit.h"
  #include "EvilGameEngine/CoreGameLogin/NetworkPackets/ItemShopPackets/PurchaseItemFromAmazonNetworkPacket.h"
  #include "EvilGameEngine/CoreGameLogin/NetworkPackets/ItemShopPackets/PurchaseItemFromAppleNetworkPacket.h"
  #include "EvilGameEngine/CoreGameLogin/NetworkPackets/ItemShopPackets/PurchaseItemFromDarkfactorNetworkPacket.h"
#endif

BoardGameEngine::BoardGameEngine() : 
  CoreGameEngine()
{
  m_GameServer = NULL;

  // Game packets
  RegisterPacketType(BoardGamePacketData::PacketData_GameCreateWithPlayer, CreateGameWithPlayerNetworkPacket::Create );
  RegisterPacketType(BoardGamePacketData::PacketData_GameCreateWithRandomPlayer, CreateGameWithRandomPlayerNetworkPacket::Create );
  RegisterPacketType(BoardGamePacketData::PacketData_GameCreate, NewGameNetworkPacket::Create);
  RegisterPacketType(BoardGamePacketData::PacketData_GameList, GameListNetworkPacket::Create);
  RegisterPacketType(BoardGamePacketData::PacketData_GameDelete, DeleteGameNetworkPacket::Create);
  RegisterPacketType(BoardGamePacketData::PacketData_PlayfieldLoad, LoadPlayfieldNetworkPacket::Create);
  RegisterPacketType(BoardGamePacketData::PacketData_PlayfieldSave, SavePlayfieldNetworkPacket::Create);
  RegisterPacketType(BoardGamePacketData::PacketData_PlayfieldDelete, DeletePlayfieldNetworkPacket::Create);
  RegisterPacketType(BoardGamePacketData::PacketData_PlayfieldList, PlayfieldListNetworkPacket::Create);
  RegisterPacketType(BoardGamePacketData::PacketData_SearchPlayer, SearchForPlayerWithNickNetworkPacket::Create);

  // Playfield packets
  RegisterPacketType(BoardGamePacketData::PacketData_PlayfieldDelete, DeletePlayfieldNetworkPacket::Create);
  RegisterPacketType(BoardGamePacketData::PacketData_PlayfieldLoad, LoadPlayfieldNetworkPacket::Create);
  RegisterPacketType(BoardGamePacketData::PacketData_PlayfieldList, PlayfieldListNetworkPacket::Create);
  RegisterPacketType(BoardGamePacketData::PacketData_PlayfieldSave, SavePlayfieldNetworkPacket::Create);

  // Network error messages
  NetworkBase::RegisterNetworkErrorMessage(BoardGamePacketData::ErrorCode_CodeError, "Board:Code Error");
  NetworkBase::RegisterNetworkErrorMessage(BoardGamePacketData::ErrorCode_ErrorWithPlayfield, "Error with playfield");
  NetworkBase::RegisterNetworkErrorMessage(BoardGamePacketData::ErrorCode_ErrorWithMissionId, "Error with mission id");
  NetworkBase::RegisterNetworkErrorMessage(BoardGamePacketData::ErrorCode_ErrorWithSavingGame, "Could not save game");
  NetworkBase::RegisterNetworkErrorMessage(BoardGamePacketData::ErrorCode_UnknownGame, "Unknown game");
  NetworkBase::RegisterNetworkErrorMessage(BoardGamePacketData::ErrorCode_NotActivePlayer, "Action illegal|Not active player");
  NetworkBase::RegisterNetworkErrorMessage(BoardGamePacketData::ErrorCode_CouldNotsaveGame, "Could not save game");
  NetworkBase::RegisterNetworkErrorMessage(BoardGamePacketData::ErrorCode_InvalidDeckTile, "Invalid deck tile");
}

BoardGameEngine::~BoardGameEngine()
{
  delete m_GameServer;
}

//BoardGameEngine* BoardGameEngine::GetInstance()
//{
//  return safe_cast< BoardGameEngine* > ( CoreEngine::GetInstance() );
//}


/************************************************************************************************
* AddRenderObject:
* Adds a 3D render object to the rendered ( to the Render engine's renderlist )
*
* @param  (RenderObject*)   renderObject - The render object to add to the render engine
*
* @author Thor Richard Hansen
*************************************************************************************************/
#ifdef CLIENT
void BoardGameEngine::AddRenderObject( RenderObject* renderObject )
{
  m_Render->AddRenderObject( renderObject );
}
#endif

/************************************************************************************************
* RemoveRenderObject:
* Remove a 3D render object from the render engine
*
* @param  (RenderObject*)   renderObject - The render object to remove from the render engine
*
* @author Thor Richard Hansen
*************************************************************************************************/
#ifdef CLIENT
void BoardGameEngine::RemoveRenderObject( RenderObject* renderObject )
{
  m_Render->RemoveRenderObject( renderObject );
}
#endif

/************************************************************************************************
* PurchaseItem:
* When the user successfully purchases an item from the Google/Amazon item shop.
*
* @param  (uint32)  itemId        - Id of the item that was purchased
* @param  (String)  receipt       - The receipt for the purchase
*
* @author Thor Richard Hansen
*************************************************************************************************/
#ifdef CLIENT
void BoardGameEngine::PurchaseWindowsItem( uint32 itemCode )
{
  SendPacketToServer(new PurchaseItemFromDarkFactorNetworkPacket(itemCode));
}
void BoardGameEngine::PurchaseIOSItem( uint32 itemCode, const String& receipt )
{
  SendPacketToServer(new PurchaseItemFromAppleNetworkPacket(itemCode, receipt));
}
void BoardGameEngine::PurchaseAmazonItem( uint32 itemCode, const String& receiptId, const String& receipt )
{
  SendPacketToServer( new PurchaseItemFromAmazonNetworkPacket( itemCode, receiptId, receipt ) );
}
#endif

