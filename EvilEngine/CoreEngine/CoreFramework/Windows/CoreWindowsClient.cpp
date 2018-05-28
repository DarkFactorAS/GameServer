
#include "Precompile.h"

#if defined PLATFORM_WINDOWS

#include "CoreWindowsClient.h"
#include "EvilEngine/CoreLib/Utils/CoreUtils.h"
#include "EvilEngine/CoreIO/UserConfig/UserConfig.h"

CoreWindowsClient::CoreWindowsClient( const char* winName, const char* winDisplayName ) :
  CoreWindowsCommon( winName, winDisplayName ),
  m_Width( 1 ),
  m_Height( 1 )
{
}

HWND CoreWindowsClient::CreateClientWindow( int iconId )
{
  RECT actualDesktop;
  GetWindowRect(GetDesktopWindow(), &actualDesktop);
  m_Width  = actualDesktop.right - actualDesktop.left;
  m_Height = actualDesktop.bottom - actualDesktop.top;

  // Load the window setting if we have it available
  UserConfig* appConfig = new UserConfig("Settings");
  if ( appConfig->Load() )
  {
    int32 width  = (int32)appConfig->GetUInt32("width");
    int32 height = (int32)appConfig->GetUInt32("height");
    //int32 mode   = appConfig->GetInt32("mode");
    if ( width > 32 && width < m_Width && height > 28 && height < m_Height )
    {
      m_Width = width;
      m_Height = height;
    }
  }

  HINSTANCE hInstance   = GetModuleHandle(NULL);				// Grab An Instance For Our Window
  HICON hIcon           = LoadIcon(hInstance, MAKEINTRESOURCE( iconId ));

  HWND		hWnd=NULL;		// Holds Our Window Handle
  WNDCLASS	wc;						// Windows Class Structure
  RECT		WindowRect;
  WindowRect.left   = (long)  0;		
  WindowRect.right  = (long)  m_Width;		
  WindowRect.top    = (long)  0;		
  WindowRect.bottom = (long)  m_Height;	

  wc.style              = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
  wc.lpfnWndProc        = (WNDPROC) WndProc;					// WndProc Handles Messages
  wc.cbClsExtra		= 0;									// No Extra Window Data
  wc.cbWndExtra		= 0;									// No Extra Window Data
  wc.hInstance		= hInstance;							// Set The Instance
  wc.hIcon              = hIcon;			// Load The Default Icon
  wc.hCursor            = LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
  wc.hbrBackground      = NULL;									// No Background Required For GL
  wc.lpszMenuName       = NULL;									// We Don't Want A Menu
  wc.lpszClassName      = CoreUtils::ConvertCharArrayToLPCWSTR( m_WinShortName );							

  if (!RegisterClass(&wc))									// Attempt To Register The Window Class
  {
    //MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
    return NULL;
  }

  AdjustWindowRectEx(&WindowRect, WS_OVERLAPPEDWINDOW, FALSE,  WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);

  // Create The Window
  hWnd = CreateWindowEx( WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,							// Extended Style For The Window
    CoreUtils::ConvertCharArrayToLPCWSTR( m_WinShortName ) ,							// Class Name
    CoreUtils::ConvertCharArrayToLPCWSTR( m_WinDisplayName ),                                // Window Title
    WS_OVERLAPPEDWINDOW |							// Defined Window Style
    WS_CLIPSIBLINGS |					// Required Window Style
    WS_CLIPCHILDREN,					// Required Window Style
    0, 0,								// Window Position
    WindowRect.right-WindowRect.left,	// Calculate Window Width
    WindowRect.bottom-WindowRect.top,	// Calculate Window Height
    NULL,								// No Parent Window
    NULL,								// No Menu
    hInstance,							// Instance
    NULL);								// Dont Pass Anything To WM_CREATE
  if ( !hWnd )
  {
    //MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
    return NULL;
  }

  ShowWindow(hWnd,SW_SHOW);						// Show The Window
  SetForegroundWindow(hWnd);						// Slightly Higher Priority
  SetFocus(hWnd);									// Sets Keyboard Focus To The Window

  return hWnd;
}

#endif /// PLATFORM_WINDOWS
