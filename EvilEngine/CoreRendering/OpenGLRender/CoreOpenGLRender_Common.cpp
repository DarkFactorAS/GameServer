/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : CoreOpenGLRender
*
* Description : Main loop for the openGL render engine
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "PreCompile.h"
#include "CoreOpenGLRender.h"

#include "EvilEngine/CoreIO/Logger/Logger.h"
#include "EvilEngine/CoreLib/Utils/CoreUtils.h"
#include "EvilEngine/CoreRendering/RenderObjects/RenderObject.h"
#include "EvilEngine/CoreRendering/Camera/RenderCamera.h"
#include "EvilEngine/CoreRendering/RenderObjects/QuadRenderObject.h"
#include "EvilEngine/CoreRendering/CoreShaderManager/CoreShaderManager.h"
#include "EvilEngine/CoreRendering/CoreTextureManager/CoreTextureManager.h"

#include <stdlib.h>

std::vector< CoreOpenGLRender::DebugObject > CoreOpenGLRender::s_DebugRenderObjects;

// HACK!
#include "EvilEngine/CoreEngine/CoreResourceManager/CoreResourceManager.h"
#include "../RenderObjects/CubeRenderObject.h"

Matrix CoreOpenGLRender::s_Projection;
Matrix CoreOpenGLRender::s_MVP;
float  CoreOpenGLRender::s_Ratio            = 1.0f;
int    CoreOpenGLRender::m_DefaultTextureId = 0;
int    CoreOpenGLRender::m_Width  = 1;
int    CoreOpenGLRender::m_Height = 1;

Signal1<void,float> CoreOpenGLRender::s_SignalAspectRatioChanged;
Signal0<void>       CoreOpenGLRender::s_SignalScreenResetStart;
Signal0<void>       CoreOpenGLRender::s_SignalScreenResetDone;
Signal3<void,uint32,uint32,uint8> CoreOpenGLRender::s_SignalRequestScreenResize;

CoreOpenGLRender::CoreOpenGLRender() : 
  m_Camera( NULL ),
  m_ScreenMode( CoreOpenGLRender::SCREENMODE_WINDOWED ),
  m_Framebuffer( NULL ),
  m_ScreenTexture( NULL ),
  m_UpdateFPS( 0.0f ),
  m_RenderDelta( 0.0f ),
  m_NumFrames( 0 )
{
  CHECK_OPENGL
    
  const GLubyte* openGLVersion = glGetString(GL_VERSION);
  LogInfoFMT("CoreOpenGLRender", "OpenGL version : %s", openGLVersion);

  const GLubyte* openGLVendor = glGetString(GL_VENDOR);
  LogInfoFMT("CoreOpenGLRender", "OpenGL vendor : %s", openGLVendor);

  //const GLubyte* openGLRender = glGetString(GL_RENDER);
  //LogInfoFMT("CoreOpenGLRender", "OpenGL render : %s", openGLRender);

  CHECK_OPENGL
}

/************************************************************************************************
* InitOpenGL:
* Initializes the open GL screen. This function sets up the different culling methods 
* and render settings.
*
* @param  (int)   width        - The render screen width
* @param  (int)   height       - The render screen height
* @param  (int)   bits         - The render screen color depth ( 16/24/32 bit )
* @return (bool)               - Returns true if the initialization succeeded
*
* @author Thor Richard Hansen
*************************************************************************************************/
bool CoreOpenGLRender::PostInitOpenGL(  int width, int height, int /*bits*/ )
{
  CHECK_OPENGL
    
  s_SignalScreenResetStart();

  m_Camera = RenderCamera::GetInstance();

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
  glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  ResizeScreen( width, height );

  // Send a signal that the screen is done resetting
  s_SignalScreenResetDone();
  
  CHECK_OPENGL
    
  return true;
}

void CoreOpenGLRender::DestroyOpenGL()
{
  for ( size_t index = 0; index < m_RenderObjectList.size(); index++ )
  {
    RenderObject* renderObject = m_RenderObjectList[ index ];
    delete renderObject;
  }
  m_RenderObjectList.clear();

  for ( size_t index = 0; index < s_DebugRenderObjects.size(); index++ )
  {
    RenderObject* renderObject = s_DebugRenderObjects[ index ].m_RenderObject;
    delete renderObject;
  }
  s_DebugRenderObjects.clear();

  delete m_ScreenTexture;
  m_ScreenTexture = NULL;
}

/************************************************************************************************
* ResizeScreen:
* Called when the openGL render window has changed in size. Recalculate the projection view
* matrix ( rescale the 3D world ). This also forwards the change to all render modules so they
* can take action if they want to.
*
* @param  (int)   width        - The render screen width
* @param  (int)   height       - The render screen height
*
* @author Thor Richard Hansen
*************************************************************************************************/
void CoreOpenGLRender::ResizeScreen( int width, int height )
{
  CHECK_OPENGL
    
  if (height==0)
  {
    height=1;
  }

  if ( ( m_Width == width && m_Height == height ) || height < 1 || width < 1 )
  {
    return;
  }

  m_Width   = width;
  m_Height  = height;
  s_Ratio   = (float)m_Width / (float)m_Height;

  glViewport(0,0,width,height);						// Reset The Current Viewport

  s_Projection.Perspective( 45.0f, s_Ratio, 0.125f, 100.0f);
  s_MVP.Identity();
  m_Camera->FrameProcess( 0.0f );
  const Matrix& viewMatrix = m_Camera->GetViewMatrix();
  Matrix::Multiply( viewMatrix, s_MVP, &s_MVP );
  Matrix::Multiply( s_Projection, s_MVP, &s_MVP );

  CoreTextureManager::DestroyFrameBufferTexture( m_Framebuffer );
#if defined PLATFORM_WINDOWS
  m_Framebuffer = CoreTextureManager::CreateFrameBufferTexture( width, height );
#endif  
  if ( m_Framebuffer != nullptr )
  {
    delete m_ScreenTexture;
    m_ScreenTexture = new QuadRenderObject( Vector2( 0.0f,0.0f ), Vector2( 1.0f, 1.0f ), 0 );
    m_ScreenTexture->AddTexture( new Texture( 0, m_Framebuffer->m_TextureId ) );
    m_ScreenTexture->AssignShader( false );
  }
  
  // Update all render modules
  for ( size_t index = 0; index < m_RenderModuleList.size(); index++ )
  {
    RenderModule* renderModule = m_RenderModuleList[ index ];
    renderModule->ResizeScreen( width, height );
  }

  s_SignalAspectRatioChanged( s_Ratio );
    
  CHECK_OPENGL
}

/************************************************************************************************
* FrameProcess:
* Draws all the objects and other stuff which is common
*
* @param  (float)   deltaTime    - Amount of time that passed since last call.
*
* @author Thor Richard Hansen
*************************************************************************************************/
void CoreOpenGLRender::FrameProcess( float deltaTime )
{
  long preTime    = CoreUtils::GetTime();
  DrawScene( deltaTime );
  long postTime   = CoreUtils::GetTime();

  float renderDelta = ( postTime - preTime ) / 1000.0f;

  m_NumFrames++;
  m_RenderDelta += renderDelta;
  m_UpdateFPS += deltaTime;

  if ( m_UpdateFPS > 5.0f )
  {
    m_Framerate = m_NumFrames / m_UpdateFPS;
    float percentInRender = ( 100.0f * m_RenderDelta ) / m_UpdateFPS;
    if ( m_Framerate < 50 )
    {
      LogInfoFMT("OpenGL","FPS: %0.2f [Frames: %d] [RenderTime : %0.2f - TotalTime: %0.2f ] %0.0f percent spent in rendering!", m_Framerate, m_NumFrames, m_RenderDelta, m_UpdateFPS, percentInRender );
    }
    m_NumFrames   = 0;
    m_UpdateFPS   = 0.0f;
    m_RenderDelta = 0.0f;
  }
}

/************************************************************************************************
* DrawScene:
* Goes through all render objects and render modules and draws them on the scene. 
*
* @param  (float)   deltaTime    - Amount of time that passed since last call.
*
* @author Thor Richard Hansen
*************************************************************************************************/
void CoreOpenGLRender::DrawObjects( float deltaTime )
{
  CHECK_OPENGL

  if ( m_Camera != NULL )
  {
    m_Camera->FrameProcess( deltaTime );
    const Matrix& viewMatrix = m_Camera->GetViewMatrix();
    s_MVP.Identity();
    Matrix::Multiply( viewMatrix, s_MVP, &s_MVP );
    Matrix::Multiply( s_Projection, s_MVP, &s_MVP );
  }

  glUseProgram( CoreShaderManager::GetInstance()->GetTextureShader() );
  CHECK_OPENGL

  // Clear and initialize the framebuffer
  if ( m_Framebuffer != NULL && m_Framebuffer->m_FrameBufferId != 0 )
  {
    // Clear the screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0,0, m_Width, m_Height );
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    CHECK_OPENGL
    
    // Clear the framebuffer
    glBindFramebuffer( GL_FRAMEBUFFER, m_Framebuffer->m_FrameBufferId );
    glViewport(0,0, m_Framebuffer->m_Width, m_Framebuffer->m_Height );
    glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Framebuffer->m_TextureId, 0);
    glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_Framebuffer->m_DepthRenderBufferId );
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    CHECK_OPENGL
  }
  else
  {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    CHECK_OPENGL
  }
    

  // Run all 3D render objects
  for ( std::vector< RenderObject* >::iterator itObj = m_RenderObjectList.begin(); itObj != m_RenderObjectList.end(); )
  {
    RenderObject* renderObject = *itObj;
    if ( !renderObject->HasDestroy() )
    {
      if ( !renderObject->HasFlag( CoreBaseObject::FLAGS_ISINVISIBLE ) )
      {
        renderObject->OnDraw( s_MVP );
      }
      itObj++;
    }
    else
    {
      itObj = m_RenderObjectList.erase( itObj );
    }
  }

  // Run all 3D render objects
#ifdef DEBUG
  for ( std::vector< DebugObject >::iterator itObj = s_DebugRenderObjects.begin(); itObj != s_DebugRenderObjects.end();  )
  {
    DebugObject& obj = *itObj;

    if ( obj.m_Timeout > deltaTime )
    {
      obj.m_Timeout -= deltaTime;
      obj.m_RenderObject->OnDraw( s_MVP );
      itObj++;
    }
    else
    {
      obj.m_Timeout = 0.0f;
      itObj = s_DebugRenderObjects.erase( itObj );
    }
  }
#endif

  // Run all rendermodules
  for ( size_t index = 0; index < m_RenderModuleList.size(); index++ )
  {
    RenderModule* renderModule = m_RenderModuleList[ index ];
    if ( renderModule->m_ModuleId != 10 || m_Framebuffer == NULL || m_Framebuffer->m_FrameBufferId == 0 )
    {
      renderModule->OnDraw( s_MVP );
    }
  }

  // Render our frame buffer to screen
  if ( m_Framebuffer != NULL && m_Framebuffer->m_FrameBufferId != 0 )
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0,0,m_Width,m_Height);

    Matrix proj;
    proj.Ortographic( -1, 1, -1, 1, 0.1f, 100.0f );
    m_ScreenTexture->OnDraw( proj );

    CHECK_OPENGL

    // Run all rendermodules
    for ( size_t index = 0; index < m_RenderModuleList.size(); index++ )
    {
      RenderModule* renderModule = m_RenderModuleList[ index ];
      if ( renderModule->m_ModuleId == 10 )
      {
        renderModule->OnDraw( s_MVP );
        CHECK_OPENGL
      }
    }
  }
}

/************************************************************************************************
* AddRenderObject:
* Add a render object to the list. This list contains all the render objects. If it is not here
* it is not rendered. The render engine takes ownership of the render object so it should not
* hold a reference outside of this class. ( Since this class will delete it when it is removed )
*
* @param  (RenderObject*)   renderObject - The render object to add
*
* @author Thor Richard Hansen
*************************************************************************************************/
void CoreOpenGLRender::AddRenderObject( RenderObject* renderObject )
{
  if ( renderObject != NULL )
  {
    m_RenderObjectList.push_back( renderObject );
  }
}

/************************************************************************************************
* RemoveRenderObject:
* Remove a render object from the list. This list contains all the render objects. If it is not here
* it is not rendered. The render engine takes ownership of the render object so it should not
* hold a reference outside of this class. ( Since this class will delete it when it is removed )
*
* @param  (RenderObject*)   renderObject - The render object to remove
*
* @author Thor Richard Hansen
*************************************************************************************************/
void CoreOpenGLRender::RemoveRenderObject( RenderObject* renderObject )
{
  if ( renderObject == NULL )
  {
    return;
  }

  // Run all 3D render objects
  for ( std::vector<RenderObject*>::iterator itRender = m_RenderObjectList.begin(); itRender != m_RenderObjectList.end(); ++itRender )
  {
    if ( *itRender == renderObject )
    {
      m_RenderObjectList.erase( itRender );
      delete renderObject;
      return;
    }
  }
}

/************************************************************************************************
* AddRenderModule:
* Adds a render module to the render engine. Render modules are used to render custom render
* objects like gui or particles that are not part of the normal 3D models.
*
* @param  (RenderModule)       - The rendermodule we want to add
*
* @author Thor Richard Hansen
*************************************************************************************************/
void CoreOpenGLRender::AddRenderModule( RenderModule* newRenderModule )
{
  for ( std::vector<RenderModule*>::iterator itModule = m_RenderModuleList.begin(); itModule != m_RenderModuleList.end(); itModule++ )
  {
    RenderModule* renderModule = *itModule;
    if ( renderModule->m_ModuleId > newRenderModule->m_ModuleId )
    {
      m_RenderModuleList.insert( itModule, newRenderModule );
      return;
    }
  }
  m_RenderModuleList.push_back( newRenderModule );
}

/************************************************************************************************
* RemoveRenderModule:
* Removes a render module from the render engine. Render modules are used to render custom render
* objects like gui or particles that are not part of the normal 3D models.
*
* @param  (RenderModule)       - The rendermodule we want to remove
*
* @author Thor Richard Hansen
*************************************************************************************************/
void CoreOpenGLRender::RemoveRenderModule( RenderModule* removeRenderModule )
{
  for ( std::vector<RenderModule*>::iterator itModule = m_RenderModuleList.begin(); itModule != m_RenderModuleList.end(); itModule++ )
  {
    RenderModule* renderModule = *itModule;
    if ( renderModule == removeRenderModule )
    {
      m_RenderModuleList.erase( itModule );
      return;
    }
  }
}

RenderModule* CoreOpenGLRender::GetRenderModuleWithType( uint32 typeId )
{
  for ( std::vector<RenderModule*>::iterator itModule = m_RenderModuleList.begin(); itModule != m_RenderModuleList.end(); itModule++ )
  {
    RenderModule* renderModule = *itModule;
    if ( renderModule->m_ModuleId == typeId )
    {
      return renderModule;
    }
  }
  return NULL;
}

void CoreOpenGLRender::DebugDrawBox( const Vector3 position, float x, float y, float z, float timeout, const Color& color )
{
  CubeRenderObject* render = new CubeRenderObject( position, Vector3( x, y, z ), 10006 );
  render->SetColor( color );
  s_DebugRenderObjects.push_back( DebugObject( render, timeout ) );
}

void CoreOpenGLRender::DebugDrawLine( const Vector3 position1, const Vector3 position2, float timeout, const Color& color )
{
  DebugDrawBox( position1, 0.05f, 0.05f, 0.05f, timeout, color );
  DebugDrawBox( position2, 0.05f, 0.05f, 0.05f, timeout, color );

  Vector3 diff   = position2 - position1;
  Vector3 midPos = position1 + ( (position2 - position1) * 0.5f );
  Vector3 size   = Vector3( Vec3Length( midPos ) * 0.4f, 0.05f, 0.05f );

  //Vector3 rot    = Vector3::Normalize(diff);
  Quaternion quat ( diff, Vector3::s_Vec3One );
  Vector3 testX = Vector3::s_Vec3Forward * quat;
  //Vector3 testY = Vector3::s_Vec3Up * quat;
  //Vector3 testZ = Vector3::s_Vec3Right * quat;

  float x = dfDeg( testX.x );
  //float y = dfDeg( testY.x );
  //float z = dfDeg( testZ.x );

  //Vector3 test = Vector3(1,1,1) * quat;



  CubeRenderObject* render = new CubeRenderObject( midPos, size, 10006 );
  render->SetColor( color );
  render->SetRotation( x, 0, 0 );
  s_DebugRenderObjects.push_back( DebugObject( render, timeout ) );
}

Vector3 CoreOpenGLRender::Get2DPosIn3D(const Vector2& screenPos) const
{

  //const Matrix& viewMatrix = m_Camera->GetViewMatrix();

  /*

  GLdouble x,y,z;

  const double* m = s_MVP.GetAsDoublePointer();
  const double* p = s_Projection.GetAsDoublePointer();
  int* v = new int[4];
  v[0] = -1;
  v[1] = -1;
  v[2] = 1;
  v[3] = 1;

  gluUnProject( screenPos.x, screenPos.y, zoom, 
    m, 
    p,
    v, 
    &x, 
    &y, 
    &z);

  Vector3 worldPos((float)x,(float)y,(float)z);

  */

  Matrix viewProj( s_MVP );
  Matrix viewProjInv = MatInverse( viewProj );

  float zoom = m_Camera->GetZoom();
  //Vector3 pos = m_Camera->GetPosition();
  //Vector3 tPos = m_Camera->GetTargetPosition();

  Vector3 screenFar( screenPos.x, -screenPos.y, 0.0f );
  Vector3 screenNear( screenPos.x, -screenPos.y, zoom );
  Vector3 worldPos1 = Matrix::Multiply( viewProjInv, screenFar );
  Vector3 worldPos2 = Matrix::Multiply( viewProjInv, screenNear );
  //Vector3 worldPos2 = pos - worldPos1;

  /*
  LogInfoFMT("Render","Convert 2D [%0.2f x %0.2f] => 3D near [%0.2f x %0.2f x %0.2f] 3D far [%0.2f x %0.2f x %0.2f]", 
    screenPos.x, screenPos.y, 
    worldPos1.x, worldPos1.y, worldPos1.z ,
    worldPos2.x, worldPos2.y, worldPos2.z 
    );
    */

  //DebugDrawLine( worldPos1, worldPos2, 5.0f, Color::Yellow );

  DebugDrawBox( worldPos1, 0.01f, 0.01f, 0.01f, 2.0f, Color::Yellow );
  DebugDrawBox( worldPos2, 0.01f, 0.01f, 0.01f, 2.0f, Color::Green );

  return worldPos1;

  //float yProPlaneSize = tanf( 45.0f / 2.0F );
  //float xProPlaneSize = yProPlaneSize * s_Ratio;

  //Vector3 beam(screenPos.x * xProPlaneSize, -screenPos.y * yProPlaneSize, 1);
  //Vector3 globalBeam = beam * m_Camera->GetRotiation();
  //Vector3 beamDirection = Vector3::Normalize( globalBeam );
  //Vector3 dist = m_Camera->GetPosition() - m_Camera->GetTargetPosition();
  //float distance  = Vec3Length( Vector3::ProjectXZ( dist ) );
  //return m_Camera->GetPosition() + beamDirection * distance;
}

/*
static const __m128 ONES = _mm_set1_ps(1.0f);

// Project a single 3D point x using the 3x4 projection matrix P
// float2 out, float4 projection, float3 vector
void Project(float* out, const float* P, const Vector3* x)
{ 
  // (1) Load [X,Y,Z,1] into xyz1
  __m128 xy = _mm_loadl_pi(_mm_setzero_ps(), (const __m64*)x); // X Y
  __m128 z =  _mm_load_ss(&x->z);    // Z 0 0 0
  __m128 z1 = _mm_unpacklo_ps(z, ONES); // Z 1
  __m128 xyz1 = _mm_movelh_ps(xy, z1);  // X Y Z 1

  // (2) Load the P matrix
  __m128 P0 = _mm_loadu_ps((const float*)(P + 0));
  __m128 P1 = _mm_loadu_ps((const float*)(P + 1));
  __m128 P2 = _mm_loadu_ps((const float*)(P + 2));

  // (3) multiply with the P matrix
  __m128 tx = _mm_mul_ps(P0, xyz1);
  __m128 ty = _mm_mul_ps(P1, xyz1);
  __m128 tz = _mm_mul_ps(P2, xyz1);

  // use horizontal adds to sum up the X, Y and Z components of t
  // txyzz contains [t.x, t.y, t.z, t.z]
  __m128 txy =   _mm_hadd_ps(tx, ty);
  __m128 tzz =   _mm_hadd_ps(tz, tz);
  __m128 txyzz = _mm_hadd_ps(txy, tzz); 

  // (4) now divide the X and Y components of t by the Z component

  // move the Z component from high to low
  tzz = _mm_movehl_ps(txyzz, txyzz); 

  // divide by the Z component of t
  __m128 ixy = _mm_div_ps(txyzz, tzz);

  // Store the result
  _mm_storel_pi((__m64*)out, ixy);
}

*/
