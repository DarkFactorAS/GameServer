/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : CoreScreenModule
*
* Description : Basic implementation of screen module switching
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"

#include "EvilEngine/CoreLib/BasicTypes/BinaryStream/BinaryStream.h"
#include "EvilEngine/CoreLib/BasicTypes/Definitions/VolatileArgs.h"
#include "EvilEngine/CoreGuiToolkit/CoreGuiToolkitInclude.h"
#include "EvilEngine/CoreGuiToolkit/CoreGuiToolkit.h"
#include "EvilEngine/CoreRendering/RenderObjects/TextStringRenderObject.h"
#include "EvilEngine/CoreNetwork/Common/NetworkConnection.h"

#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/GameEnginePacketData.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameGui/CoreGameDebugGui/CoreGameDebugGui.h"

#include "CoreScreenModule.h"

#ifdef CLIENT
  String  CoreScreenModule::s_ServerVersionTxt = "";
  int     CoreScreenModule::s_Framerate = -1;
  bool    CoreScreenModule::s_ShowVersionText = false;
#endif

CoreScreenModule::CoreScreenModule( int screenId, CoreGameEngine& gameEngine ) :
  EngineModule( EngineModule::COREMODULETYPE_SCENE, &gameEngine ),
  m_GameEngine( gameEngine )
#ifdef CLIENT
  , m_HUD(NULL)
  , m_ScreenId( screenId )
  , m_FPSText( NULL )
  , m_VersionTxt( NULL )
  , m_DebugGui( NULL )
#endif
{
#ifdef CLIENT
  if ( s_Framerate >= 0 && m_FPSText == NULL )
  {
    ToggleFPS();
  }
  if ( s_ShowVersionText && m_VersionTxt == NULL )
  {
    ToggleVersionText();
  }

  m_HUD = new CoreGuiView("HUD", Vector2::s_Vec2Zero, Vector2::s_Vec2One);

  GuiDialogHelper::SetParent( m_HUD );
#else
  VOLATILE_ARG(screenId);
#endif
}

CoreScreenModule::~CoreScreenModule()
{
#ifdef CLIENT
  CoreGameEngine* gameEngine = GetCoreGameEngine();
  gameEngine->RemoveGuiObject( m_VersionTxt );
  m_VersionTxt = NULL;
  gameEngine->RemoveGuiObject( m_DebugGui );
  m_DebugGui = NULL;
  gameEngine->RemoveGuiObject( m_FPSText );
  m_FPSText = NULL;
#endif
}

CoreGameEngine* CoreScreenModule::GetCoreGameEngine()
{
  CoreGameEngine* gameEngine = safe_cast< CoreGameEngine* > (m_CoreEngine);
  return gameEngine;
}

void CoreScreenModule::SendPacketToServer(BaseNetworkPacket* packet)
{
#ifdef CLIENT
  CoreGameEngine* engine = GetCoreGameEngine();
  if (engine != NULL)
  {
    engine->SendPacketToServer(packet);
  }
#else
  VOLATILE_ARG(packet);
#endif
}

void CoreScreenModule::OnConnectionChanged(uint32 /*connectionInstance*/, uint32 conectionStatus)
{
  switch (conectionStatus)
  {
  case NetworkConnection::NETWORKSTATUS_FAILED:
    NetworkConnectionFailed();
    break;
  case NetworkConnection::NETWORKSTATUS_DISCONNECTED:
    NetworkServerDisconnected();
    break;
  }
}

/************************************************************************************************
* SlotPurchaseItem:
* Callback when the user presses the "Become Member" button. This button will forward the shop
* request up to iOS/Android/Windows and each platform will handle the request
*
* @param  (int)    btnId    - Id of the button that triggered this window
*
* @author Thor Richard Hansen
*************************************************************************************************/
void CoreScreenModule::SlotUpgradeToMember()
{
  CoreEngine* engine = GetEngine();
  if ( engine != NULL )
  {
    engine->AddMessage(CoreGameEngine::MESSAGE_PURCHASE_MEMBERSHIP);
  }
}

/************************************************************************************************
* SlotPurchaseItem:
* Callback when the user presses the "Become Member" button. This button will forward the shop
* request up to iOS/Android/Windows and each platform will handle the request
*
* @param  (int)    btnId    - Id of the button that triggered this window
*
* @author Thor Richard Hansen
*************************************************************************************************/
void CoreScreenModule::SlotRefreshItemPurchases()
{
  CoreEngine* engine = GetEngine();
  if (engine != NULL)
  {
    engine->AddMessage( CoreGameEngine::MESSAGE_PURCHASE_REFRESH );
  }
}

/************************************************************************************************
 * SlotPurchaseItem:
 * Callback when the user presses the "Become Member" button. This button will forward the shop
 * request up to iOS/Android/Windows and each platform will handle the request
 *
 * @param  (int)    btnId    - Id of the button that triggered this window
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
void CoreScreenModule::SlotHandlePurchase( const char* /* itemId */, const char* /*receipt*/ )
{
  //m_Parent.PurchaseIOSItem( 1, receipt );
}

void CoreScreenModule::ToggleVersionText()
{
#ifdef CLIENT
  if ( m_VersionTxt == NULL )
  {
    ShowVersionText();
    s_ShowVersionText = true;
  }
  else
  {
    GetCoreGameEngine()->RemoveGuiObject( m_VersionTxt );
    m_VersionTxt = NULL;
    s_ShowVersionText = false;
  }
#endif
}

bool CoreScreenModule::IsVersionShown()
{
#if defined CLIENT
  return ( s_ShowVersionText );
#else
  return false;
#endif
}

void CoreScreenModule::ToggleFPS()
{
#ifdef CLIENT
  if ( m_FPSText == NULL )
  {
    m_FPSText = new CoreGuiTextfield( "FPS : ", Vector2( 0.0f, -0.7f ), Vector2( 1.0f, 0.035f ), TextStringRenderObject::ALIGNMENT_CENTERLEFT  );
    m_FPSText->SetLayer( CoreGuiBase::LAYER_WINDOW );
    m_FPSText->SetColor( Color::Yellow );
    GetCoreGameEngine()->AddGuiObject( m_FPSText );

    if ( s_Framerate >= 0 )
    {
      UpdateFPS( (float)s_Framerate );
    }
  }
  else
  {
    GetCoreGameEngine()->RemoveGuiObject( m_FPSText );
    m_FPSText = NULL;
  }
#endif
}

bool CoreScreenModule::IsFPSShown()
{
#if defined CLIENT
  return ( m_FPSText != NULL );
#else
  return false;
#endif
}

void CoreScreenModule::UpdateFPS( float fps )
{
#ifdef CLIENT
  if ( m_FPSText != NULL )
  {
    s_Framerate = (int) fps;
    m_FPSText->SetText( String::FormatString("FPS : %d", s_Framerate ) );
  }
#else
  VOLATILE_ARG( fps );
#endif
}

void CoreScreenModule::ClearServerVersion()
{
#ifdef CLIENT
  s_ServerVersionTxt = String::zero;
  ShowVersionText();
#endif
}


void CoreScreenModule::ShowVersionText()
{
#ifdef CLIENT

  CoreGameEngine* gameEngine = GetCoreGameEngine();

  String versionText;
  if ( !s_ServerVersionTxt.empty() )
  {
    versionText = String::FormatString( "Version: Client : %s / Server : %s ", gameEngine->GetBuildVersion(), s_ServerVersionTxt.c_str() );
  }
  else
  {
    versionText = String::FormatString( "Version: Client : %s ", gameEngine->GetBuildVersion() );
  }

  if ( m_VersionTxt != NULL )
  {
    m_VersionTxt->SetText( versionText );
  }
  else
  {
    m_VersionTxt = new CoreGuiTextfield( versionText, Vector2( 0.0f, 0.97f ), Vector2( 1.0f, 0.035f ), TextStringRenderObject::ALIGNMENT_CENTERLEFT  );
    m_VersionTxt->SetLayer( CoreGuiBase::LAYER_WINDOW );
    m_VersionTxt->SetColor( Color::Yellow );
    GetCoreGameEngine()->AddGuiObject( m_VersionTxt );
  }

#endif
}

/************************************************************************************************
* ShowDebugGui:
* Show a debug button that enables the rest of the debug gui
*
* @param  (int)    btnId    - Id of the button that triggered this window
*
* @author Thor Richard Hansen
*************************************************************************************************/
void CoreScreenModule::ShowDebugGuiButton( bool autoOpen )
{
#if defined CLIENT
  CoreGameEngine* gameEngine = GetCoreGameEngine();

  gameEngine->RemoveGuiObject( m_DebugGui );
  m_DebugGui = new CoreGameDebugGui( *gameEngine, autoOpen );
  gameEngine->AddGuiObject( m_DebugGui );
  gameEngine->GetGuiToolkit()->SignalMouseLongTouch.Connect( m_DebugGui, &CoreGameDebugGui::SlotOnMouseLongTouch );

#else
  VOLATILE_ARG( autoOpen );
#endif
}
