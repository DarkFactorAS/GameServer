#include "Precompile.h"
#include "Quaternion.h"

Quaternion::Quaternion()
{
  x = 0;
  y = 0;
  z = 0;
  w = 0;
}

Quaternion::Quaternion( float xx, float yy, float zz, float ww )
{
  x = xx;
  y = yy;
  z = zz;
  w = ww;
}

Quaternion::Quaternion( const Vector3& vec, float ww )
{
  x = vec.x;
  y = vec.y;
  z = vec.z;
  w = ww;
}

Quaternion::Quaternion( const Vector3& cV1, const Vector3& cV2 )
{
  Vector3 h = cV1 + cV2;
  //! support rotation through 180 deg
  //! note: rotation from +/-y to -/+y is not supported, but rarely needed.
  float len = Vec3LengthSq( h );
  if(len < 1e-12f)
  {
    h = Vector3(cV1.z,0,-cV1.x);
    len = h.x*h.x + h.z*h.z;
    if(len < 1e-12f)
    {
      x = 0.0f;
      y = 0.0f;
      z = 0.0f;
      w = 1.0f;
      //fcBugreportFmt(false, ("Bad Quaternion((%f,%f,%f) - (%f,%f,%f))\n", cV1.x, cV1.y, cV1.z, cV2.x, cV2.y, cV2.z));

      return;
    }
  }
  h = h * ( 1 /  sqrtf( len ) );

  x = cV1.y * h.z - cV1.z * h.y;
  y = cV1.z * h.x - cV1.x * h.z;
  z = cV1.x * h.y - cV1.y * h.x;
  w = cV1.x * h.x + cV1.y * h.y + cV1.z * h.z;
}
