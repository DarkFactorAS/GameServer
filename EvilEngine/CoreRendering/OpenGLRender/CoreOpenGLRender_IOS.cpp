/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : IOSOpenGLScreen.cpp
*
* Description : Specific code needed to initialize thing in openGL on the iOS platform
*
* Author      : Thor Richard Hansen
*************************************************************************************************/
#include "PreCompile.h"

#ifdef PLATFORM_IOS
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

#include "CoreOpenGLRender.h"
#include "../CoreShaderManager/CoreShaderManager.h"

CoreOpenGLRender::~CoreOpenGLRender()
{
  DestroyOpenGL();
}

bool CoreOpenGLRender::InitOpenGL( int width, int height, int bits )
{
  bool hasShader      = CoreShaderManager::GetInstance()->LoadShader( CoreShaderManager::SHADERTYPE_TEXTURE, "TextureShader");
  bool hasDebugShader = CoreShaderManager::GetInstance()->LoadShader( CoreShaderManager::SHADERTYPE_DEBUGLINE, "ColorShader");
  return ( hasShader && hasDebugShader && PostInitOpenGL(width, height, bits ) );
}

void CoreOpenGLRender::ChangeResolution( int /*width*/, int /*height*/, SCREEN_MODE /*screenMode*/ )
{
}

void CoreOpenGLRender::DrawScene( float deltaTime )
{
  DrawObjects( deltaTime );
}
#endif