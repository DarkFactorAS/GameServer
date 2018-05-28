#ifndef EVILENGINE_CORELIB_COREMATH_RECT
#define EVILENGINE_CORELIB_COREMATH_RECT 1

class RectInt
{
public:

  RectInt( int x, int y, int width, int height ) :
    m_X( x ),
    m_Y( y ),
    m_Width( width ),
    m_Height( height )
  {
  }

  int CenterX() const { return m_X; }
  int CenterY() const { return m_Y; }
  int Width()   const { return m_Width; }
  int Height()  const { return m_Height; }

  int m_X;
  int m_Y;
  int m_Width;
  int m_Height;
};

class RectF
{
public:

  RectF( float x, float y, float width, float height ) :
    m_X( x ),
    m_Y( y ),
    m_Width( width ),
    m_Height( height )
  {
  }

  float CenterX() const { return m_X; }
  float CenterY() const { return m_Y; }
  float Width()   const { return m_Width; }
  float Height()  const { return m_Height; }

  float m_X;
  float m_Y;
  float m_Width;
  float m_Height;
};

#endif /// EVILENGINE_CORELIB_COREMATH_RECTF