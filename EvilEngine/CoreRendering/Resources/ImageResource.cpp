/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : ImageLoader
*
* Description : Resource that loads a bmp/png image to memory ( usually used for textures )
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "PreCompile.h"
#include "ImageResource.h"
#include "EvilEngine/CoreLib/Utils/CoreUtils.h"

#include <stdio.h>
#include <string.h>

//#ifdef WINDOWS
//  #define USE_PNG
//#else
  #include "EvilEngine/CoreLib/BasicTypes/Definitions/VolatileArgs.h"
//#endif

#ifdef USE_PNG
#define PNG_DEBUG 3
#include "png.h"

#pragma warning( disable : 4611 )

static void png_read_callback(png_structp png, png_bytep data, png_size_t size) 
{
  ImageLoaderResource& loader = *((ImageLoaderResource*) png_get_io_ptr(png));
  memcpy( data, loader.m_RawMemory->m_Data + loader.m_RawReadPointer, size );
  loader.m_RawReadPointer += size;
}
#endif

ImageLoaderResource::ImageLoaderResource( const char * filename, bool isFlipped ) : 
  Resource( ResourceType_Bmp, filename )
{
  m_Data            = NULL;
  m_RawReadPointer  = 0;
  m_Width           = 0;
  m_Height          = 0;
  m_Format          = 0;
  m_RawMemory       = NULL;
  m_PixelSize       = 0;
  m_IsFlipped       = isFlipped;
}

ImageLoaderResource::ImageLoaderResource( int resourceType, uint32 resourceInstance, const char * filename, bool isFlipped ) : 
  Resource( resourceType, resourceInstance, filename )
{
  m_Data            = NULL;
  m_RawReadPointer  = 0;
  m_Width           = 0;
  m_Height          = 0;
  m_Format          = 0;
  m_RawMemory       = NULL;
  m_PixelSize       = 0;
  m_IsFlipped       = isFlipped;
}

ImageLoaderResource::ImageLoaderResource( const char * filename, ResourceRef* memory, bool isFlipped ) :
  Resource( ResourceType_Bmp, filename )
{
  m_Data            = NULL;
  m_RawReadPointer  = 0;
  m_Width           = 0;
  m_Height          = 0;
  m_Format          = 0;
  m_RawMemory       = memory;
  m_PixelSize       = 0;
  m_IsFlipped       = isFlipped;
}


bool ImageLoaderResource::LoadData()
{
  if ( m_Data != NULL )
  {
    return true;
  }

  if ( m_RawMemory == NULL )
  {
    m_RawMemory = LoadFileToMemory( m_Filename );
  }

  if ( m_RawMemory != NULL )
  {
    bool loadedOk = ParseBitmapFromMemory( m_RawMemory );
    free( m_RawMemory );
    m_RawMemory = NULL;
    return loadedOk;
  }
  return false;
}


bool ImageLoaderResource::ParseBitmapFromMemory( ResourceRef* rawData )
{
  if ( rawData == NULL )
  {
    return false;
  }

  uint8_t* memory = rawData->m_Data;
  if ( memory == NULL )
  {
    return false;
  }

  // Load as BMP
  bool ret = false;
  if ( memory[0]=='B' && memory[1]=='M' )
  {
    ret = ParseBmpFromMemory( rawData );
  }
#ifdef USE_PNG
  else if ( png_sig_cmp(memory, 0, 8) == 0)
  {
    ret = ParsePngFromMemory( rawData );
  }
#endif

  if ( !ret )
  {
    LogErrorFMT("ImageLoader", "Failed to parse file : %d/%s", GetInstance(), m_Filename );

    if ( m_Data != NULL )
    {
      free( m_Data );
      m_Data = NULL;
    }
  }


  m_IsLoaded = true;
  return ret;
}

bool ImageLoaderResource::ParseBmpFromMemory( ResourceRef * rawMemory )
{
  m_RawMemory = rawMemory;

  uint8_t* memory = rawMemory->m_Data;

  // Data read from the header of the BMP file
  //unsigned char header[54]; // Each BMP file begins by a 54-bytes header
  unsigned int dataPos;     // Position in the file where the actual data begins
  unsigned int imageSize;   // = width*height*3

  if ( memory[0]!='B' || memory[1]!='M' )
  {
    printf("Not a correct BMP file\n");
    return false;
  }

  // Read ints from the byte array
  dataPos    = *(int*)&(memory[0x0A]);
  imageSize  = *(int*)&(memory[0x22]);
  m_Width    = *(int*)&(memory[0x12]);
  m_Height   = *(int*)&(memory[0x16]);
  int bpp    = *(short*)&(memory[0x1C]);
  m_PixelSize= 3;
  m_Format   = GL_RGBA;

  if (imageSize==0)    imageSize = m_Width * m_Height * ( bpp / 8 );
  if (dataPos==0)      dataPos=54; 
  m_RawReadPointer += 54;

  if ( bpp == 24 )
  {
    m_Data = new unsigned char [imageSize];
    memcpy((void *)m_Data, (const void *)( memory + m_RawReadPointer), (int)imageSize);
    return Convert24BitBmpToRGBA();
  }
  else if ( bpp == 8 )
  {
    unsigned char* palette = new unsigned char[ 1024 ];
    memcpy(palette, memory + m_RawReadPointer , 1024 ); // 256 colors x RGB-A colors
    m_RawReadPointer += 1024;

    m_Data = new unsigned char [imageSize];
    memcpy((void *)m_Data, (const void *)( memory + m_RawReadPointer), (int)imageSize);

    return Convert8BitBmpToRGBA( palette );
  }
  else if ( bpp == 32 )
  {
    m_Data = new unsigned char [imageSize];
    memcpy((void *)m_Data, (const void *)( memory + m_RawReadPointer), (int)imageSize);
    return Convert32BitBmpToRGBA();
  }
  return false;
}

//Converts 24 bit A-BGR to 24 bit RGB-A
bool ImageLoaderResource::Convert32BitBmpToRGBA()
{
  //If RGBA set alphas to 1.0f / (255)
  uint32 bufferSize = m_Width * m_Height * 4;
  for(uint i=0; i< bufferSize; i+= 4)
  {
    //uint8_t v = m_Data[i + 3];
    m_Data[i + 0] = m_Data[i + 3];

    if ( m_Data[i+0] == 0xFF && m_Data[i+1] == 0xFF && m_Data[i+2] == 0xFF)
    {
      m_Data[i + 3] = 0;
    }
    else
    {
      m_Data[i + 3] = 0xFF;
    }
  }

  return true;
}

//Converts 24 bit BGR to 24 bit RGB-A
bool ImageLoaderResource::Convert24BitBmpToRGBA()
{
  m_Format   = GL_RGBA;

  //Allocate RGB/A buffer for final data
  uint32 bufferSize = m_Width * m_Height * 4;
  unsigned char* data = new unsigned char[ bufferSize ];
  if( data == NULL )
  {
    LogError( "ImageLoader", "Insufficient memory to allocate an image buffer" );
    return false;
  }

  //Calculate the actual padded width of the raw data (data aligned on 4 byte boundaries)
  int padWidth;
  for (padWidth = (int) (m_Width * 24.0f/8.0f) ; padWidth%4 != 0 ; padWidth++);

  //Convert from raw data
  int srcIndex = 0;	//Source data index
  int tgtIndex = 0;	//Target data index
  int tgtSkip  = 1;

  //if(b_negHeight)//Data upside-down (usual format)
  //{
  //  for(int t=0; t<i_height; t++)
  //  {
  //    srcIndex = padWidth * (i_height - 1 - t) + 2;

  //    for(int s=0; s<i_width; s++, tgtIndex+= tgtSkip, srcIndex+= 6)
  //      for(int i=0; i<3; i++, tgtIndex++, srcIndex--)
  //        data[tgtIndex] = rawData[srcIndex];
  //  }
  //}
  //else //Data is not upside-down
  {
    for(uint t=0; t< m_Height; t++)
    {
      srcIndex = padWidth * t + 2;

      for(uint s=0; s< m_Width; s++, tgtIndex+= tgtSkip, srcIndex+= 6)
      {
        for(uint i=0; i<3; i++, tgtIndex++, srcIndex--)
        {
          data[tgtIndex] = m_Data[srcIndex];
        }
      }
    }
  }

  //If RGBA set alphas to 1.0f / (255)
  for(uint i=3; i< bufferSize; i+= 4)
  {
    data[i] = 0xFF;
  }

  free( m_Data );
  m_Data = data;

  return true;
}

bool ImageLoaderResource::Convert8BitBmpToRGBA( unsigned char* palette )
{
  m_Format   = GL_RGBA;

  //Allocate RGB/A buffer for final data
  uint32 bufferSize = m_Width * m_Height * 4;
  unsigned char* data = new unsigned char[ bufferSize ];
  if( data == NULL )
  {
    LogError( "ImageLoader", "Insufficient memory to allocate an image buffer" );
    return false;
  }

  //Calculate the actual padded width of the raw data (data aligned on 4 byte boundaries)
  int padWidth;
  for (padWidth = (int) (m_Width * 8.0f/8.0f) ; padWidth%4 != 0 ; padWidth++);

  int sIndex = 0;
  int tIndex = 0;
  for(uint t=0; t<m_Height; t++)
  {
    sIndex = padWidth * t;

    for(uint s=0; s< m_Width; s++)
    {
      uint8 color = ( m_Data[sIndex] ) * 4;

      data[tIndex++] = palette[ color + 2 ];
      data[tIndex++] = palette[ color + 1 ];
      data[tIndex++] = palette[ color + 0 ];
      data[tIndex++] = 0xFF;//palette[ color + 3 ];
      sIndex ++;
    }
  }

  free( m_Data );
  m_Data = data;

  return true;
}

void ImageLoaderResource::FlipImage( uint8* img, int width, int height, int bpp )
{
  int pixWidth = width * bpp;
  uint8* tmpLine = (uint8*) malloc( pixWidth );
  for ( int y = 0; y < height / 2; y++ )
  {
    memcpy( tmpLine,                                  img + ( ( height - y - 1 ) * pixWidth ), pixWidth );  // tmp -> B
    memcpy( img + ( ( height - y - 1 ) * pixWidth ),  img + ( y * pixWidth ),     pixWidth );  // B -> A
    memcpy( img + ( y * pixWidth ),                   tmpLine,                    pixWidth );  // A -> tmp
  }
  free( tmpLine );
}

bool ImageLoaderResource::ParsePngFromMemory( ResourceRef* rawData ) 
{
#ifdef USE_PNG

  m_RawMemory     = rawData;
  uint8_t* memory = rawData->m_Data;


  //png_byte lHeader[8];
  png_structp lPngPtr = NULL; 
  png_infop lInfoPtr = NULL;
  //png_byte* lImageBuffer = NULL; 
  png_bytep* lRowPtrs = NULL;
  png_size_t lRowSize; 
  bool lTransparency;

  m_RawReadPointer += 8;
  if (png_sig_cmp(memory, 0, 8) != 0)
  {
    return false;
  }

  lPngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!lPngPtr)
  {
    return false;
  }
  lInfoPtr = png_create_info_struct(lPngPtr);
  if (!lInfoPtr)
  {
    png_destroy_read_struct(&lPngPtr, &lInfoPtr, NULL);
    return false;
  }

  png_set_read_fn(lPngPtr, this, png_read_callback);
  if (setjmp(png_jmpbuf(lPngPtr)))
  {
    png_destroy_read_struct(&lPngPtr, &lInfoPtr, NULL);
    return false;
  }

  png_set_sig_bytes(lPngPtr, 8);
  png_read_info(lPngPtr, lInfoPtr);

  png_int_32 lDepth, lColorType;
  png_uint_32 lWidth, lHeight;
  png_get_IHDR(lPngPtr, lInfoPtr, &lWidth, &lHeight, &lDepth, &lColorType, NULL, NULL, NULL);
  m_Width   = lWidth; 
  m_Height  = lHeight;

  dfAssert( m_Width == m_Height && CoreUtils::IsPowerOfTwo( m_Width ), "Image [%s] has to be squared and power of two! size[%d x %d]", m_Filename, m_Width, m_Height );

  // Creates a full alpha channel if transparency is encoded as
  // an array of palette entries or a single transparent color.
  lTransparency = false;
  if (png_get_valid(lPngPtr, lInfoPtr, PNG_INFO_tRNS)) 
  {
    png_set_tRNS_to_alpha(lPngPtr);
    lTransparency = true;

    png_destroy_read_struct(&lPngPtr, &lInfoPtr, NULL);
    return false;
    //goto ERROR;
  }
  // Expands PNG with less than 8bits per channel to 8bits.
  if (lDepth < 8) 
  {
    png_set_packing (lPngPtr);
    // Shrinks PNG with 16bits per color channel down to 8bits.
  }
  else if (lDepth == 16) 
  {
    png_set_strip_16(lPngPtr);
  }
  // Indicates that image needs conversion to RGBA if needed.
  switch (lColorType) 
  {
    case PNG_COLOR_TYPE_PALETTE:
      png_set_palette_to_rgb(lPngPtr);
      m_Format    = lTransparency ? GL_RGBA : GL_RGB;
      m_PixelSize = lTransparency ? 4 : 3;
      break;
    case PNG_COLOR_TYPE_RGB:
      m_Format    = lTransparency ? GL_RGBA : GL_RGB;
      m_PixelSize = lTransparency ? 4 : 3;
      break;
    case PNG_COLOR_TYPE_RGBA:
      m_Format    = GL_RGBA;
      m_PixelSize = 4;
      break;
    case PNG_COLOR_TYPE_GRAY:
      png_set_expand_gray_1_2_4_to_8(lPngPtr);
      m_Format    = lTransparency ? GL_LUMINANCE_ALPHA:GL_LUMINANCE;
      m_PixelSize = lTransparency ? 2 : 1;
      break;
    case PNG_COLOR_TYPE_GA:
      png_set_expand_gray_1_2_4_to_8(lPngPtr);
      m_Format = GL_LUMINANCE_ALPHA;
      m_PixelSize = 2;
      break;
  }
  png_read_update_info(lPngPtr, lInfoPtr);

  lRowSize = png_get_rowbytes(lPngPtr, lInfoPtr);
  if (lRowSize <= 0)
  {
    png_destroy_read_struct(&lPngPtr, &lInfoPtr, NULL);
    return false;
  }
  m_Data = (uint8_t*) malloc( lRowSize * lHeight );
  if (!m_Data)
  {
    png_destroy_read_struct(&lPngPtr, &lInfoPtr, NULL);
    return false;
  }

  lRowPtrs = new png_bytep[lHeight];
  if (!lRowPtrs)
  {
    png_destroy_read_struct(&lPngPtr, &lInfoPtr, NULL);
    free(m_Data); m_Data = NULL;
    return false;
  }
  for (unsigned int i = 0; i < lHeight; ++i) 
  {
//    lRowPtrs[lHeight - (i + 1)] = m_Data + i * lRowSize;
    lRowPtrs[i] = m_Data + i * lRowSize;
  }

  png_read_image(lPngPtr, lRowPtrs);

  //mResource.close();
  png_destroy_read_struct(&lPngPtr, &lInfoPtr, NULL);
  delete[] lRowPtrs;
#else
  VOLATILE_ARG( rawData );
#endif
  return true;
}
