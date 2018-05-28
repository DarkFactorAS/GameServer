/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : DDSTextureResource
*
* Description : Loads the DDS Texture format from file and unpacks it to a raw image data blob
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "PreCompile.h"
#include "DDSTextureResource.h"
#include "EvilEngine/CoreEngine/CoreResourceManager/CoreResourceManager.h"
#include "EvilEngine/CoreLib/Utils/CoreUtils.h"

struct RGBA_t
{
  union
  {
    uint32 rgba;
    //    struct { uint8 b,g,r,a; } part;
    struct { uint8 r,g,b,a; } part;
  };
};

struct DXT5Block_t
{
  unsigned char   anAlpha[2];
  unsigned char   anAlphaBits[6];
  unsigned short  anCol[2];
  unsigned char   anLine[4];
};

void DecompressDXT5( size_t nWidth, size_t nHeight, void *pDst, const void *pSrc)
{
  size_t nBlockWidth = (nWidth+3)>>2;
  size_t nBlockHeight = (nHeight+3)>>2;

  for( size_t iy=0; iy<nBlockHeight; iy++ )
  {
    unsigned char nClip = (iy<<2>nHeight-4)?2:0;
    for( size_t ix=0; ix<nBlockWidth; ix++ )
    {
      nClip |= ix<<2>nWidth-4;
      DXT5Block_t* pBlock = ( ((DXT5Block_t*)pSrc)+ iy*nBlockWidth+ ix );

      RGBA_t aColorPalette[4];
      aColorPalette[0].part.b = uint8( ((  pBlock->anCol[0]      & 0x1f) << 3) | (  (pBlock->anCol[0]      & 0x1c) >> 2) );
      aColorPalette[0].part.g = uint8( (( (pBlock->anCol[0]>>5 ) & 0x3f) << 2) | (( (pBlock->anCol[0]>>5 ) & 0x30) >> 4) );
      aColorPalette[0].part.r = uint8( (( (pBlock->anCol[0]>>11) & 0x1f) << 3) | (( (pBlock->anCol[0]>>11) & 0x1c) >> 2) );

      aColorPalette[1].part.b = uint8( ((  pBlock->anCol[1]      & 0x1f) << 3) | (  (pBlock->anCol[1]      & 0x1c) >> 2) );
      aColorPalette[1].part.g = uint8( (( (pBlock->anCol[1]>>5 ) & 0x3f) << 2) | (( (pBlock->anCol[1]>>5 ) & 0x30) >> 4) );
      aColorPalette[1].part.r = uint8( (( (pBlock->anCol[1]>>11) & 0x1f) << 3) | (( (pBlock->anCol[1]>>11) & 0x1c) >> 2) );

      aColorPalette[2].part.r = (aColorPalette[0].part.r*2 + aColorPalette[1].part.r) / 3;
      aColorPalette[2].part.g = (aColorPalette[0].part.g*2 + aColorPalette[1].part.g) / 3;
      aColorPalette[2].part.b = (aColorPalette[0].part.b*2 + aColorPalette[1].part.b) / 3;

      aColorPalette[3].part.r = (aColorPalette[1].part.r*2 + aColorPalette[0].part.r) / 3;
      aColorPalette[3].part.g = (aColorPalette[1].part.g*2 + aColorPalette[0].part.g) / 3;
      aColorPalette[3].part.b = (aColorPalette[1].part.b*2 + aColorPalette[0].part.b) / 3;

      uint8 aAlphaPalette[8];
      aAlphaPalette[0] = pBlock->anAlpha[0];
      aAlphaPalette[1] = pBlock->anAlpha[1];
      if( aAlphaPalette[0] > aAlphaPalette[1] )
      {
        aAlphaPalette[2] = (aAlphaPalette[0]*6 + aAlphaPalette[1]  )/7;
        aAlphaPalette[3] = (aAlphaPalette[0]*5 + aAlphaPalette[1]*2)/7;
        aAlphaPalette[4] = (aAlphaPalette[0]*4 + aAlphaPalette[1]*3)/7;
        aAlphaPalette[5] = (aAlphaPalette[0]*3 + aAlphaPalette[1]*4)/7;
        aAlphaPalette[6] = (aAlphaPalette[0]*2 + aAlphaPalette[1]*5)/7;
        aAlphaPalette[7] = (aAlphaPalette[0]   + aAlphaPalette[1]*6)/7;
      }
      else
      {
        aAlphaPalette[2] = (aAlphaPalette[0]*4 + aAlphaPalette[1]  )/5;
        aAlphaPalette[3] = (aAlphaPalette[0]*3 + aAlphaPalette[1]*2)/5;
        aAlphaPalette[4] = (aAlphaPalette[0]*2 + aAlphaPalette[1]*3)/5;
        aAlphaPalette[5] = (aAlphaPalette[0]*1 + aAlphaPalette[1]*4)/5;
        aAlphaPalette[6] = 0;
        aAlphaPalette[7] = 255;
      }

      unsigned short nAlphaLines[4] = { 
        static_cast<unsigned short>((pBlock->anAlphaBits[1]<<8) |  pBlock->anAlphaBits[0])     ,
        static_cast<unsigned short>((pBlock->anAlphaBits[2]<<4) | (pBlock->anAlphaBits[1]>>4)) ,
        static_cast<unsigned short>((pBlock->anAlphaBits[4]<<8) |  pBlock->anAlphaBits[3])     ,
        static_cast<unsigned short>((pBlock->anAlphaBits[5]<<4) | (pBlock->anAlphaBits[4]>>4))
      }; 

      uint32 *pDstBlock = (uint32*)pDst+(iy*nWidth+ix)*4;

      if( nClip )
      {
        size_t iyye = nHeight-(iy<<2);
        if( iyye > 4 ) iyye = 4;
        for( size_t iyy=0; iyy<iyye; iyy++ )
        {
          uint8 nColorLine = pBlock->anLine[iyy];
          uint16 nAlphaLine = nAlphaLines[iyy];

          uint32 *pDstBlockLine = pDstBlock+iyy*nWidth;
          if( (ix<<2)+0<nWidth )
            pDstBlockLine[0] = aColorPalette[nColorLine&3].rgba | (aAlphaPalette[nAlphaLine&7] << 24);
          if( (ix<<2)+1<nWidth )
            pDstBlockLine[1] = aColorPalette[(nColorLine>>2)&3].rgba | (aAlphaPalette[(nAlphaLine>>3)&7] << 24);
          if( (ix<<2)+2<nWidth )
            pDstBlockLine[2] = aColorPalette[(nColorLine>>4)&3].rgba | (aAlphaPalette[(nAlphaLine>>6)&7] << 24);
          if( (ix<<2)+3<nWidth )
            pDstBlockLine[3] = aColorPalette[nColorLine>>6].rgba | (aAlphaPalette[(nAlphaLine>>9)&7] << 24);
        }
      }
      else
      {
        for( size_t iyy=0; iyy<4; iyy++ )
        {
          uint8 nColorLine = pBlock->anLine[iyy];
          uint16 nAlphaLine = nAlphaLines[iyy];

          uint32 *pDstBlockLine = pDstBlock+iyy*nWidth;
          pDstBlockLine[0] = (aColorPalette[nColorLine&3].rgba      & 0xffffff) | (aAlphaPalette[nAlphaLine&7]      << 24);
          pDstBlockLine[1] = (aColorPalette[(nColorLine>>2)&3].rgba & 0xffffff) | (aAlphaPalette[(nAlphaLine>>3)&7] << 24);
          pDstBlockLine[2] = (aColorPalette[(nColorLine>>4)&3].rgba & 0xffffff) | (aAlphaPalette[(nAlphaLine>>6)&7] << 24);
          pDstBlockLine[3] = (aColorPalette[nColorLine>>6].rgba     & 0xffffff) | (aAlphaPalette[(nAlphaLine>>9)&7] << 24);
        }
      }
    }
  }
}

void GetMipOffsets( size_t width, size_t height, size_t numMips, size_t blocksize, size_t blockWidth, size_t blockHeight, size_t& totalOffsets )
{
  size_t offset = 0;
  for ( size_t curMip = 0; curMip < numMips; ++curMip )
  {
    size_t curWidth = std::max< size_t >( width >> curMip, 1 );
    size_t curHeight = std::max< size_t >( height >> curMip, 1 );

    size_t numBlocksU = ( curWidth + (blockWidth-1) ) / blockWidth;
    size_t numBlocksV = ( curHeight + (blockHeight-1) ) / blockHeight;
    size_t mipSize = numBlocksU * numBlocksV * blocksize;

    //offsets.offsets[ curMip ] = static_cast< uint32 >( offset );
    //offsets.sizes[ curMip ] = static_cast< uint32 >( mipSize );
    offset += mipSize;
  }

  totalOffsets = offset;
}

DDSTextureResource::DDSTextureResource( uint32 resourceInstance, const char * filename, bool isFlipped ) :
  TextureResource( ResourceType_Texture, resourceInstance, filename, isFlipped )
{
  m_NumMinMaps  = 0;
  m_TextureId   = 0;
}

DDSTextureResource::DDSTextureResource( int resourceType, uint32 resourceInstance, const char * filename, bool isFlipped ) :
  TextureResource( resourceType, resourceInstance, filename, isFlipped )
{
  m_NumMinMaps  = 0;
  m_TextureId   = 0;
}

bool DDSTextureResource::LoadData()
{
  m_RawMemory = LoadFileToMemory( m_Filename );
  if ( m_RawMemory != NULL )
  {
    bool loadedData = ParseData( m_RawMemory );
    if ( !loadedData )
    {
      loadedData = TextureResource::LoadData();
    }

    dfAssertFMT( loadedData, "Failed to parse DDSTexture '%s'", m_Filename );

    delete m_RawMemory;
    m_RawMemory = NULL;
    return loadedData;
  }
  return false;
}

bool DDSTextureResource::ParseData( ResourceRef* rawData )
{
  // Check header for file
  if ( strncmp( (const char*)(rawData->m_Data), "DDS ", 4) != 0) 
  {
    return false;
  }
  m_RawReadPointer = 4;

  // get the surface desc
  DDSURFACEDESC2 ddsd;
  memcpy(&ddsd, rawData->m_Data + m_RawReadPointer, sizeof(ddsd) );
  m_RawReadPointer += sizeof(ddsd);
  if((ddsd.dwFlags & DDSD_PIXELFORMAT) != DDSD_PIXELFORMAT)
  {
    return false;
  }
  if(((ddsd.dwFlags & DDSD_WIDTH) != DDSD_WIDTH) || ((ddsd.dwFlags & DDSD_HEIGHT) != DDSD_HEIGHT))
  {
    return false;
  }
  if(((ddsd.ddpfPixelFormat.dwFlags & DDPF_FOURCC) != DDPF_FOURCC) && ((ddsd.ddpfPixelFormat.dwFlags & DDPF_RGB) != DDPF_RGB))
  {
    return false;
  }
  if(((ddsd.ddpfPixelFormat.dwFlags & DDPF_FOURCC) == DDPF_FOURCC) && (ddsd.dwMipMapCount<=1) && ((ddsd.ddpfPixelFormat.dwFourCC==FOURCC_DXT1)||(ddsd.ddpfPixelFormat.dwFourCC==FOURCC_DXT3)||(ddsd.ddpfPixelFormat.dwFourCC==FOURCC_DXT5)) )
  {
    return false;
  }

  m_Width  = ddsd.dwWidth;
  m_Height = ddsd.dwHeight;

  long fileSize = 0;
  bool validFormat = false;

  if((ddsd.ddpfPixelFormat.dwFlags & DDPF_FOURCC) == DDPF_FOURCC) // if FOURCC code is valid
  {
    switch(ddsd.ddpfPixelFormat.dwFourCC)
    {
    case FOURCC_DXT1:
      m_Format    = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
      m_PixelSize = (ddsd.dwAlphaBitDepth!=0) ? 4 : 3;
      validFormat = true;
      fileSize    = rawData->m_Filesize;
      break;
    case FOURCC_DXT3:
      m_Format    = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
      m_PixelSize = 4;
      validFormat = true;
      fileSize    = rawData->m_Filesize;
      break;
    case FOURCC_DXT5:
      {
        size_t totalMips = 1;
        size_t totalSize = 0;
        GetMipOffsets( m_Width, m_Height, totalMips, 4, 1, 1, totalSize );

        void* pDst       = malloc( totalSize );
        DecompressDXT5( m_Width, m_Height, pDst, rawData->m_Data + m_RawReadPointer );

        m_Data      = (uint8*)pDst;
        m_Format    = GL_RGBA;//GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        m_PixelSize = 4;
        validFormat = true;
        fileSize    = rawData->m_Filesize;

        if ( m_IsFlipped )
        {
          FlipImage( m_Data, m_Width, m_Height, 4 );
        }

        return true;
      }
    default:
      break;
    }
  }

  // If we didn't find a FOURCC DXT format, try looking for uncompressed RGB[A] data.
  if ( !validFormat && ddsd.ddpfPixelFormat.dwFlags & 0x40)
  {
    unsigned int rmask = ddsd.ddpfPixelFormat.dwRBitMask;
    unsigned int gmask = ddsd.ddpfPixelFormat.dwGBitMask;
    unsigned int bmask = ddsd.ddpfPixelFormat.dwBBitMask;
    //unsigned int amask = ddsd.ddpfPixelFormat.dwRGBAlphaBitMask;
    int ridx = getMaskByteIndex(rmask);
    int gidx = getMaskByteIndex(gmask);
    int bidx = getMaskByteIndex(bmask);
    //int aidx = getMaskByteIndex(amask);

    switch (ddsd.ddpfPixelFormat.dwRGBBitCount)
    {
      case 24:
        {
          m_Format    = GL_RGB;
          m_PixelSize = 3;
          fileSize    = ( m_Width * m_Height * m_PixelSize) + m_RawReadPointer;
          BGR2RGB( rawData->m_Data + m_RawReadPointer, m_Width * m_Height, 3 );
        }
        break;

      case 32:
        {
          m_Format    = GL_RGBA;
          m_PixelSize = 4;
          fileSize    = ( m_Width * m_Height * m_PixelSize) + m_RawReadPointer;

          if (ridx == 2 && gidx == 1 && bidx == 0)
          {
            BGR2RGB( rawData->m_Data + m_RawReadPointer, m_Width * m_Height, 4 );
          }
        }
        break;

      default:
        return false;
    }
  }
  m_NumMinMaps  = ddsd.dwMipMapCount;
  m_Data        = (uint8_t*) malloc( fileSize - m_RawReadPointer );
  memcpy( m_Data, rawData->m_Data + m_RawReadPointer, fileSize - m_RawReadPointer ); 

  return true;
}

int DDSTextureResource::getMaskByteIndex(unsigned int mask)
{
  switch (mask)
  {
  case 0xff000000:
    return 3;
  case 0x00ff0000:
    return 2;
  case 0x0000ff00:
    return 1;
  case 0x000000ff:
    return 0;
  default:
    return -1; // no or invalid mask
  }
}

void DDSTextureResource::BGR2RGB( uint8_t* data, int numPixels, int pixelSize )
{
  unsigned char c1;
  unsigned char c2;

  // Switch the R and B component
  for( long n=0; n< numPixels * pixelSize; n += pixelSize )
  {
    c1 =*( data + n + 0 );
    c2 =*( data + n + 2 );

    *( data + n + 0 ) = c2;
    *( data + n + 2 ) = c1;
  }
}

uint32 DDSTextureResource::CreateTexture()
{
  if ( m_TextureId != 0 )
  {
    return m_TextureId;
  }

  switch( m_Format )
  {
    // Not implemented yet
    case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
      return 0;
    case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
      break;
    default:
      return TextureResource::CreateTexture();
  }
  
  CHECK_OPENGL
  
#ifdef _WINDOWS_
  glEnable(GL_TEXTURE_2D);
#else
  glActiveTexture(GL_TEXTURE0);
#endif

  glGenTextures(1, &m_TextureId);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glBindTexture(GL_TEXTURE_2D, m_TextureId );

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,  GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  uint32 width  = m_Width;
  uint32 height = m_Height;
  
  switch( m_Format )
  {
    case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
      {
        int blockSize = 16;
        long offset=0;

        for (int l = 0; l < m_NumMinMaps && (width || height); ++l)
        {
          if ( width == 0)
            width= 1;
          if ( height == 0)
            height = 1;

          CHECK_OPENGL

          int size = ((width+3)/4)*((height+3)/4)*blockSize;
          glCompressedTexImage2D(GL_TEXTURE_2D, l, m_Format, width, height, 0, size, m_Data + offset );

          offset += size;
          width >>= 1;
          height >>= 1;
        }
        break;
      }
    default:
      return 0;
  }

  int glError = glGetError();
  if (glError != GL_NO_ERROR) 
  {
    ImageLoaderResource* resource = (ImageLoaderResource*) CoreResourceManager::GetInstance()->GetResource( m_ResourceId );
    if ( resource != NULL )
    {
      LogErrorFMT( "DDSTextureResource", "GLERROR[%d]:: CreateTexture: ResourceId[%u/%s] TextureId[%u] format[%d] size[%d x %d]", glError, m_ResourceId, resource->GetFilename(), m_TextureId, m_Format, m_Width, m_Height );
    }
    else
    {
      LogErrorFMT( "DDSTextureResource", "GLERROR[%d]:: CreateTexture: ResourceId[%u] TextureId[%u] format[%d] size[%d x %d]", glError, m_ResourceId, m_TextureId, m_Format, m_Width, m_Height );
    }
    return 0;
  }

  return m_TextureId;
}

