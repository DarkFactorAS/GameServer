#ifndef EVILENGINE_CORELIB_COREMATH_VECTOR2
#define EVILENGINE_CORELIB_COREMATH_VECTOR2 1

#include "Vector3.h"

class Vector2
{
public:

  static Vector2 s_Vec2Zero;
  static Vector2 s_Vec2One;
  static Vector2 s_Vec2Minus;

  Vector2();
  Vector2( float xx, float yy );
  Vector2( const Vector3& vec );

  void      Set( float xx, float yy );
  void      SetUInt16( uint16 xx, uint16 yy );
  int       GetIntX() const { return (int)x;}
  int       GetIntY() const { return (int)y;}
  uint16    GetUInt16X() const { return (uint16)x;}
  uint16    GetUInt16Y() const { return (uint16)y;}
  float     GetX() const { return x;}
  float     GetY() const { return y;}

  bool      IsNull() const{ return m_IsNull;}
  bool      IsValid() const{ return ( x != -1000 || y != -1000 );}

  bool operator == ( const Vector2& othervec ) const
  {
    return ( x == othervec.x && y == othervec.y );
  }
  bool operator != ( const Vector2& othervec ) const
  {
    return ( x != othervec.x || y != othervec.y );
  }
//private:
  float x;
  float y;
  bool m_IsNull;
};

#endif /// EVILENGINE_CORELIB_COREMATH_VECTOR2
