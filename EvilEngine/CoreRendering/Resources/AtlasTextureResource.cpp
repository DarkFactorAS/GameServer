/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : AtlasTextureResource
*
* Description : Loads an image and automaticly creates smaller textures from it. Also known as a 
*               an atlas or gui atlas.
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "PreCompile.h"
#include "AtlasTextureResource.h"
#include "EvilEngine/CoreEngine/CoreResourceManager/CoreResourceManager.h"

AtlasTextureResource::AtlasTextureResource( uint32 resourceInstance, const char * filename ) :
  DDSTextureResource( ResourceType_AtlasTexture, resourceInstance, filename, false )
{
}

AtlasTextureResource::AtlasTextureResource( uint32 resourceInstance, const char * filename, int gridSizeX, int gridSizeY ) :
  DDSTextureResource( ResourceType_AtlasTexture, resourceInstance, filename, false )
{
  CreateBitmapGrid( gridSizeX, gridSizeY );
}

AtlasTextureResource::~AtlasTextureResource()
{
  CoreResourceManager* resourceManager = CoreResourceManager::GetInstanceIfAny();
  if ( resourceManager == NULL )
  {
    return;
  }

  for ( size_t index = 0; index < m_AtlasEntries.size(); index++ )
  {
    const ATLAS_ENTRY& entry = m_AtlasEntries[ index ];
    if ( entry.m_OwnsId )
    {
      resourceManager->RemoveResource( entry.m_ResourceId );
    }
  }
}


void AtlasTextureResource::CreateBitmapFromArea( uint32 resourceId, const RectInt& area )
{
  m_AtlasEntries.push_back( ATLAS_ENTRY( resourceId, area.m_X, area.m_Y, area.m_Width, area.m_Height, false ) );
}

void AtlasTextureResource::CreateBitmapInGrid(  uint32 resourceId, int x, int y, int maxX, int maxY )
{
  m_AtlasEntries.push_back( ATLAS_ENTRY( resourceId, x, y, maxX, maxY, true ) );
}

void AtlasTextureResource::CreateBitmapGrid( int numX, int numY )
{
  for ( int y = 0; y < numY; y++ )
  {
    for ( int x = 0; x < numX; x++ )
    {
      uint32 resourceId = m_ResourceId + ( y * numX ) + x + 1;
      m_AtlasEntries.push_back( ATLAS_ENTRY( resourceId, x, y, numX, numY, true ) );
    }
  }
}

bool AtlasTextureResource::LoadData()
{
  bool loadedImage = DDSTextureResource::LoadData();
  if ( loadedImage && !m_AtlasEntries.empty() )
  {
    CoreResourceManager* resourceManager = CoreResourceManager::GetInstance();

    Bitmap* srcBitmap = new Bitmap( m_Width, m_Height, m_Format, m_Data );
    m_Data = NULL;

    for ( size_t index = 0; index < m_AtlasEntries.size(); index++ )
    {
      ATLAS_ENTRY& entry = m_AtlasEntries[ index ];
      entry.m_OwnsId = true;

      if ( entry.m_Scaled )
      {
        int tileHeight  = m_Height / entry.m_MaxY;
        int tileWidth   = m_Width  / entry.m_MaxX;

        Bitmap* bitmap = new Bitmap( tileWidth, tileHeight, m_Format );
        bitmap->CopyArea( PointI( 0, 0 ), *srcBitmap, RectInt( entry.m_X * tileWidth, entry.m_Y * tileHeight, tileWidth, tileHeight ), true );
        TextureResource* resource = new TextureResource( ResourceType_Texture, entry.m_ResourceId, 
          String::FormatString("%s:ScaledAtlas:%d", m_Filename, index ).c_str(), 
          bitmap->m_Width, bitmap->m_Height, bitmap->m_PixelFormat, bitmap->m_Data, false );
        resourceManager->AddResource( resource, Resource::RESOURCEFLAG_PARENTCACHE );
      }
      else
      {
        Bitmap* bitmap = new Bitmap( entry.m_MaxX, entry.m_MaxY, m_Format );
        bitmap->CopyArea( PointI( 0, 0 ), *srcBitmap, RectInt( entry.m_X, entry.m_Y, entry.m_MaxX, entry.m_MaxY ), true );
        TextureResource* resource = new TextureResource( ResourceType_Texture, entry.m_ResourceId, 
          String::FormatString("%s:Atlas:%d", m_Filename, index ).c_str(), 
          bitmap->m_Width, bitmap->m_Height, bitmap->m_PixelFormat, bitmap->m_Data, false );
        resourceManager->AddResource( resource, Resource::RESOURCEFLAG_PARENTCACHE );
      }
    }
    delete srcBitmap;
    //m_AtlasEntries.clear();
  }
  return loadedImage;
}
