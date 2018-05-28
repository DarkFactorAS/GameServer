
#include "PreCompile.h"
#include "Bitmap.h"

Bitmap::Bitmap( int width, int height, int format ) :
  m_Width( width ),
  m_Height( height ),
  m_PixelSize( 4 ),
  m_PixelFormat( format ),
  m_Data( NULL )
{
  m_Data        = (uint8*) malloc( m_Width * m_Height * m_PixelSize );
  if ( m_Data != NULL )
  {
    memset( m_Data, 0, m_Width * m_Height * m_PixelSize );
  }
}

Bitmap::Bitmap( int width, int height, int format, uint8* data ) :
  m_Width( width ),
  m_Height( height ),
  m_PixelSize( 4 ),
  m_PixelFormat( format ),
  m_Data( data )
{
}

Bitmap::~Bitmap()
{
  free( m_Data );
}

/************************************************************************************************
* GetPixel:
* Returns the 4 byte pixel at a specific location
*
* @param  (int)    x - The x position for the pixel
* @param  (int)    y - The y position for the pixel
* @return (uint32)     The pixel at the location ( In RGB-A format )
*
* @author Thor Richard Hansen
*************************************************************************************************/
uint32 Bitmap::GetPixel( int x, int y )
{
  int pos = ( y * m_Width * m_PixelSize) + ( x * m_PixelSize );
  int r = m_Data[ pos + 0 ];
  int g = m_Data[ pos + 1 ];
  int b = m_Data[ pos + 2 ];
  int a = m_Data[ pos + 3 ];

  return ( a << 24 ) + ( r << 16  ) + ( g << 8 ) + b;
}

/************************************************************************************************
* CopyArea:
* Copy an area from another bitmap into this one.
*
* @param  (const Vector2&)   dstPoint      - The top/left corner in THIS bitmap where we start
*                                            to draw the other bitmap.
* @param  (const Bitmap&)    src           - The bitmap to copy the data from
* @param  (const RectF&)     srcArea       - The rectangle area in the source bitmap to copy from
*
* @author Thor Richard Hansen
*************************************************************************************************/
void Bitmap::CopyArea( const PointI& dstPoint, const Bitmap& src, const RectInt& srcArea, bool flipped )
{
  // Source data
  int srcByteWidth      = src.m_Width * src.m_PixelSize;
  int srcByteStartX     = srcArea.m_X * src.m_PixelSize;
  int srcByteStartY     = srcArea.m_Y * src.m_PixelSize * src.m_Width;

  // Destination data
  int destByteWidth     = srcArea.m_Width * m_PixelSize;
  int destTexByteWidth  = m_Width * m_PixelSize;
  //int destImageSize     = m_Width * m_Height * m_PixelSize;

  int destByteStartX    = dstPoint.x * m_PixelSize;
  int destByteStartY    = dstPoint.y * m_PixelSize * m_Width;

  // Blit all pixels to the correct position
  int yy = 0;
  for ( int y = 0; y < srcArea.m_Height; y++ )
  {
    if ( flipped )
    {
      yy = srcArea.m_Height - y - 1;
    }
    else
    {
      yy = y;
    }
    uint8* memSrc = src.m_Data + srcByteStartX + srcByteStartY + ( srcByteWidth * yy );
    uint8* memDst = m_Data + destByteStartX + destByteStartY + ( destTexByteWidth * y );
    memcpy( memDst, memSrc, destByteWidth );
  }
}
