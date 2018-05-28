
#ifndef EVILENGINE_CORERENDERING_RESOURCE_TEXTURERESOURCE
#define EVILENGINE_CORERENDERING_RESOURCE_TEXTURERESOURCE 1

#include "../Resources/ImageResource.h"

class TextureResource : public ImageLoaderResource
{
public:
  TextureResource( uint32 resourceInstance, const char * filename, bool isFlipped );
  TextureResource( int resourceType, uint32 resourceInstance, const char * filename, bool isFlipped );
  TextureResource( int resourceType, uint32 resourceInstance, const char * filename, uint32 width, uint32 height, uint32 format, uint8_t* data, bool isFlipped );

  virtual uint32  CreateTexture();
  static uint32   CreateTexture( uint32 resourceId, uint32 width, uint32 height, uint32 imageFormat, const uint8* imageData );
};

#endif /// EVILENGINE_CORERENDERING_RESOURCE_TEXTURERESOURCE
