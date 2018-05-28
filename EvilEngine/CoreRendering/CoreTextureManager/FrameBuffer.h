#ifndef EVILENGINE_CORERENDERING_CORETEXTUREMANAGER_FRAMEBUFFER
#define EVILENGINE_CORERENDERING_CORETEXTUREMANAGER_FRAMEBUFFER 1

class FrameBuffer
{
public:

  FrameBuffer() :
    m_FrameBufferId( 0 ),
    m_TextureId( 0 ),
    m_DepthRenderBufferId( 0 ),
    m_Width( 0 ),
    m_Height( 0 )
  {
  }

  FrameBuffer( uint32 frameBufferId, uint32 textureId, uint32 depthRenderBufferId, uint32 width, uint32 height ) :
    m_FrameBufferId( frameBufferId ),
    m_TextureId( textureId ),
    m_DepthRenderBufferId( depthRenderBufferId ),
    m_Width( width ),
    m_Height( height )
  {
  }

  uint32 m_FrameBufferId;
  uint32 m_TextureId;
  uint32 m_DepthRenderBufferId;
  uint32 m_Width;
  uint32 m_Height;
};

#endif /// EVILENGINE_CORERENDERING_CORETEXTUREMANAGER_FRAMEBUFFER
