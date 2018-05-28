
#include "PreCompile.h"
#include "CoreGameDebugGui.h"
#include "EvilEngine/CoreNetwork/Client/NetworkClient.h"
#include "EvilEngine/CoreGuiToolkit/CoreGuiToolkitInclude.h"
#include "EvilGameEngine/CoreGameEngine/CoreScreenModule/CoreScreenModule.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameGui/DebugGuiResolutionDialog/DebugGuiResolutionDialog.h"


CoreGameDebugGui::CoreGameDebugGui( CoreGameEngine& parent, bool autoOpen ) :
  DebugGui( parent, autoOpen ),
  m_CoreGameEngine( parent )
{
}

void CoreGameDebugGui::ShowButtons( int btnId )
{
  DebugGui::ShowButtons( btnId );
  if ( m_Menu != NULL )
  {
    int networkMode = NetworkClient::NETWORKHOST_NONE;
    NetworkClient* networkClient = safe_cast< NetworkClient* > ( m_CoreGameEngine.GetNetwork() );
    if ( networkClient != NULL )
    {
      networkMode = networkClient->GetCurrentHost();
      int connectionIndex = 0;
      if ( networkClient->HasHostType(NetworkClient::NETWORKMODE_LOCALHOST) )
      {
        m_Menu->AddChild( CreateMenuButton(FLAG_HASCLIENTGM, "Server|LAN", connectionIndex++, 3, GuiResource_ButtonBackground, &CoreGameDebugGui::SlotEnableLocalhost, networkMode == NetworkClient::NETWORKMODE_LOCALHOST ) );
      }
      if ( networkClient->HasHostType(NetworkClient::NETWORKMODE_TESTLIVE) )
      {
        m_Menu->AddChild( CreateMenuButton(FLAG_HASCLIENTGM,"Server|Testlive", connectionIndex++, 3, GuiResource_ButtonBackground, &CoreGameDebugGui::SlotEnableTestLive, networkMode == NetworkClient::NETWORKMODE_TESTLIVE ) );
      }
      if ( networkClient->HasHostType(NetworkClient::NETWORKMODE_LIVE) )
      {
        m_Menu->AddChild( CreateMenuButton(FLAG_HASCLIENTGM,"Server|Live", connectionIndex++, 3, GuiResource_ButtonBackground, &CoreGameDebugGui::SlotEnableLive, networkMode == NetworkClient::NETWORKMODE_LIVE ) );
      }
      m_Menu->AddChild( CreateMenuButton(FLAG_HASCLIENTGM,"Show|Version", 0, 4, GuiResource_ButtonBackground, &CoreGameDebugGui::ShowVersionText, IsVersionShown() ) );
      m_Menu->AddChild( CreateMenuButton(FLAG_NONE, "Toggle|FPS", 1, 4, GuiResource_ButtonBackground, &CoreGameDebugGui::ToggleFPS, IsFPSShown() ) );

#if defined( PLATFORM_WINDOWS ) || defined( PLATFORM_OSX )
      m_Menu->AddChild(CreateMenuButton(FLAG_NONE, "Resolution|Window", 3, 2, GuiResource_ButtonBackground, &CoreGameDebugGui::SlotShowResolutionWindow, false));
#endif
    }
  }
}

void CoreGameDebugGui::SlotEnableLocalhost( int /*btnId*/ )
{
#ifdef CLIENT
  m_CoreGameEngine.ChangeServer( NetworkClient::NETWORKMODE_LOCALHOST );
  RefreshButtons();
#endif
}
void CoreGameDebugGui::SlotEnableTestLive( int /*btnId*/ )
{
#ifdef CLIENT
  m_CoreGameEngine.ChangeServer( NetworkClient::NETWORKMODE_TESTLIVE );
  RefreshButtons();
#endif
}
void CoreGameDebugGui::SlotEnableLive( int /*btnId*/ )
{
#ifdef CLIENT
  m_CoreGameEngine.ChangeServer( NetworkClient::NETWORKMODE_LIVE );
  RefreshButtons();
#endif
}

void CoreGameDebugGui::ShowVersionText( int /*btnId*/ )
{
#ifdef CLIENT
  CoreScreenModule* screenModule = m_CoreGameEngine.GetActiveScreenModule();
  if ( screenModule != NULL )
  {
    screenModule->ToggleVersionText();
    RefreshButtons();
  }
#endif
}

bool CoreGameDebugGui::IsVersionShown()
{
#ifdef CLIENT
  CoreScreenModule* screenModule = m_CoreGameEngine.GetActiveScreenModule();
  if ( screenModule != NULL )
  {
    return  screenModule->IsVersionShown();
  }
#endif
  return false;
}

void CoreGameDebugGui::ToggleFPS( int /*btnId*/ )
{
#ifdef CLIENT
  CoreScreenModule* screenModule = m_CoreGameEngine.GetActiveScreenModule();
  if ( screenModule != NULL )
  {
    screenModule->ToggleFPS();
    RefreshButtons();
  }
#endif
}

bool CoreGameDebugGui::IsFPSShown()
{
#ifdef CLIENT
  CoreScreenModule* screenModule = m_CoreGameEngine.GetActiveScreenModule();
  if ( screenModule != NULL )
  {
    return screenModule->IsFPSShown();
  }
#endif
  return false;
}

/************************************************************************************************
* SlotShowResolutionWindow:
* Debug window to display all the resolutions. This is a debug window that is accessible from
* all screen modules.
*
* @param  (int)    btnId    - Id of the button that triggered this window
*
* @author Thor Richard Hansen
*************************************************************************************************/
void CoreGameDebugGui::SlotShowResolutionWindow(int /* btnId */)
{
  SetMode( DEBUGMODE_RESOLUTION );
  RemoveChild( GetChildWithName("DebugResolution") );
  AddChild( new DebugGuiResolutionDialog( "DebugResolution", m_CoreGameEngine ) );
}
