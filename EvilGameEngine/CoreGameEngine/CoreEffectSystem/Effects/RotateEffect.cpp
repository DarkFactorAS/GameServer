/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : RotatePackage
*
* Description : Rotates an object around x,y,z
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "PreCompile.h"
#include "RotateEffect.h"

RotateEffect::RotateEffect( float runTime, const Vector3& rotationAxis ) :
  BaseEffect( runTime ),
  m_Rotation( rotationAxis )
{
}

bool RotateEffect::Run( CoreBaseObject* runObject, float deltaTime )
{
  Vector3 rot = runObject->GetRotation();

  if ( m_Rotation.x != 0.0f )
  {
    rot.x+= 90 * deltaTime * m_Rotation.x;
    if ( rot.x > 360.0f )
    {
      int n = (int)(rot.x / 360.0f);
      rot.x -= 360.0f * n;
    }
    if ( rot.x < 0 )
    {
      rot.x = 0;
    }
  }

  if ( m_Rotation.y != 0.0f )
  {
    rot.y+= 90 * deltaTime * m_Rotation.y;
    if ( rot.y > 360.0f )
    {
      int n = (int)(rot.y / 360.0f);
      rot.y -= 360.0f * n;
    }
    if ( rot.y < 0 )
    {
      rot.y = 0;
    }
  }

  if ( m_Rotation.z != 0.0f )
  {
    rot.z+= 90 * deltaTime * m_Rotation.z;
    if ( rot.z > 360.0f )
    {
      int n = (int)(rot.z / 360.0f);
      rot.z -= 360.0f * n;
    }
    if ( rot.z < -360.0f )
    {
      int n = (int)(rot.z / 360.0f);
      rot.z -= 360.0f * n;
    }
  }

  // Just rotate all 3 angles
  runObject->SetRotation( rot.x, rot.y, rot.z );

  return BaseEffect::Run( runObject, deltaTime );
}
