
#ifndef EVILENGINE_CORELIB_COREMATH_VECTOR3
#define EVILENGINE_CORELIB_COREMATH_VECTOR3 1

#include <math.h>

class Vector2;

class Vector3
{
public:
  Vector3();
  Vector3( float x, float y, float z );
  Vector3( const Vector2& vec );

  void                  Normalize();
  void                  Set( float xx, float yy, float zz ){ x = xx; y = yy; z = zz; }
  void                  Set( const Vector3& vec ){ x = vec.x; y = vec.y; z = vec.z; }
  bool                  IsNull() const{ return ( x == 0 && y == 0 && z == 0 );}
  static Vector3        ProjectXZ( const Vector3& vector );
  static Vector3        Normalize( const Vector3& vector );
  static Vector3        Normal( const Vector3& pointA, const Vector3& pointB, const Vector3& pointC );

  static float          Dot( const Vector3& a, const Vector3& b);
  static void           Cross( const Vector3& a, const Vector3& b, Vector3& res );
  static Vector3        Cross( const Vector3& a, const Vector3& b );

  int                   GetIntX() const { return (int)x;}
  int                   GetIntY() const { return (int)y;}
  int                   GetIntZ() const { return (int)z;}

  void                  SetIntX( int xx ){ x = (float)xx; }
  void                  SetIntY( int yy ){ y = (float)yy; }
  void                  SetIntZ( int zz ){ z = (float)zz; }

  
  const Vector3&  operator  =( const Vector3& othervec )           { x = othervec.x; y = othervec.y; z = othervec.z; return *this; }

  static Vector3 s_Vec3Zero;
  static Vector3 s_Vec3One;
  static Vector3 s_Vec3Up;
  static Vector3 s_Vec3Forward;
  static Vector3 s_Vec3Right;

  float x;
  float y;
  float z;
};

inline const Vector3  operator +( const Vector3& lhs, const Vector3& rhs )     { return Vector3( lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z ); }
inline const Vector3  operator -( const Vector3& lhs, const Vector3& rhs )     { return Vector3( lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z ); }
inline const Vector3  operator *( const Vector3& lhs, float rhs )              { return Vector3( lhs.x * rhs, lhs.y * rhs, lhs.z * rhs ); }

inline float Vec3LengthSq( const Vector3& v )
{
  return ( v.x * v.x ) + ( v.y * v.y ) + ( v.z * v.z );
}

inline float Vec3Length( const Vector3& v )
{
  return sqrtf( Vec3LengthSq( v ));
}

inline const Vector3 Vec3Lerp( const Vector3& a, const Vector3& b, float s )
{
  return a + (( b - a ) * s ); 
}

inline float dfDeg( float rad )
{
  static float convert = 180.0f / (float)3.14159265358979323846f;
  return rad * convert;
}

#endif /// EVILENGINE_CORELIB_COREMATH_VECTOR3