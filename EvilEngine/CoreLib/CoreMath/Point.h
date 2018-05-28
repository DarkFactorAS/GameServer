#ifndef EVILENGINE_CORELIB_COREMATH_POINT
#define EVILENGINE_CORELIB_COREMATH_POINT 1

class PointI
{
public:

  PointI( int xx, int yy )
  {
    x = xx;
    y = yy;
  }

  void Set( int xx, int yy )
  {
    x = xx;
    y = yy;
  }

  int x;
  int y;
};

#endif /// EVILENGINE_CORELIB_COREMATH_POINT
