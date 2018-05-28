
#include "Precompile.h"

#ifdef CLIENT
#include "DebugGuiResolutionDialog.h"
#include "EvilEngine/CoreGuiToolkit/GuiElements/CoreGuiWindow.h"
#include "EvilEngine/CoreRendering/OpenGLRender/CoreOpenGLRender.h"
//#include "EvilEngine/CoreGameEngine/CoreResourceManager/CoreResourceManager.h"
#include "EvilEngine/CoreIO/UserConfig/UserConfig.h"

DebugGuiResolutionDialog::DebugGuiResolutionDialog( const String& windowName, CoreGameEngine& parent) :
  CoreGuiXMLView( "DebugGuiResolutionDialog.xml"),
  m_Parent( parent )
{
  SetName( windowName );
}

void DebugGuiResolutionDialog::OnDataLoaded( Resource* /*resource*/ )
{
  CoreGuiWindow* guiWindow = (CoreGuiWindow*) GetChildWithName("window");
  if ( guiWindow != NULL )
  {
    guiWindow->m_SignalButtonDown.Connect( this, &DebugGuiResolutionDialog::SlotChangeResolution );
  }

  ConnectButton( "BtnSamsungS3", &DebugGuiResolutionDialog::SlotChangeResolution );
  ConnectButton( "BtnIpadMini", &DebugGuiResolutionDialog::SlotChangeResolution );
  //ConnectButton( "BtnHpLaptop", &DebugGuiResolutionDialog::SlotChangeResolution );
  ConnectButton( "BtnFullscreen", &DebugGuiResolutionDialog::SlotChangeResolution );
  ConnectButton("BtnBorderless", &DebugGuiResolutionDialog::SlotChangeResolution);

  ConnectButton( "BtnRes1", &DebugGuiResolutionDialog::SlotChangeResolution );
  ConnectButton( "BtnRes2", &DebugGuiResolutionDialog::SlotChangeResolution );
  ConnectButton( "BtnRes3", &DebugGuiResolutionDialog::SlotChangeResolution );
  ConnectButton( "BtnRes4", &DebugGuiResolutionDialog::SlotChangeResolution);

  ConnectButton( "BtnClose", &DebugGuiResolutionDialog::SlotCloseWindow );
}

void DebugGuiResolutionDialog::SlotCloseWindow( int /*btnId*/ )
{
  Destroy();
}

/************************************************************************************************
* SlotChangeResolution:
* Callback for when the user selects a resolution in the resolution window.
*
* @param  (int)    btnId    - Id of the button that triggered this callback
*
* @author Thor Richard Hansen
*************************************************************************************************/
void DebugGuiResolutionDialog::SlotChangeResolution( int btnId )
{
  CoreOpenGLRender* render = m_Parent.GetRender();
  if ( render == NULL )
  {
    return;
  }

  int width   = 0;
  int height  = 0;
  CoreOpenGLRender::SCREEN_MODE screenMode = CoreOpenGLRender::SCREENMODE_WINDOWED;

  switch( btnId )
  {
  case RESOLUTION_SAMSUNG_S3:
    width   = 800;
    height  = 400;
    break;
  case RESOLUTION_IPAD_MINI:
    width   = 1024;
    height  = 768;
    break;
  case RESOLUTION_HP_LAPTOP:
    width   = 1600;
    height  = 900;
    break;
  case RESOLUTION_320:
    width   = 320;
    height  = 240;
    break;
  case RESOLUTION_640:
    width   = 640;
    height  = 640;
    break;
  case RESOLUTION_1024:
    width   = 1024;
    height  = 600;
    break;
  case RESOLUTION_1920:
    width   = 1920;
    height  = 1200;
    break;
  case RESOLUTION_TOGGLE_FULLSCREEN:
    width       = -1;
    height      = -1;
    screenMode  = ( m_Parent.GetRender()->GetScreenMode() == CoreOpenGLRender::SCREENMODE_WINDOWED ) ? CoreOpenGLRender::SCREENMODE_FULLSCREEN : CoreOpenGLRender::SCREENMODE_WINDOWED;
    break;
  case RESOLUTION_BORDERLESS:
    width       = -1;
    height      = -1;
    screenMode  = CoreOpenGLRender::SCREENMODE_BORDERLESS;
    break;
  default:
    return;
  }

  render->ChangeResolution( width, height, screenMode );

  // Load the window setting if we have it available
  UserConfig* appConfig = new UserConfig("Settings");
  appConfig->SetUInt32("width", width );
  appConfig->SetUInt32("height", height );
  appConfig->SetUInt32("mode", screenMode );
  appConfig->Save();
}

#endif