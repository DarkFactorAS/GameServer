
#include "Precompile.h"
#include "Touch.h"
#include "EvilEngine/CoreLib/CoreMath/CoreMath.h"

Touch::Touch()
{

}

Touch* Touch::GetInstance()
{
  static Touch* s_Instance = NULL;
  if ( s_Instance == NULL )
  {
    s_Instance = new Touch();
  }
  return s_Instance;
}

Touch::TouchPoint Touch::OnMouseButtonDown( int /*buttonId*/ )
{
  Touch::TouchPoint touchPoint( 0, 0 );
  return touchPoint;
}

const Vector2& Touch::GetPosition( int /*index*/ )
{
  return Vector2::s_Vec2Zero;
}
