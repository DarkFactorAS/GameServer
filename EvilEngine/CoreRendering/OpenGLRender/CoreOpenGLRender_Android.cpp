/*************************************************************************************
* Project     : OpenGLDemo 2014
* File        : CoreOpenGLRender_Android
* Description : Android specific code to initialize and run the openGL(es) render
*               engine.
*               
* Author      : Thor Richard Hansen
*************************************************************************************/

#include "PreCompile.h"

#ifdef ANDROID
#include <jni.h>
//#include <errno.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h> 


#include <math.h>

#include "CoreOpenGLRender.h"
#include "../CoreShaderManager/CoreShaderManager.h"

// The simple framework expects the application code to define these functions.
extern void appInit();
extern void appDeinit();
extern void appRender(long tick, int width, int height);

CoreOpenGLRender::~CoreOpenGLRender()
{
  DestroyOpenGL();
}

bool CoreOpenGLRender::InitOpenGL( int width, int height, int bits )
{
  bool hasShader      = CoreShaderManager::GetInstance()->LoadShader( CoreShaderManager::SHADERTYPE_TEXTURE, "shaders/TextureShader");
  bool hasDebugShader = CoreShaderManager::GetInstance()->LoadShader( CoreShaderManager::SHADERTYPE_DEBUGLINE, "shaders/ColorShader");
  return ( hasShader && hasDebugShader && PostInitOpenGL(width, height, bits ) );
}

void CoreOpenGLRender::ChangeResolution( int width, int height, SCREEN_MODE screenMode )
{
}

void CoreOpenGLRender::DrawScene( float deltaTime )
{
    //glClear(GL_COLOR_BUFFER_BIT);
    DrawObjects( deltaTime );
    //eglSwapBuffers(m_Display, m_Surface);
}

#endif
