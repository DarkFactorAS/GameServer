#ifndef EVILENGINE_CORELIB_COREMATH_VECTOR4
#define EVILENGINE_CORELIB_COREMATH_VECTOR4 1

class Vector4
{
public:

  Vector4()
  {
    m     = new float[4];
    m[0]  = 0;
    m[1]  = 0;
    m[2]  = 0;
    m[3]  = 1;

    x = 0;
    y = 0;
    z = 0;
    w = 0;
  }

  Vector4( float xx, float yy, float zz, float ww )
  {
    m     = new float[4];
    m[0]  = xx;
    m[1]  = yy;
    m[2]  = zz;
    m[3]  = ww;

    x = xx;
    y = yy;
    z = zz;
    w = ww;
  }

  void          Set( float xx, float yy, float zz ){ x = xx; m[0] = xx; y = yy; m[1] = yy; z = zz; m[2] = zz; }
  void          SetX( float xx ){ x = xx; m[0] = xx; }
  void          SetY( float yy ){ y = yy; m[1] = yy; }
  void          SetZ( float zz ){ z = zz; m[2] = zz; }
  void          SetW( float ww ){ w = ww; m[3] = ww; }

  float* m;
  float  x;
  float  y;
  float  z;
  float  w;
};

#endif /// EVILENGINE_CORELIB_COREMATH_VECTOR4