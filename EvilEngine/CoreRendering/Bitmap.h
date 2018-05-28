#ifndef EVILENGINE_CORERENDERING_BITMAP
#define EVILENGINE_CORERENDERING_BITMAP 1

#include "../CoreLib/CoreMath/Vector2.h"
#include "../CoreLib/CoreMath/Rect.h"
#include "../CoreLib/CoreMath/Point.h"
#include <string.h>

class Bitmap
{
public:

  Bitmap( int width, int height, int format );
  Bitmap( int width, int height, int format, uint8* data );
  virtual ~Bitmap();

  uint32  GetPixel( int x, int y );
  void    CopyArea( const PointI& dstPoint, const Bitmap& src, const RectInt& srcArea, bool flipped );

  int     m_Width;
  int     m_Height;
  int     m_PixelFormat;
  int     m_PixelSize;
  uint8*  m_Data;
};

#endif /// EVILENGINE_CORERENDERING_BITMAP
