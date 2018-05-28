
#ifndef EVILENGINE_CORERENDERING_RESOURCE_ATLASTEXTURERESOURCE
#define EVILENGINE_CORERENDERING_RESOURCE_ATLASTEXTURERESOURCE 1

#include "../Resources/DDSTextureResource.h"
#include <vector>
#include "EvilEngine/CoreLib/CoreMath/Rect.h"
#include "../Bitmap.h"

class AtlasTextureResource : public DDSTextureResource
{
public:

  struct ATLAS_ENTRY
  {
    bool    m_Scaled;
    uint32  m_ResourceId;
    int     m_X;
    int     m_Y;
    int     m_MaxX;
    int     m_MaxY;
    bool    m_OwnsId;

    ATLAS_ENTRY( uint32 resourceId, int x, int y, int maxX, int maxY, bool scaled )
    {
      m_ResourceId  = resourceId;
      m_X           = x;
      m_Y           = y;
      m_MaxX        = maxX;
      m_MaxY        = maxY;
      m_Scaled      = scaled;
      m_OwnsId      = false;
    }
  };

  AtlasTextureResource( uint32 resourceInstance, const char * filename );
  AtlasTextureResource( uint32 resourceInstance, const char * filename , int gridSizeX, int gridSizeY );
  virtual ~AtlasTextureResource();

  void                  CreateBitmapFromArea( uint32 resourceId, const RectInt& area );
  void                  CreateBitmapInGrid(  uint32 resourceId, int numX, int numY, int maxX, int maxY );

  void                  CreateBitmapGrid( int numX, int numY );
  virtual bool          LoadData() DF_OVERRIDE;
  virtual uint32        CreateTexture() DF_OVERRIDE { return 0; }

private:

  //std::vector< TextureResource* > m_Resources;

  uint32                m_GridX;
  uint32                m_GridY;

  std::vector<ATLAS_ENTRY>  m_AtlasEntries;
};

#endif /// EVILENGINE_CORERENDERING_RESOURCE_TEXTURERESOURCE
