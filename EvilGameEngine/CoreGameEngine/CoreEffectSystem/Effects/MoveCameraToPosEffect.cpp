/*************************************************************************************************
* Project     : EvilEngine Multi-Platform 2014-2015
* File        : CameraFollowObjectEffect
*
* Description : Moves the camera to always look at the GameObject
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "PreCompile.h"
#include "MoveCameraToPosEffect.h"
#include "EvilEngine/CoreRendering/Camera/RenderCamera.h"

MoveCameraToPosEffect::MoveCameraToPosEffect( float runTime, const Vector3& toPosition ) :
  BaseEffect( runTime )
{
  m_StartPosition = toPosition;
  m_EndPosition   = toPosition;
  m_RunTime       = 0.0f;
  m_TotalTime     = runTime;
}

void MoveCameraToPosEffect::Start( CoreBaseObject* /*runObject*/ )
{
  m_StartPosition = RenderCamera::GetInstance()->GetTargetPosition();
}

void MoveCameraToPosEffect::End( CoreBaseObject* /*runObject*/ )
{
  RenderCamera::GetInstance()->MoveToPosition( m_EndPosition );
}

bool MoveCameraToPosEffect::Run( CoreBaseObject* runObject, float deltaTime )
{
  m_RunTime += deltaTime;
  float vDelta = ( m_RunTime / m_TotalTime );
  if ( vDelta > 1.0f ){ vDelta = 1.0f; }

  // Move towards the end position
  Vector3 position = Vec3Lerp( m_StartPosition, m_EndPosition, vDelta );

  // Add the arch
  //position.z += m_Path.z * sinf( 3.14f * vTime );

  RenderCamera::GetInstance()->MoveToPosition( position );

  return BaseEffect::Run( runObject, deltaTime );
}
