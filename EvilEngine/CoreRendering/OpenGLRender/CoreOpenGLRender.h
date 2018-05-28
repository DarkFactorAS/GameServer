#ifndef EVILENGINE_CORERENDERING_COREOPENGLRENDER
#define EVILENGINE_CORERENDERING_COREOPENGLRENDER 1

#ifdef SERVER
  #error "This file is client only!";
#endif

#include "../RenderModule.h"
#include "EvilEngine/CoreLib/CoreMath/Matrix.h"
#include "EvilEngine/CoreLib/Signal/Signal.h"
#include "EvilEngine/CoreRendering/CoreTextureManager/FrameBuffer.h"

#include <vector>
#include <map>

#ifdef WINDOWS
  #include <windows.h>
//#elif defined( ANDROID )
//  #include <EGL/egl.h>
#endif

class RenderObject;
class RenderCamera;
class QuadRenderObject;
class Color;

class CoreOpenGLRender
{
public:
  CoreOpenGLRender();
  ~CoreOpenGLRender();

  struct DebugObject
  {
    RenderObject* m_RenderObject;
    float         m_Timeout;
    DebugObject( RenderObject* renderObject, float timeOut )
    {
      m_RenderObject  = renderObject;
      m_Timeout       = timeOut;
    }
  };

  enum SCREEN_MODE
  {
    SCREENMODE_WINDOWED,
    SCREENMODE_FULLSCREEN,
    SCREENMODE_BORDERLESS,
  };

#ifdef WINDOWS
  CoreOpenGLRender( HWND hHwnd );
#endif

  virtual bool                      InitOpenGL( int width, int height, int bits );
  virtual void                      DestroyOpenGL();
  virtual void                      ResizeScreen( int width, int height );
  virtual void                      ChangeResolution( int width, int height, SCREEN_MODE screenMode );
  virtual void                      DrawScene( float deltaTime );

  void                              FrameProcess( float deltaTime );
  void                              SetProjection( float* debug ){ s_Projection.Set(debug); }
  void                              AddRenderObject( RenderObject* renderObject );
  void                              RemoveRenderObject( RenderObject* renderObject );

  void                              AddRenderModule( RenderModule* renderModule );
  void                              RemoveRenderModule( RenderModule* renderModule );
  RenderModule*                     GetRenderModuleWithType( uint32 typeId );

  static bool                       GLCheck( const char* txt );

  int                               GetWidth() const{ return m_Width; }
  int                               GetHeight() const{ return m_Height; }
  SCREEN_MODE                       GetScreenMode() const{ return m_ScreenMode; }
  static float                      GetRatio() { return s_Ratio; }

  static void                       SetDefaultTexture( int textureId ){ m_DefaultTextureId = textureId; }
  static int                        GetDefaultTexture(){ return m_DefaultTextureId; }
  Vector3                           Get2DPosIn3D(const Vector2& screenPos) const;

  static Signal1<void,float>        s_SignalAspectRatioChanged;
  static Signal3<void,uint32,uint32,uint8> s_SignalRequestScreenResize;
  static Signal0<void>              s_SignalScreenResetStart;
  static Signal0<void>              s_SignalScreenResetDone;

  static void                       DebugDrawBox( const Vector3 position, float x, float y, float z, float timeout, const Color& color );
  static void                       DebugDrawLine( const Vector3 position1, const Vector3 position2, float timeout, const Color& color );

  static int                        m_Width;
  static int                        m_Height;

private:
  bool                              PostInitOpenGL( int width, int height, int bits );
  void                              DrawObjects( float deltaTime );

  static Matrix                     s_Projection;
  static Matrix                     s_MVP;
  static float                      s_Ratio;

  SCREEN_MODE                       m_ScreenMode;
  static int                        m_DefaultTextureId;

  std::vector<RenderModule*>        m_RenderModuleList;
  std::vector<RenderObject*>        m_RenderObjectList;
  static std::vector< DebugObject > s_DebugRenderObjects;

  RenderCamera*                     m_Camera;

  FrameBuffer*                      m_Framebuffer;
  QuadRenderObject*                 m_ScreenTexture;

  float                             m_Framerate;
  float                             m_UpdateFPS;
  float                             m_RenderDelta;
  int                               m_NumFrames;

#ifdef WINDOWS
  HWND                              m_Hwnd;
//#elif defined( ANDROID )
//  EGLDisplay   m_Display;
//  EGLSurface   m_Surface;
//  EGLContext   m_Context;
#endif
};

#endif /// EVILENGINE_CORERENDERING_COREOPENGLRENDER
