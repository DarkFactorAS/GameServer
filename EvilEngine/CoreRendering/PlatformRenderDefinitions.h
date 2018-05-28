#pragma once

// Include the platform specific render includes
#ifdef ANDROID

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h> 

#elif WINDOWS

#define GLEW_STATIC
#include <GL/glew.h>

#elif PLATFORM_IOS

#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

#elif PLATFORM_OSX

  #include <GLUT/glut.h>
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>

#endif

#define CHECK_OPENGL \
{ \
  int glError = glGetError(); \
  switch( glError ) \
  { \
    case 0: \
      break; \
    case 1282: \
      LogFatalFMT( "CoreRender", "OpenGL error = 'Shader error' (%s:%d)", __FILE__, __LINE__ );\
      break; \
    case 1286: \
      LogFatalFMT( "CoreRender", "OpenGL error = 'Invalid framebuffer operation' (%s:%d)", __FILE__, __LINE__ );\
      break; \
    default: \
      LogFatalFMT( "CoreRender", "OpenGL error = %d (%s:%d)", glError, __FILE__, __LINE__ );\
      break;\
  } \
}
