/*************************************************************************************************
 * Project     : EvilEngine Multiplatform 2014
 * File        : CoreOpenGLRender_OSX.cpp
 *
 * Description : Specific code needed to initialize thing in openGL on the OSX platform
 *
 * Author      : Thor Richard Hansen
 *************************************************************************************************/
#include "PreCompile.h"

#ifdef PLATFORM_OSX
//#import <OpenGLES/ES2/gl.h>
//#import <OpenGLES/ES2/glext.h>

#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

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

void CoreOpenGLRender::ChangeResolution( int width, int height, SCREEN_MODE screenMode )
{
  s_SignalRequestScreenResize( width, height, screenMode );
}

void CoreOpenGLRender::DrawScene( float deltaTime )
{
    DrawObjects( deltaTime );
}
#endif