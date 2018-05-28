#ifndef EVILENGINE_CORELIB_COREMATH_QUATERNION
#define EVILENGINE_CORELIB_COREMATH_QUATERNION 1

#include "Vector3.h"
#include <math.h>

class Quaternion
{
public:

  Quaternion();
  Quaternion( float x, float y, float z, float w );
  Quaternion( const Vector3& vec, float w );
  Quaternion( const Vector3& cV1, const Vector3& cV2 );

  float x;
  float y;
  float z;
  float w;
};

inline const Quaternion operator *( const Quaternion& a, const Quaternion& b )
{
  return Quaternion( b.w * a.x + b.x * a.w + b.y * a.z - b.z * a.y,
    b.w * a.y - b.x * a.z + b.y * a.w + b.z * a.x,
    b.w * a.z + b.z * a.w + b.x * a.y - b.y * a.x,
    b.w * a.w - b.x * a.x - b.y * a.y - b.z * a.z );
}

inline const Quaternion operator *( const Quaternion& lhs, float rhs )
{
  return Quaternion( lhs.x * rhs,
    lhs.y * rhs,
    lhs.z * rhs,
    lhs.w * rhs );
}

inline const Vector3 Vec3TransformCoord( const Vector3& v, const Quaternion& q )
{
  float tx = (( q.y * v.z ) - ( q.z * v.y )) * 2.0f;
  float ty = (( q.z * v.x ) - ( q.x * v.z )) * 2.0f;
  float tz = (( q.x * v.y ) - ( q.y * v.x )) * 2.0f;

  return Vector3( v.x + ( tx * q.w ) + ( q.y * tz ) - ( q.z * ty ),
    v.y + ( ty * q.w ) + ( q.z * tx ) - ( q.x * tz ),
    v.z + ( tz * q.w ) + ( q.x * ty ) - ( q.y * tx ));
}

inline const Vector3  operator *( const Vector3& lhs, const Quaternion& rhs )  { return Vec3TransformCoord( lhs, rhs ); }

inline const Quaternion QuatRotationAxis( const Vector3& v, float angle )
{
  Vector3 n(v);
  n.Normalize();
  float     fHalfAngle = 0.5f * angle;
  float     fSin = sinf( fHalfAngle );

  return Quaternion( fSin * n.x,
    fSin * n.y,
    fSin * n.z,
    cosf( fHalfAngle ));
}

#endif /// EVILENGINE_CORELIB_COREMATH_QUATERNION
