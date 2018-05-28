/*************************************************************************************
* Project     : OpenGLDemo 2014
* File        : CoreOpenGLRender_Windows
* Description : Windows specific code to initialize and run the openGL(es) render
*               engine.
*               
* Author      : Thor Richard Hansen
*************************************************************************************/

#include "PreCompile.h"

#ifdef WINDOWS

#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library

#include "CoreOpenGLRender.h"
#include "../CoreShaderManager/CoreShaderManager.h"

CoreOpenGLRender::CoreOpenGLRender( HWND hHwnd ) :
  m_Camera( NULL ),
  m_ScreenMode( CoreOpenGLRender::SCREENMODE_WINDOWED ),
  m_Hwnd( hHwnd ),
  m_UpdateFPS( 0.0f ),
  m_RenderDelta( 0.0f ),
  m_NumFrames( 0 )
{
}

CoreOpenGLRender::~CoreOpenGLRender()
{
  ChangeDisplaySettings(0, 0);
  DestroyOpenGL();
}

bool CoreOpenGLRender::InitOpenGL( int width, int height, int bits )
{
  HDC hDC = GetDC( m_Hwnd );
  if ( hDC == NULL )
  {
    return false;
  }

  static PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
  {
    sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
    1,											// Version Number
    PFD_DRAW_TO_WINDOW |						// Format Must Support Window
    PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
    PFD_DOUBLEBUFFER,							// Must Support Double Buffering
    PFD_TYPE_RGBA,								// Request An RGBA Format
    (BYTE)bits,										// Select Our Color Depth
    0, 0, 0, 0, 0, 0,							// Color Bits Ignored
    0,											// No Alpha Buffer
    0,											// Shift Bit Ignored
    0,											// No Accumulation Buffer
    0, 0, 0, 0,									// Accumulation Bits Ignored
    16,											// 16Bit Z-Buffer (Depth Buffer)  
    0,											// No Stencil Buffer
    0,											// No Auxiliary Buffer
    PFD_MAIN_PLANE,								// Main Drawing Layer
    0,											// Reserved
    0, 0, 0										// Layer Masks Ignored
  };

  GLuint pixelFormat = ChoosePixelFormat(hDC,&pfd);
  if ( pixelFormat == 0 )
  {
    return false;
  }

  if( !SetPixelFormat(hDC,pixelFormat,&pfd) )
  {
    return false;
  }

  // Permanent Rendering Context
  HGLRC	hRC = wglCreateContext(hDC);
  if ( hRC == NULL )
  {
    return false;
  }

  // Bind the openGL view to the window view
  if ( !wglMakeCurrent(hDC,hRC) )
  {
    wglDeleteContext(hRC);
    return false;
  }

  // Initialize GLEW
  if (GLEW_OK != glewInit())
  {
    wglDeleteContext(hRC);
    return false;
  }

  bool hasShader      = CoreShaderManager::GetInstance()->LoadShader( CoreShaderManager::SHADERTYPE_TEXTURE, "WindowsShaders/TextureShader");
  bool hasDebugShader = CoreShaderManager::GetInstance()->LoadShader( CoreShaderManager::SHADERTYPE_DEBUGLINE, "WindowsShaders/ColorShader");
  return ( hasShader && hasDebugShader && PostInitOpenGL(width, height, bits ) );
}

void CoreOpenGLRender::ChangeResolution( int width, int height, SCREEN_MODE screenMode )
{
  if ( width == -1 ){   width = m_Width;   }
  if ( height == -1 ){  height = m_Height; }

  RECT rect;
  rect.left         = 0;
  rect.top          = 0;
  rect.right        = width;
  rect.bottom       = height;

  int oldScreenMode = m_ScreenMode;
  m_ScreenMode      = screenMode;

  switch( screenMode )
  {
    case SCREENMODE_WINDOWED:
      {
        // Center the window on the desktop
        RECT actualDesktop;
        GetWindowRect(GetDesktopWindow(), &actualDesktop);
        int posX = ( actualDesktop.right - actualDesktop.left ) / 2 - ( rect.right - rect.left ) / 2;
        int posY = ( actualDesktop.bottom - actualDesktop.top ) / 2 - ( rect.bottom - rect.top ) / 2;

        SetWindowLongPtr(m_Hwnd, GWL_STYLE, WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE | WS_MINIMIZEBOX | WS_MAXIMIZEBOX );
        MoveWindow( m_Hwnd, posX, posY, rect.right-rect.left, rect.bottom-rect.top, true );

        if ( oldScreenMode == SCREENMODE_FULLSCREEN )
        {
          ChangeDisplaySettings(0, 0);
        }
      }
      break;

    case SCREENMODE_FULLSCREEN:
      {
        SetWindowLongPtr(m_Hwnd, GWL_STYLE, WS_SYSMENU | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE);
        AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
        MoveWindow( m_Hwnd, 0, 0, rect.right-rect.left, rect.bottom-rect.top, true );

        DEVMODE dm;
        dm.dmSize = sizeof(DEVMODE);
        dm.dmPelsWidth  = 1024;//width;
        dm.dmPelsHeight = 768;//height;
        dm.dmBitsPerPel = 32;
        dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
        if ( ChangeDisplaySettings(&dm, 0) != DISP_CHANGE_SUCCESSFUL )
        {
          ChangeResolution( width, height, SCREENMODE_WINDOWED );
          dfAssert( false, "CoreOpenGLRender::ChangeResolution. Could not set fullscreen!" );
        }
      }
      break;

    case SCREENMODE_BORDERLESS:
      {
        // Center the window on the desktop
        RECT actualDesktop;
        GetWindowRect(GetDesktopWindow(), &actualDesktop);
        width  = actualDesktop.right - actualDesktop.left;
        height = actualDesktop.bottom - actualDesktop.top;

        rect.right  = width;
        rect.bottom = height;

        //SetWindowLongPtr(m_Hwnd, GWL_STYLE, WS_SYSMENU | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE);
        SetWindowLongPtr(m_Hwnd, GWL_STYLE, WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE);
        AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
        MoveWindow( m_Hwnd, 0, 0, rect.right-rect.left, rect.bottom-rect.top, true );

        if ( oldScreenMode == SCREENMODE_FULLSCREEN )
        {
          ChangeDisplaySettings(0, 0);
        }
      }
      break;
  }
}

void CoreOpenGLRender::DrawScene( float deltaTime )
{
  DrawObjects( deltaTime );

  HDC hDC = GetDC( m_Hwnd );
  SwapBuffers(hDC);
}

#endif