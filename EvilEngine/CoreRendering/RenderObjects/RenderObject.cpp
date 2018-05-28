/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : RenderObject
*
* Description : The base class for a render object. This class initializes all the common 
*               properties like shaders and textures.
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "PreCompile.h"
#include "RenderObject.h"

#include "../OpenGLRender/CoreOpenGLRender.h"
#include "../CoreShaderManager/CoreShaderManager.h"
#include "EvilEngine/CoreEngine/CoreResourceManager/Resource.h"
#include "EvilEngine/CoreRendering/CoreTextureManager/CoreTextureManager.h"

#include "EvilEngine/CoreIO/Logger/Logger.h"

uint32  RenderObject::s_ResourceId = 0;
bool    RenderObject::s_DrawWireframe = false;

#if defined DEBUG && defined PLATFORM_WINDOWS
bool    RenderObject::s_DrawInvisibleAlpha = true;
#else
bool    RenderObject::s_DrawInvisibleAlpha = false;
#endif

RenderObject::RenderObject() : 
  CoreBaseObject(),
  m_Wireframe( false ),
  m_Buffer( NULL ),
  m_BufferIdx( 0 ),
  m_BufferSize( 0 ),
  m_Texture( NULL ),
  m_Color( 1.0f, 1.0f, 1.0f, 1.0f ),
  m_Shader(0),
  m_UniformTexture(0)
{
}

/************************************************************************************************
* RenderObject:
* Constructor
*
* @param  (const Vector3&)  position    - 3D world position of the object
* @param  (const Vector3&)  size        - 3D size of the object ( width, height, depth )
* @param  (int)             resourceId  - The resource/texture to use for this object
*
* @author Thor Richard Hansen
*************************************************************************************************/
RenderObject::RenderObject( const Vector3& position, const Vector3& size, uint32 resourceId ) : 
  CoreBaseObject( position, size, resourceId ),
  m_Wireframe( false ),
  m_Buffer( NULL ),
  m_BufferIdx( 0 ),
  m_BufferSize( 0 ),
  m_Texture( NULL ),
  m_Shader(0),
  m_UniformTexture(0),
  m_Color( 1.0f, 1.0f, 1.0f, 1.0f )
{
}

RenderObject::~RenderObject()
{
  if ( m_Texture != NULL && m_Texture->m_ResourceId != 0 )
  {
    CoreTextureManager::GetInstance()->ReleaseTexture( m_Texture->m_TextureId );
  }

  free(m_Buffer);
  m_Buffer = NULL;

  // TODO : release the shader
  //m_Shader
}


void RenderObject::SetResourceId( uint32 resourceId )
{
  if ( m_Texture != NULL && m_Texture->m_ResourceId != resourceId )
  {
    CoreTextureManager::GetInstance()->ReleaseTexture( m_Texture->m_TextureId );
    m_Texture = NULL;
  }
  CoreBaseObject::SetResourceId( resourceId );
}

void RenderObject::SetResourceId( int /*instanceId*/, uint32 resourceId )
{
  SetResourceId( resourceId );
}

/************************************************************************************************
* SetVertices:
* Constructor
*
* @param  (int)             numVertices - The number of vertices this object has
*
* @author Thor Richard Hansen
*************************************************************************************************/
void RenderObject::SetVertices( int numVertices )
{
  m_BufferSize      = numVertices * RENDER_BUFFER_SIZE;
  m_Buffer          = new GLfloat[ m_BufferSize ];
  m_BufferIdx       = 0;
}

/************************************************************************************************
* AddPoint:
* Adds a vertex, normal, texture point to the buffer
*
* @param  (float)      vertexX  - X position of the vertex
* @param  (float)      vertexY  - Y position of the vertex
* @param  (float)      vertexZ  - Z position of the vertex
* @param  (float)      normalX  - X position of the normal
* @param  (float)      normalY  - Y position of the normal
* @param  (float)      normalZ  - Z position of the normal
* @param  (float)      textureX - X position of the texture
* @param  (float)      textureY - Y position of the texture
*
* @author Thor Richard Hansen
*************************************************************************************************/
void RenderObject::AddPoint( float vertexX, float vertexY, float vertexZ,
                            float normalX, float normalY, float normalZ,
                            float textureX, float textureY )
{
  if ( m_BufferIdx < m_BufferSize + RENDER_BUFFER_SIZE )
  {
    m_Buffer[ m_BufferIdx++ ] = vertexX;
    m_Buffer[ m_BufferIdx++ ] = vertexY;
    m_Buffer[ m_BufferIdx++ ] = vertexZ;
    m_Buffer[ m_BufferIdx++ ] = normalX;
    m_Buffer[ m_BufferIdx++ ] = normalY;
    m_Buffer[ m_BufferIdx++ ] = normalZ;
    m_Buffer[ m_BufferIdx++ ] = textureX;
    m_Buffer[ m_BufferIdx++ ] = textureY;
  }
}

/************************************************************************************************
* AddPoint:
* Adds a vertex, normal, texture point to the buffer
*
* @param  (float)      vertexX  - X position of the vertex
* @param  (float)      vertexY  - Y position of the vertex
* @param  (float)      vertexZ  - Z position of the vertex
* @param  (float)      normalX  - X position of the normal
* @param  (float)      normalY  - Y position of the normal
* @param  (float)      normalZ  - Z position of the normal
* @param  (float)      textureX - X position of the texture
* @param  (float)      textureY - Y position of the texture
*
* @author Thor Richard Hansen
*************************************************************************************************/
void RenderObject::AddPoint( const Vector3& vertex, const Vector3& normal, const Vector2& texture )
{
  return AddPoint( vertex.x, vertex.y, vertex.z, normal.x, normal.y, normal.z, texture.x, texture.y );
}
void RenderObject::AddPoint( float vertexX, float vertexY, float vertexZ )
{
  return AddPoint( vertexX, vertexY, vertexZ, Vector3::s_Vec3Up, 0.5f, 0.5f );
}
void RenderObject::AddPoint( const Vector3& position, const Vector3& normal )
{
  return AddPoint( position.x, position.y, position.z, normal, 0.5f, 0.5f );
}
void RenderObject::AddPoint( float positionX, float positionY, float positionZ, const Vector3& normal, float textureScaleX, float textureScaleY )
{
  return AddPoint( positionX, positionY, positionZ, normal.x, normal.y, normal.z, 0.5f + ( positionX * textureScaleX ), 0.5f + ( positionY * textureScaleY ) );
}

void RenderObject::AssignShader()
{
  AssignShader( m_Texture == NULL );
}

void RenderObject::AssignShader( bool useLineShader )
{
  CHECK_OPENGL

  if ( !useLineShader )
  {
    m_Shader = CoreShaderManager::GetInstance()->GetTextureShader();
  }
  else
  {
    m_Shader = CoreShaderManager::GetInstance()->GetDebugLineShader();
  }

  glBindAttribLocation( m_Shader, SHADER_ATTRIBUTE_POSITION,      "vPosition");
  glBindAttribLocation( m_Shader, SHADER_ATTRIBUTE_TEXTURE_COORD, "vTexcoord");
  glBindAttribLocation( m_Shader, SHADER_ATTRIBUTE_NORMALS,       "vNormal");

  m_UniformTexture      = glGetUniformLocation( m_Shader, "texture" );
  m_Uniform_MVP_Matrix  = glGetUniformLocation( m_Shader, "modelViewProjectionMatrix" );
  m_Uniform_Normals     = glGetUniformLocation( m_Shader, "normalMatrix" );
  m_Uniform_Tint        = glGetUniformLocation( m_Shader, "vColor" );
}

/************************************************************************************************
* OnDraw:
* Will create the texture on demand when we try to draw this render object.
*
* @param  (const Matrix&)  projection - What projection to use : 2D (gui) or 3D ( world )
* @return (bool)                      - Returns false if the object cannot create the texture.
*
* @author Thor Richard Hansen
*************************************************************************************************/
bool RenderObject::OnDraw( const Matrix& projection )
{
  if ( m_BufferIdx == 0 )
  {
    return false;
  }

  if ( m_Buffer == NULL )
  {
    return false;
  }

  if ( m_Texture == NULL )
  {
    m_Texture = CoreTextureManager::GetInstance()->GetTexture( m_ResourceId );
    AssignShader();
  }
  if (m_Texture == NULL)
  {
    return true;
  }

  //if ( !m_Wireframe )
  //{
  //  glUseProgram( m_Shader );
  //}
  //else
  //{
  //  glUseProgram( CoreShaderManager::GetInstance()->GetDebugLineShader() );
  //}

  // Set the rotation, position and size for this object
  Matrix modelViewMatrix = m_LocalMatrix;

  // Apply the world matrix
  Matrix::Multiply( projection, modelViewMatrix, &modelViewMatrix );
  glUniformMatrix4fv(m_Uniform_MVP_Matrix, 1, GL_FALSE, (const GLfloat*) &modelViewMatrix.m[0] );

  // Multiplying the end matrix with inverse of projection should get the local coordinates ?? wtf
  //Matrix viewProjInv = MatInverse( projection );
  //Matrix modelPlain;
  //Matrix::Multiply( modelViewMatrix, viewProjInv, &modelPlain );

  bool drawWire = s_DrawWireframe || m_Wireframe;

  // Set Color
  if ( RenderObject::s_DrawInvisibleAlpha && m_Color.GetAlpha() <= 0.0f )
  {
    glUniform4fv(m_Uniform_Tint, 1, Color::Red.m);
    drawWire = true;
  }
  else if ( RenderObject::s_DrawInvisibleAlpha && ( m_Texture == NULL || m_Texture->m_TextureId == 0 ) )
  {
    glUniform4fv(m_Uniform_Tint, 1, Color::Yellow.m);
    drawWire = true;
  }
  else
  {
    glUniform4fv(m_Uniform_Tint, 1, m_Color.m);
  }

#if defined PLATFORM_WINDOWS
  if ( drawWire )
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }
#endif

  int fSize = sizeof(float) * RENDER_BUFFER_SIZE;
  glVertexAttribPointer(SHADER_ATTRIBUTE_POSITION,      3, GL_FLOAT, GL_FALSE,  fSize, &m_Buffer[ 0 ] );
  glVertexAttribPointer(SHADER_ATTRIBUTE_NORMALS,       3, GL_FLOAT, GL_FALSE,  fSize, &m_Buffer[ 3 ] );
  glEnableVertexAttribArray(SHADER_ATTRIBUTE_POSITION);
  glEnableVertexAttribArray(SHADER_ATTRIBUTE_NORMALS);

  if ( m_Texture != NULL && m_Texture->m_TextureId != 0 )
  {
    glVertexAttribPointer(SHADER_ATTRIBUTE_TEXTURE_COORD, 2, GL_FLOAT, GL_FALSE,  fSize, &m_Buffer[ 6 ] );
    glEnableVertexAttribArray(SHADER_ATTRIBUTE_TEXTURE_COORD);

    if ( !drawWire )
    {
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, m_Texture->m_TextureId);
      glUniform1i(m_UniformTexture, 0);
    }
    glDrawArrays( GL_TRIANGLE_STRIP, 0, m_BufferIdx / RENDER_BUFFER_SIZE );
  }
  else if ( drawWire )
  {
    glDrawArrays( GL_TRIANGLE_STRIP, 0, m_BufferIdx / RENDER_BUFFER_SIZE );
  }

#if defined PLATFORM_WINDOWS
  if ( drawWire )
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );
  }
#endif

  return true;
}
