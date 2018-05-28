/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : RotatePackage
*
* Description : Rotates an object around x,y,z
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "PreCompile.h"
#include "MoveEffect.h"

MoveEffect::MoveEffect( float runTime, const Vector3& fromPosition, const Vector3& toPosition ) :
  BaseEffect( runTime ),
  m_FromPosition( fromPosition ),
  m_ToPosition( toPosition )
{
  m_Speed = ( m_ToPosition - m_FromPosition ) * ( 1 / runTime );
}

bool MoveEffect::Run( CoreBaseObject* runObject, float deltaTime )
{
  Vector3 position = runObject->GetPosition();

  position.x += m_Speed.x * deltaTime;
  position.y += m_Speed.y * deltaTime;
  position.z += m_Speed.z * deltaTime;

  // Just rotate all 3 angles
  runObject->SetPosition( position );

  return BaseEffect::Run( runObject, deltaTime );
}
