/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : TextStringRenderObject
*
* Description : Render a text string on the screen
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "PreCompile.h"
#include "TextStringRenderObject.h"
#include "EvilEngine/CoreRendering/OpenGLRender/CoreOpenGLRender.h"
#include "EvilEngine/CoreEngine/CoreResourceManager/CoreResourceManager.h"
#include "EvilEngine/CoreEngine/CoreResourceManager/Resource.h"
#include "../Resources/FontAtlasTextureResource.h"
#include "EvilEngine/CoreLib/Utils/CoreUtils.h"

/************************************************************************************************
* TextStringRenderObject:
* Constructor
*
* @param  (const String&)   text        - The text to render
* @param  (const Vector3&)  position    - 3D world position of the object
* @param  (const Vector3&)  size        - 3D size of the object ( width, height, depth )
* @param  (int)             fontId      - What font to use
*
* @author Thor Richard Hansen
*************************************************************************************************/
TextStringRenderObject::TextStringRenderObject( const String& text, const Vector3& position, const Vector3& size, int fontId ) : 
  QuadRenderObject( position, size, ++RenderObject::s_ResourceId ),
  m_FontId( fontId ),
  m_Text( text ),
  m_Alignment( ALIGNMENT_CENTER ),
  m_Scale( 1.0f ),
  m_LoadedTextures( false ),
  m_WordWrap( 0 )
{
  Init();

  CoreOpenGLRender::s_SignalScreenResetStart.Connect( this, &TextStringRenderObject::Release );
  //CoreOpenGLRender::s_SignalScreenResetDone.Connect( this, &TextStringRenderObject::Release );
}

TextStringRenderObject::~TextStringRenderObject()
{
  //CoreOpenGLRender::s_SignalScreenResetStart.Disconnect( this, &TextStringRenderObject::Release );
  Release();  
}

/************************************************************************************************
* Release:
* Destroys all the string textures
*
* @author Thor Richard Hansen
*************************************************************************************************/
void TextStringRenderObject::Release()
{
  for ( size_t index = 0; index < m_TextLines.size(); index++ )
  {
    Textline& textline = m_TextLines[ index ];
    if ( textline.m_TextureId != 0 )
    {
      glDeleteTextures(1, &(textline.m_TextureId) );
    }
    delete textline.m_Texture;
  }
  m_TextLines.clear();

  m_LoadedTextures = false;
}

void TextStringRenderObject::SetTextScale( float scale )
{
  m_Scale = scale;
  Release();  
}

void TextStringRenderObject::SetWordWrap( int numCharacters)
{
  if ( numCharacters != m_WordWrap )
  {
    m_WordWrap = numCharacters;
    Release();
  }
}

/************************************************************************************************
* CreateTexture:
* Creates a texture from an image blob. This function is overloaded from RenderObject since
* this class creates the texture by combining multiple images ( font characters ) into one
* texture.
*
* @return (uint32)                  - Returns the texture id that was created ( or 0 if it failed )
*
* @author Thor Richard Hansen
*************************************************************************************************/
uint32 TextStringRenderObject::CreateTexture()
{
  if ( m_FontId == 0 )
  {
    return 0;
  }

  // Make sure that the font resource is ready
  CoreResourceManager* resourceManager = CoreResourceManager::GetInstance();
  if ( resourceManager == NULL )
  {
    return 0;
  }

  FontAtlasTextureResource* fontResource = ( FontAtlasTextureResource* ) resourceManager->GetResource( Resource::ResourceType_FontAtlasTexture, m_FontId );
  if ( fontResource == NULL || !fontResource->IsLoaded() )
  {
    return 0;
  }

  const Font* font = fontResource->GetFont();
  if ( font == NULL )
  {
    return 0;
  }

  Release();  
  m_LoadedTextures = true;

  float yy = m_Position.y;
  StringList textList;
  {
    ScopedMutex eventMutex( &m_TextMutex );

    if ( m_Text.empty() )
    {
      return false;
    }

    if ( m_WordWrap != 0 )
    {
      textList = StringList::WordWrap( m_Text, m_WordWrap );
    }
    else
    {
      textList = StringList::Split( m_Text, "\n" );
    }
  }
  if ( m_Alignment == ALIGNMENT_TOPLEFT || m_Alignment == ALIGNMENT_TOPCENTER || m_Alignment == ALIGNMENT_TOPRIGHT || textList.size() > 1 )
  {
    yy -= m_Size.y;
  }

  uint32 textureHeight = font->GetPixelSize();

  for ( StringList::iterator it = textList.begin(); it != textList.end(); ++it )
  {
    const String& text = *it;

    // Calculate width
    uint32 textureWidth  = 0;
    for ( size_t index = 0; index < text.length(); index++ )
    {
      const FontCharacter* fontChar = font->GetCharacter( text.CharacterAt( index ) );
      if ( fontChar != NULL )
      {
        textureWidth += fontChar->GetWidth();
      }
    }

    textureWidth    = CoreUtils::GetPowerOfTwo( textureWidth );

    // Create new texture
    int pixelX = 0;
    Bitmap* texture = new Bitmap( textureWidth, textureHeight, fontResource->GetFormat() );
    for ( size_t index = 0; index < text.length(); index++ )
    {
      const FontCharacter* fontChar = font->GetCharacter( text.CharacterAt( index ) );
      if ( fontChar != NULL )
      {
        texture->CopyArea( PointI( pixelX, 0 ), *fontChar->m_Bitmap, RectInt( 0, 0, fontChar->GetWidth(), fontChar->GetHeight() ), false );
        pixelX += fontChar->GetWidth();
      }
    }

    Vector3 adjustedSize  = Vector3(m_Size);
    Vector3 adjustedPos   = Vector3(m_Position);

    float ratio           = m_Size.y * 0.5f;
    float textureScale    = ( 1.0f * pixelX ) / textureWidth;
    float charScale       = ( 1.0f * pixelX ) / ( text.length() * font->GetPixelSize() );
    float finalScale      = charScale * text.length() * ratio * m_Scale;
    adjustedSize.x        = finalScale * 1.5f;
    adjustedSize.y        = adjustedSize.y * m_Scale;
    adjustedPos.y         = yy;
    //adjustedSize.z        = 0.1f;

    //SetTextureBufferLeft( textureScale );
    uint32 textureId      = TextureResource::CreateTexture( m_FontId, textureWidth, textureHeight, fontResource->GetFormat(), texture->m_Data );

    switch ( m_Alignment )
    {
      case ALIGNMENT_CENTERLEFT:
      case ALIGNMENT_TOPLEFT:
        adjustedPos.x     = m_Position.x - m_Size.x + adjustedSize.x;
        break;
      case ALIGNMENT_CENTERRIGHT:
      case ALIGNMENT_TOPRIGHT:
        adjustedPos.x     = m_Position.x + m_Size.x - adjustedSize.x;
        break;
      default:
        break;
    }

    m_TextLines.push_back( Textline( text, adjustedPos, adjustedSize, finalScale, textureScale, textureId, texture ) );

    //m_Size.x = finalScale;
    yy += adjustedSize.y * 2.0f;
  }

  //float diffY = ( m_Position.y - ( yy * 0.5f  ) ) - ( m_Position.y - ( m_Size.y * 0.5f ) );

  //// Make everything centered in Y
  //for ( std::vector< Textline >::iterator it = m_TextLines.begin(); it != m_TextLines.end(); ++it )
  //{
  //  Textline& textLine = *it;
  //  textLine.m_AdjustedPos.y -= diffY;
  //}

  // Adjust all lines to make them center
  //m_TextLines.push_back( Textline( text, adjustedPos, adjustedSize, finalScale, textureId, texture ) );


  return 0;
}

/************************************************************************************************
* SetText:
* Sets the text in the inputfield
*
* @param  (const String)      text - The text to set
*
* @author Thor Richard Hansen
*************************************************************************************************/
void TextStringRenderObject::SetText( const String& text )
{
  ScopedMutex eventMutex( &m_TextMutex );

  if ( !m_Text.Equals( text ) )
  {
    Release();
    m_Text            = text;
  }
}

void TextStringRenderObject::SetPosition( float x, float y, float z )
{
  float diffX = x - m_Position.x;
  float diffY = y - m_Position.y;

  for ( size_t index = 0; index < m_TextLines.size(); index++ )
  {
    Textline& textline = m_TextLines[ index ];
    textline.m_AdjustedPos.x += diffX;
    textline.m_AdjustedPos.y += diffY;
    textline.m_AdjustedPos.z = z;
  }
  m_Position.Set( x, y, z ); 
}

void TextStringRenderObject::SetSize( float x, float y, float z )
{
  QuadRenderObject::SetSize( x, y, z );
  Release();
}

/************************************************************************************************
* OnDraw:
* Renders this object in the openGL engine
*
* @param  (const Matrix&)  projection - What projection to use : 2D (gui) or 3D ( world )
* @return (bool)                      - Returns false if the object cannot be rendered.
*
* @author Thor Richard Hansen
*************************************************************************************************/
bool TextStringRenderObject::OnDraw( const Matrix& projection )
{
  CHECK_OPENGL

  if ( !m_LoadedTextures )
  {
    CreateTexture();
    AssignShader( false );
  }

  for ( size_t index = 0; index < m_TextLines.size(); index++ )
  {
    //glUseProgram( m_Shader );

    const Textline& textline = m_TextLines[ index ];

    // Set the rotation, position and size for this object
    Matrix modelViewMatrix;
    modelViewMatrix.Translate( textline.m_AdjustedPos );
    //modelViewMatrix.Rotate( Vector3::s_Vec3One, radX );
    modelViewMatrix.Scale( textline.m_AdjustedSize );

    // Apply the world matrix
    Matrix::Multiply( projection, modelViewMatrix, &modelViewMatrix );
    glUniformMatrix4fv(m_Uniform_MVP_Matrix, 1, GL_FALSE, (const GLfloat*) &modelViewMatrix.m[0] );

    // Set color
    glUniform4fv(m_Uniform_Tint, 1, m_Color.m);

    int fSize = sizeof(float) * RENDER_BUFFER_SIZE;
    glVertexAttribPointer(SHADER_ATTRIBUTE_POSITION,      3, GL_FLOAT, GL_FALSE,  fSize, &m_Buffer[ 0 ] );
    glVertexAttribPointer(SHADER_ATTRIBUTE_NORMALS,       3, GL_FLOAT, GL_FALSE,  fSize, &m_Buffer[ 3 ] );
    glEnableVertexAttribArray(SHADER_ATTRIBUTE_NORMALS);
    glEnableVertexAttribArray(SHADER_ATTRIBUTE_POSITION);

    if ( textline.m_TextureId != 0 )
    {
      SetTextureBufferLeft( textline.m_TextureScale );

      glEnableVertexAttribArray(SHADER_ATTRIBUTE_TEXTURE_COORD);
      glVertexAttribPointer(SHADER_ATTRIBUTE_TEXTURE_COORD, 2, GL_FLOAT, GL_FALSE,  fSize, &m_Buffer[ 6 ] );

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, textline.m_TextureId);
      glUniform1i(m_UniformTexture, 0);
    }

    glDrawArrays( GL_TRIANGLE_STRIP, 0, m_BufferIdx / RENDER_BUFFER_SIZE );
  }

  CHECK_OPENGL

  return true;
}

float TextStringRenderObject::GetTextWidth( size_t lineIndex )
{
  if ( lineIndex < m_TextLines.size() )
  {
    const Textline& textLine = m_TextLines[ lineIndex ];
    return textLine.m_AdjustedSize.x * 2;
  }
  return 0.0f;
}
