
#include "Precompile.h"
#include "Vector3.h"
#include "Vector2.h"

Vector3 Vector3::s_Vec3Zero     = Vector3( 0.0f, 0.0f, 0.0f );
Vector3 Vector3::s_Vec3One      = Vector3( 1.0f, 1.0f, 1.0f );
Vector3 Vector3::s_Vec3Up       = Vector3( 0.0f, 1.0f, 0.0f );
Vector3 Vector3::s_Vec3Forward  = Vector3( 0.0f, 0.0f, 1.0f );
Vector3 Vector3::s_Vec3Right    = Vector3( 1.0f, 0.0f, 0.0f );

Vector3::Vector3()
{
  x = 0.0f;
  y = 0.0f;
  z = 0.0f;
}

Vector3::Vector3( float xx, float yy, float zz )
{
  x = xx;
  y = yy;
  z = zz;
}

Vector3::Vector3( const Vector2& vec )
{
  x = vec.x;
  y = vec.y;
  z = 0;
}

// the following functions are some matrix and vector helpers
// they work for this demo but in general it is recommended
// to use more advanced matrix libaries,
// e.g. OpenGL Mathematics (GLM)

float Vector3::Dot( const Vector3& a, const Vector3& b) 
{
  return a.x*b.x + a.y*b.y + a.z*b.z;
}

void Vector3::Cross( const Vector3& a, const Vector3& b, Vector3& res ) 
{
  res.x = a.y * b.z  -  b.y * a.z;
  res.y = a.z * b.x  -  b.z * a.x;
  res.z = a.x * b.y  -  b.x * a.y;
}

Vector3 Vector3::Cross( const Vector3& a, const Vector3& b ) 
{
  Vector3 res;
  res.x = a.y * b.z  -  b.y * a.z;
  res.y = a.z * b.x  -  b.z * a.x;
  res.z = a.x * b.y  -  b.x * a.y;
  return res;
}

void Vector3::Normalize() 
{
  float mag = sqrt( x * x  +  y * y  +  z * z );
  if ( mag != 0 )
  {
    x /= mag; 
    y /= mag; 
    z /= mag;
  }
}

/************************************************************************************************
* Normal:
* Returns the normal for a triangle.
*
* @param  (Vector3)   pointA       - The first point in the triangle
* @param  (Vector3)   pointB       - The second point in the triangle
* @param  (Vector3)   pointC       - The third point in the triangle
* @return (Vector3)                - The normal for the triangle
*
* @author Thor Richard Hansen
*************************************************************************************************/
Vector3 Vector3::Normal( const Vector3& pointA, const Vector3& pointB, const Vector3& pointC )
{
  Vector3 u,v, n;

  u.x = ( pointB.x - pointA.y );
  u.y = ( pointB.y - pointA.z );
  u.z = ( pointB.z - pointA.x );

  v.x = ( pointC.x - pointA.y );
  v.y = ( pointC.y - pointA.z );
  v.z = ( pointC.z - pointA.x );

  n.x = ( u.y * v.z ) - ( u.z * v.y );
  n.y = ( u.z * v.x ) - ( u.x * v.z );
  n.z = ( u.x * v.y ) - ( u.y * v.x );

  return n;
}


Vector3 Vector3::Normalize( const Vector3& otherVec )
{
  Vector3 newVec( otherVec );
  newVec.Normalize();
  return newVec;
}

Vector3 Vector3::ProjectXZ( const Vector3& otherVec )
{
  Vector3 newVec( otherVec );
  newVec.y = 0;
  return newVec;
}
