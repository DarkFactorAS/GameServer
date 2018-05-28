
#include "PreCompile.h"
#include "CoreTextureManager.h"

#include "EvilEngine/CoreRendering/Resources/TextureResource.h"
#include "EvilEngine/CoreRendering/OpenGLRender/CoreOpenGLRender.h"
#include "EvilEngine/CoreEngine/CoreResourceManager/CoreResourceManager.h"

CoreTextureManager::CoreTextureManager()
{
  LogInfo("CoreTextureManager", "Create");
  CoreOpenGLRender::s_SignalScreenResetStart.Connect( this, &CoreTextureManager::ReleaseTextures );
  CoreOpenGLRender::s_SignalScreenResetDone.Connect( this, &CoreTextureManager::ReloadTextures );
}

CoreTextureManager::~CoreTextureManager()
{
  //LogInfo("CoreTextureManager", "Destroy");
  //CoreOpenGLRender::s_SignalScreenReset.Disconnect( this, &CoreTextureManager::ReloadTextures );
}

/************************************************************************************************
 * GetInstance:
 * Returns the singleton instance of the shader manager
 *
 * @return (CoreShaderManager) - Shader manager singleton pointer
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
CoreTextureManager* CoreTextureManager::GetInstance()
{
  static CoreTextureManager*  s_Instance = NULL;

  if ( s_Instance == NULL )
  {
    s_Instance = new CoreTextureManager();
  }
  return s_Instance;
}


const Texture* CoreTextureManager::GetTexture( uint32 resourceId )
{
  // 1. Check if we have it in the cache
  std::map< uint32, Texture >::iterator itEntry = m_Texturemap.find( resourceId );
  if ( itEntry != m_Texturemap.end() )
  {
    Texture& texture = itEntry->second;
    texture.m_References++;
    return &texture;
  }

  // 2. Generate it from a resource if it exist
  TextureResource* resource = (TextureResource*) CoreResourceManager::GetInstance()->GetResource( Resource::ResourceType_Texture, resourceId );
  if ( resource == NULL || !resource->HasData() )
  {
    return NULL;
  }

  // 3. Add it to the cache and return it
  uint32 textureId = resource->CreateTexture();
  m_Texturemap[ resourceId ] = Texture( resourceId, textureId );

  return &(m_Texturemap[ resourceId ]);
}

uint32 CoreTextureManager::GetTextureId( uint32 resourceId )
{
  // 1. Check if we have it in the cache
  std::map< uint32, Texture >::iterator itEntry = m_Texturemap.find( resourceId );
  if ( itEntry != m_Texturemap.end() )
  {
    Texture& texture = itEntry->second;
    texture.m_References++;
    return texture.m_TextureId;
  }

  // 2. Generate it from a resource if it exist
  TextureResource* resource = (TextureResource*) CoreResourceManager::GetInstance()->GetResource( Resource::ResourceType_Texture, resourceId );
  if ( resource == NULL || !resource->HasData() )
  {
    return 0;
  }

  // 3. Add it to the cache and return it
  uint32 textureId = resource->CreateTexture();
  if ( textureId != 0 )
  {
    m_Texturemap[ resourceId ] = Texture( resourceId, textureId );
    return textureId;
  }

  return 0;
}

/************************************************************************************************
* ReleaseTexture:
* Releases a texture from the render engine
*
* @param  (uint32)      textureId   - ID of the texture to release
*
* @author Thor Richard Hansen
*************************************************************************************************/
void CoreTextureManager::ReleaseTexture( uint32 textureId )
{
  if ( textureId == 0 )
  {
    return;
  }

  // 1. Remove this texture from the cache
  for ( std::map< uint32, Texture >::iterator itEntry = m_Texturemap.begin(); itEntry != m_Texturemap.end(); ++itEntry )
  {
    Texture& texture = itEntry->second;
    if ( texture.m_TextureId == textureId )
    {
      dfAssert( texture.m_References > 0, "Broken texture reference" );

      texture.m_References--;
      if ( texture.m_References == 0 )
      {
        m_Texturemap.erase( itEntry );
        glDeleteTextures(1, &textureId);
      }
      return;
    }
  }

  dfAssert( false, "Unknown texture id!" );
}

void CoreTextureManager::ReleaseTextures()
{
  // Go through all textures in the cache and reload it
  for ( std::map< uint32, Texture >::iterator itEntry = m_Texturemap.begin(); itEntry != m_Texturemap.end(); ++itEntry )
  {
    Texture& texture    = itEntry->second;
    if ( texture.m_TextureId != 0 )
    {
      glDeleteTextures(1, &texture.m_TextureId);
      texture.m_TextureId = 0;
    }
  }
}

void CoreTextureManager::ReloadTextures()
{
  CoreResourceManager* resourceManager = CoreResourceManager::GetInstance();

  LogInfoFMT("CoreTextureManager", "ReloadTextures. Reloading %d textures ...", m_Texturemap.size() );

  // Go through all textures in the cache and reload it
  for ( std::map< uint32, Texture >::iterator itEntry = m_Texturemap.begin(); itEntry != m_Texturemap.end(); ++itEntry )
  {
    Texture& texture    = itEntry->second;

    if ( texture.m_TextureId != 0 )
    {
      LogWarningFMT("CoreTextureManager", "Could not release texture [Id:%d - ResourceId:%d] -> Call ReleaseTextureFirst!", texture.m_TextureId, texture.m_ResourceId );
      texture.m_TextureId = 0;
    }

    // 2. Generate it from a resource if it exist
    TextureResource* resource = (TextureResource*) resourceManager->GetResource( Resource::ResourceType_Texture, texture.m_ResourceId );
    if ( resource == NULL || !resource->HasData() )
    {
      LogWarningFMT("CoreTextureManager", "Could not fetch resource[%d] -> Texture will be missing.", texture.m_ResourceId );
      continue;
    }

    // Recreate the texture
    texture.m_TextureId = resource->CreateTexture();
    if ( texture.m_TextureId == 0 )
    {
      LogWarningFMT("CoreTextureManager", "Could not recreate texture [ResourceId:%d] -> Texture will be missing.", texture.m_ResourceId );
      continue;
    }

    LogInfoFMT("CoreTextureManager", "Recreated texture[%d] with resourceId[%d]", texture.m_TextureId, texture.m_ResourceId );
  }
}

void CoreTextureManager::DestroyFrameBufferTexture( FrameBuffer* frameBuffer )
{
  if ( frameBuffer != NULL )
  {
    if ( frameBuffer->m_TextureId != 0 )
    {
      glDeleteTextures(1, &frameBuffer->m_TextureId);
      frameBuffer->m_TextureId = 0;
    }
    if ( frameBuffer->m_FrameBufferId )
    {
      glDeleteFramebuffers( 1, &frameBuffer->m_FrameBufferId );
      frameBuffer->m_FrameBufferId = 0;
    }

    LogInfoFMT("CoreTextureManager", "Release frame buffer texture [ %d x %d ]", frameBuffer->m_Width, frameBuffer->m_Height );
    delete frameBuffer;
  }
}

FrameBuffer* CoreTextureManager::CreateFrameBufferTexture( uint32 width, uint32 height )
{
  CHECK_OPENGL

#ifdef PLATFORM_WINDOWS
  glEnable(GL_TEXTURE_2D);
#else
  glActiveTexture(GL_TEXTURE0);
#endif
    
  CHECK_OPENGL

  LogInfoFMT("CoreTextureManager", "Create frame buffer texture [ %d x %d ]", width, height );


  // Generate texture
  GLuint renderedTexture;
  glGenTextures(1, &renderedTexture);
  glBindTexture(GL_TEXTURE_2D, renderedTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    float* buffer = (float*) malloc( width * height * sizeof( char ) * 2 );
  glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, width, height, 0,GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, buffer);
  glBindTexture(GL_TEXTURE_2D, 0);
  CHECK_OPENGL

  // Generate render buffer ( for depth )
  GLuint depthrenderbuffer;
  glGenRenderbuffers(1, &depthrenderbuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
  CHECK_OPENGL

  // Generate texture
  GLuint framebuffer;
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
    
  glGenFramebuffers(1, &framebuffer);
  glBindFramebuffer( GL_FRAMEBUFFER, framebuffer );
  CHECK_OPENGL

#ifdef GLEW_STATIC

    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, renderedTexture, 0);
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthrenderbuffer);
    //glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    CHECK_OPENGL

#endif
    
  int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  switch( status )
  {
      case GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE:
          break;
      case GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME:
          break;
      case GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL:
          break;
      case GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE:
          break;
      case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
          break;
      case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
          break;
      case GL_FRAMEBUFFER_UNSUPPORTED:
          break;
#ifdef GLEW_STATIC
      case GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER:
        break;
      case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        break;
      case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        break;
      case GL_FRAMEBUFFER_UNDEFINED:
        LogInfo("CoreTextureManager", "Failed to create framebuffer : Framebuffer undefined");
        break;
#endif
          
      case GL_FRAMEBUFFER_COMPLETE:
        {
          LogInfoFMT("CoreTextureManager", "Create frame buffer texture OK! [ %d x %d ] [FB-id:%d] [RenderTexture:%d] [depthBufferId:%d]", width, height, framebuffer, renderedTexture, depthrenderbuffer );
          return new FrameBuffer( framebuffer, renderedTexture, depthrenderbuffer, width, height );
        }
  }
    
  // TODO: Release the objects
  LogErrorFMT("CoreTextureManager", "Failed to create a framebuffer [ %d x %d ]. Errorcpde = %d", width,height, status );
  return NULL;
}