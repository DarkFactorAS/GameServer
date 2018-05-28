#ifndef EVILENGINE_CORERENDERING_CORETEXTUREMANAGER_TEXTURE
#define EVILENGINE_CORERENDERING_CORETEXTUREMANAGER_TEXTURE 1

class Texture
{
public:

  Texture() :
    m_ResourceId( 0 ),
    m_TextureId( 0 ),
    m_References( 0 )
  {
  }

  Texture( uint32 resourceId, uint32 textureId ) :
    m_ResourceId( resourceId ),
    m_TextureId( textureId ),
    m_References( 1 )
  {
  }

  uint32 m_ResourceId;
  uint32 m_TextureId;
  uint32 m_References;
};

#endif /// EVILENGINE_CORERENDERING_CORETEXTUREMANAGER_TEXTURE
