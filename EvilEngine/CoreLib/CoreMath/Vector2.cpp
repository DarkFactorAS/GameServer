
#include "Precompile.h"
#include "Vector2.h"

Vector2 Vector2::s_Vec2Zero     = Vector2( 0.0f, 0.0f );
Vector2 Vector2::s_Vec2One      = Vector2( 1.0f, 1.0f );
Vector2 Vector2::s_Vec2Minus    = Vector2( -1000.0f, -1000.0f );

Vector2::Vector2()
{
  x         = 0;
  y         = 0;
  m_IsNull  = true;
}

Vector2::Vector2( float xx, float yy )
{
  x = xx;
  y = yy;
  m_IsNull  = false;
}

Vector2::Vector2( const Vector3& vec )
{
  x = vec.x;
  y = vec.y;
  m_IsNull  = false;
}

void Vector2::Set( float xx, float yy )
{
  x = xx;
  y = yy;
  m_IsNull  = false;
}

void Vector2::SetUInt16( uint16 xx, uint16 yy )
{
  x = (float)xx;
  y = (float)yy;
  m_IsNull  = false;
}
