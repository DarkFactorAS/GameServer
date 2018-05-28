/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : TextureResource
*
* Description : Resource that contains an image that is used for textures
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "PreCompile.h"
#include "TextureResource.h"
#include "../RenderObjects/RenderObject.h"
#include "EvilEngine/CoreEngine/CoreResourceManager/CoreResourceManager.h"
#include "EvilEngine/CoreEngine/CoreResourceManager/Resource.h"
#include "EvilEngine/CoreLib/Utils/CoreUtils.h"

TextureResource::TextureResource( uint32 resourceInstance, const char * filename, bool isFlipped ) :
  ImageLoaderResource( ResourceType_Texture, resourceInstance, filename, isFlipped )
{
}

TextureResource::TextureResource( int resourceType, uint32 resourceInstance, const char * filename, bool isFlipped ) :
  ImageLoaderResource( resourceType, resourceInstance, filename, isFlipped )
{
}

TextureResource::TextureResource( int resourceType, uint32 resourceInstance, const char * filename, uint32 width, uint32 height, uint32 format, uint8_t* data, bool isFlipped ) :
  ImageLoaderResource( resourceType, resourceInstance, filename, isFlipped )
{
  m_Format  = format;
  m_Width   = width;
  m_Height  = height;
  m_Data    = data;
}

/************************************************************************************************
 * CreateTexture:
 * Creates a texture from an image blob.
 *
 * @param  (uint32)      width       - Width of the texture we want to create
 * @param  (uint32)      height      - Height of the texture we want to create
 * @param  (uint32)      imageFormat - What kind of format the image data is in ( RGB, RGBA, etc )
 * @param  (const void*) imageData   - The data for the image to create the texture from
 * @return (uint32)                  - Returns the texture id that was created ( or 0 if it failed )
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
uint32 TextureResource::CreateTexture()
{
  return TextureResource::CreateTexture( m_ResourceId, m_Width, m_Height, m_Format, m_Data );
}

/************************************************************************************************
* CreateTexture:
* Creates a texture from an image blob.
*
* @param  (uint32)      width       - Width of the texture we want to create
* @param  (uint32)      height      - Height of the texture we want to create
* @param  (uint32)      imageFormat - What kind of format the image data is in ( RGB, RGBA, etc )
* @param  (const void*) imageData   - The data for the image to create the texture from
* @return (uint32)                  - Returns the texture id that was created ( or 0 if it failed )
*
* @author Thor Richard Hansen
*************************************************************************************************/
uint32 TextureResource::CreateTexture( uint32 resourceId, uint32 width, uint32 height, uint32 imageFormat, const uint8* imageData )
{
  CHECK_OPENGL

  uint32 textureId = 0;

#ifdef _WINDOWS_
  glEnable(GL_TEXTURE_2D);							
#else
  glActiveTexture(GL_TEXTURE0);
#endif

  glGenTextures(1, &textureId);

  glBindTexture(GL_TEXTURE_2D, textureId);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,  GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  //LogInfoFMT( "TextureResource", "Creating texture[%d] from resource[%d] size[%d x %d] format[%d]", textureId, resourceId, width, height, imageFormat );

  //if ( imageFormat == 0x83F3 ) // GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
  //{
  //  glCompressedTexImage2D(GL_TEXTURE_2D,0,imageFormat,width,height,0,GL_UNSIGNED_BYTE,imageData);
  //}
  //else
  {
    glTexImage2D( GL_TEXTURE_2D, 0, imageFormat, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, imageData);
  }

  int glError = glGetError();
  if (glError != GL_NO_ERROR) 
  {
    ImageLoaderResource* resource = (ImageLoaderResource*) CoreResourceManager::GetInstance()->GetResource( resourceId );
    if ( resource != NULL )
    {
      LogErrorFMT( "RenderObject", "GLERROR:: CreateTexture: ResourceId[%u/%s] TextureId[%u] format[%d]", resourceId, resource->GetFilename(), textureId, imageFormat );
    }
    else
    {
      LogErrorFMT( "RenderObject", "GLERROR:: CreateTexture: ResourceId[%u] TextureId[%u] format[%d]", resourceId, textureId, imageFormat );
    }
    return 0;
  }
  return textureId;
}
