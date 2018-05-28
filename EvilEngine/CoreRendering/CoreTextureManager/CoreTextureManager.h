#ifndef EVILENGINE_CORERENDERING_CORETEXTUREMANAGER_CORETEXTUREMANAGER
#define EVILENGINE_CORERENDERING_CORETEXTUREMANAGER_CORETEXTUREMANAGER 1

class RenderObject;
class ShaderResource;

#include "EvilEngine/CoreLib/Signal/SignalTarget.h"
#include "Texture.h"
#include "FrameBuffer.h"
#include <map>

class CoreTextureManager : public SignalTarget
{
public:

  CoreTextureManager();
  ~CoreTextureManager();

  static CoreTextureManager*  GetInstance();

  const Texture*              GetTexture( uint32 resourceId );
  uint32                      GetTextureId( uint32 resourceId );
  void                        ReleaseTexture( uint32 textureId );
  void                        ReleaseTextures();
  void                        ReloadTextures();

  static void                 DestroyFrameBufferTexture( FrameBuffer* frameBuffer );
  static FrameBuffer*         CreateFrameBufferTexture( uint32 width, uint32 height );

private:

  std::map< uint32, Texture > m_Texturemap;
};

#endif /// EVILENGINE_CORERENDERING_CORETEXTUREMANAGER_CORETEXTUREMANAGER
