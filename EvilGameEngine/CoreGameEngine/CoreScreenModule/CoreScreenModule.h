#ifndef EVILENGINE_COREGAMEENGINE_CORESCREENMODULE
#define EVILENGINE_COREGAMEENGINE_CORESCREENMODULE 1
#pragma once

#include "EvilEngine/CoreLib/Signal/SignalTarget.h"
#include "EvilEngine/CoreGuiToolkit/GuiTemplates/GuiButtonTemplate.h"
#include "EvilEngine/CoreGuiToolkit/GuiTemplates/GuiDialogHelper.h"
#include "EvilEngine/CoreGuiToolkit/CoreGuiToolkitInclude.h"

#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"

class CoreGuiView;
class CoreGuiTextfield;
class BinaryStream;

class CoreScreenModule : public EngineModule, public GuiButtonTemplate, public GuiDialogHelper
{
public:
  CoreScreenModule( int screenId, CoreGameEngine& gameEngine );
  virtual ~CoreScreenModule();

#ifdef CLIENT
  CoreScreenModule& operator  =   ( const CoreScreenModule& other )
  {
    EngineModule::operator=(other);
    m_HUD                 = other.m_HUD;
    m_GameEngine          = other.m_GameEngine;
    return *this;
  }
#endif

  virtual String            GetModuleName() DF_OVERRIDE { return StaticStr("CoreScreenModule"); }

  virtual void              OnMouseDrag(float /* xPos */, float /* yPos */) {};

  /// In App Purchase functions
  void                      SlotUpgradeToMember();
  void                      SlotRefreshItemPurchases();
  void                      SlotHandlePurchase( const char* itemId, const char* receipt );

  /// TODO fix : Depricated : While netowrk hack
  virtual void              SlotReceivedNetworkData(uint32 /*methodId*/, const BinaryStream* /*data*/) {};

  virtual void              OnConnectionChanged(uint32 /*connectionInstance*/, uint32 /*conectionStatus*/);
  virtual void              NetworkConnectionFailed(){};
  virtual void              NetworkServerDisconnected(){};

  virtual void              ShowDebugGuiButton( bool autoOpen );

  void                      ToggleVersionText();
  bool                      IsVersionShown();
  void                      ShowVersionText();
  void                      ToggleFPS();
  bool                      IsFPSShown();
  void                      UpdateFPS( float fps );
  void                      ClearServerVersion();

  /// access method to send a network packet to the server
  void                      SendPacketToServer(BaseNetworkPacket* packet);

  CoreGameEngine*           GetCoreGameEngine();
  CoreGameEngine&           m_GameEngine;

#ifdef CLIENT
  uint32                    GetScreenId() const { return m_ScreenId; }

  CoreGuiView*              m_HUD;
  CoreGuiTextfield*         m_FPSText;
  static int                s_Framerate;
  static bool               s_ShowVersionText;
  CoreGuiTextfield*         m_VersionTxt;
  static String             s_ServerVersionTxt;
  CoreGuiBase*              m_DebugGui;
  uint32                    m_ScreenId;
#endif
};

#endif /// EVILENGINE_COREGAMEENGINE_CORESCREENMODULE