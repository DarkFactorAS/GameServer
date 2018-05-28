/************************************************************************************************
 * Project     : OpenGLDemo 2014
 * File        : CoreShaderManager
 * Description : Shadermanager compiles and holds all shaders used on the renderobjects
 *
 * Author      : Thor Richard Hansen
 *************************************************************************************************/

#include "PreCompile.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "CoreShaderManager.h"
#include "EvilEngine/CoreEngine/CoreResourceManager/CoreResourceManager.h"
#include "../RenderObjects/RenderObject.h"

CoreShaderManager* CoreShaderManager::s_Instance = NULL;

CoreShaderManager::CoreShaderManager() :
  m_TextureShader( 0 ),
  m_DebugLineShader( 0 )
{
  CHECK_OPENGL
    
  const GLubyte* openGLVersion = glGetString(GL_VERSION);
  LogInfoFMT("CoreShaderManager", "OpenGL version : %s", openGLVersion);

  const GLubyte* openGLVendor = glGetString( GL_VENDOR );
  LogInfoFMT("CoreShaderManager", "OpenGL vendor : %s", openGLVendor);
    
  CHECK_OPENGL
}

/************************************************************************************************
 * GetInstance:
 * Returns the singleton instance of the shader manager
 *
 * @return (CoreShaderManager) - Shader manager singleton pointer
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
CoreShaderManager* CoreShaderManager::GetInstance()
{
  if ( s_Instance == NULL )
  {
    s_Instance = new CoreShaderManager();
  }
  return s_Instance;
}

/************************************************************************************************
 * LoadShader:
 * Loads a vertex/fragment shader set.
 *
 * @param  (int)           type   - What kind of shader this is ( Vertex or Fragment )
 * @param  (const char*)   source - text source of the shader
 * @param  (const int)     length - length of the source buffer ( in bytes )
 * @return (int)                  - Returns the unique id for this compiled shader
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
bool CoreShaderManager::LoadShader( int shaderType, const char* shadername )
{
  CoreResourceManager* resourceManager = CoreResourceManager::GetInstance();
  if ( resourceManager == NULL )
  {
    return false;
  }

  String vertexShaderFilename        = String::FormatString( "%s.vsh", shadername );
  String fragmentShaderFilename      = String::FormatString( "%s.fsh", shadername );

  ResourceRef* vertexShader          = resourceManager->LoadResource( vertexShaderFilename.c_str() );
  ResourceRef* fragmentShader        = resourceManager->LoadResource( fragmentShaderFilename.c_str() );

  if ( vertexShader == NULL || fragmentShader == NULL )
  {
    dfAssert( false, "Failed to load shaders!" );
    LogErrorFMT("CoreShaderManager", "Loading resource [%s] [FAILED]", vertexShaderFilename.c_str() );
    LogErrorFMT("CoreShaderManager", "Loading resource [%s] [FAILED]", fragmentShaderFilename.c_str() );
    free( vertexShader );
    free( fragmentShader );
    return false;
  }

  int shaderId = ShaderBuild( vertexShaderFilename.c_str(), (const char*)vertexShader->m_Data, (int)vertexShader->m_Filesize, 
                              fragmentShaderFilename.c_str(), (const char*)fragmentShader->m_Data, (int)fragmentShader->m_Filesize );
  if ( shaderId == 0 )
  {
    free( vertexShader );
    free( fragmentShader );
    return false;
  }

  LogInfoFMT("CoreShaderManager", "Compiling/Linking shader [%s] [SUCCEEDED]", vertexShaderFilename.c_str() );
  LogInfoFMT("CoreShaderManager", "Compiling/Linking shader [%s] [SUCCEEDED]", fragmentShaderFilename.c_str() );

  switch( shaderType )
  {
    case SHADERTYPE_TEXTURE:
      m_TextureShader = shaderId;
      return true;
    case SHADERTYPE_DEBUGLINE:
      m_DebugLineShader = shaderId;
      return true;
    default:
      break;
  }
  return false;
}

/************************************************************************************************
 * ShaderCompile:
 * Compiles one specific shader source file.
 *
 * @param  (int)           type   - What kind of shader this is ( Vertex or Fragment )
 * @param  (const char*)   source - text source of the shader
 * @param  (const int)     length - length of the source buffer ( in bytes )
 * @return (int)                  - Returns the unique id for this compiled shader
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
int CoreShaderManager::ShaderCompile( const int type, const char* filename, const char* source, const int length)
{
  GLuint shader_object_id = 0;
  if ( length > 0 )
  {
    shader_object_id = glCreateShader(type);
    GLint compile_status;
    
    glShaderSource(shader_object_id, 1, (const GLchar **)&source, &length);
    glCompileShader(shader_object_id);
    glGetShaderiv(shader_object_id, GL_COMPILE_STATUS, &compile_status);
    
    if (compile_status == GL_FALSE)
    {
      int loglen;
      char logbuffer[1000];
      glGetShaderInfoLog(shader_object_id, sizeof(logbuffer), &loglen, logbuffer);
      dfAssertFMT( compile_status != GL_FALSE, "Failed to compiler shader [ %s ]. Error = %s", filename, logbuffer );
      return 0;
    }
  }
  return shader_object_id;
}

/************************************************************************************************
 * ShaderLink:
 * Links the fragmentshader and vertexshader into a compiled shader that can be used
 * on the renderobjects. All renderobjects has to have a shader to be rendered
 *
 * @param  (const uint)    vertexShader   - Id of the compiled vertexshader
 * @param  (const uint)    fragmentShader - Id of the compiled fragmentshader
 * @return (int)                          - Returns the unique id for the linked shader
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
int CoreShaderManager::ShaderLink(const int vertexShader, const int fragmentShader)
{
  if ( vertexShader == 0 || fragmentShader == 0 )
  {
    return 0;
  }
    
  GLuint program_object_id = glCreateProgram();
  GLint link_status;

  glAttachShader(program_object_id, vertexShader);
  glAttachShader(program_object_id, fragmentShader);

  // on iOS, the bind variables has to be set before we link the shader
  glBindAttribLocation(program_object_id, RenderObject::SHADER_ATTRIBUTE_POSITION,        "vPosition");
  glBindAttribLocation(program_object_id, RenderObject::SHADER_ATTRIBUTE_NORMALS,         "vNormal");
  glBindAttribLocation(program_object_id, RenderObject::SHADER_ATTRIBUTE_TEXTURE_COORD,   "vTexcoord");
  glBindAttribLocation(program_object_id, RenderObject::SHADER_ATTRIBUTE_COLOR,           "vColor");
  
  glLinkProgram(program_object_id);
  glGetProgramiv(program_object_id, GL_LINK_STATUS, &link_status);

  if (link_status == GL_FALSE) 
  {
    int loglen;
    char logbuffer[1000];
    glGetShaderInfoLog(program_object_id, sizeof(logbuffer), &loglen, logbuffer);
    dfAssertFMT( link_status != GL_FALSE, "Failed to link shaders! Error = %s", logbuffer );
  }

  return program_object_id;
}

/************************************************************************************************
 * ShaderBuild:
 * Wrapper function to take the vertex and fragmentshader source and compile and 
 * link them together.
 *
 * @param  (const char*)   vertexShader         - Source buffer for the vertexshader
 * @param  (const uint)    vertexShaderLength   - Length of the vertexshader source
 * @param  (const char*)   fragmentShader       - Source buffer for the fragmentshader
 * @param  (const uint)    fragmentShaderLength - Length of the fragmentshader source
 * @return (int)                                - Returns the unique id for the linked shader
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
int CoreShaderManager::ShaderBuild( const char* vertex_shader_name, const char* vertex_shader_source, const int   vertex_shader_source_length, 
                                    const char* fragment_shader_name, const char* fragment_shader_source, const int   fragment_shader_source_length ) 
{
  GLuint vertex_shader   = ShaderCompile( GL_VERTEX_SHADER, vertex_shader_name, vertex_shader_source, vertex_shader_source_length);
  GLuint fragment_shader = ShaderCompile( GL_FRAGMENT_SHADER, fragment_shader_name, fragment_shader_source, fragment_shader_source_length);
  return ShaderLink(vertex_shader, fragment_shader);
}
