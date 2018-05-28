#ifndef EVILENGINE_COREBOARDGAME_GAMEENGINE_BOARDGAMEENGINE
#define EVILENGINE_COREBOARDGAME_GAMEENGINE_BOARDGAMEENGINE 1
#pragma once

#include "EvilEngine/CoreLib/Signal/SignalTarget.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"

class AccountManager;
class BoardGameServer;
class RenderObject;
class CoreGuiBase;
class CoreGuiToolkit;

class BoardGameEngine : public CoreGameEngine
{
public:
  BoardGameEngine();
  virtual ~BoardGameEngine();
  //static BoardGameEngine*           GetInstance();

  BoardGameServer*                  GetBoardGameServer() const { return m_GameServer; }
  BoardGameServer*                  GetGameServer() { return m_GameServer; }

#ifdef CLIENT
  void                              AddRenderObject( RenderObject* renderObject );
  void                              RemoveRenderObject( RenderObject* renderObject );

  void                              PurchaseWindowsItem( uint32 itemCode );
  void                              PurchaseIOSItem( uint32 itemCode, const String& receipt );
  void                              PurchaseAmazonItem( uint32 itemCode, const String& receiptId, const String& receipt );

#endif

protected:
  BoardGameServer*                  m_GameServer;
};

#endif /// EVILENGINE_COREBOARDGAME_GAMEENGINE_BOARDGAMEENGINE
