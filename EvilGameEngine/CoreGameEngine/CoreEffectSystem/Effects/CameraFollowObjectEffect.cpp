/*************************************************************************************************
* Project     : EvilEngine Multi-Platform 2014-2015
* File        : CameraFollowObjectEffect
*
* Description : Moves the camera to always look at the GameObject
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "PreCompile.h"
#include "CameraFollowObjectEffect.h"
#include "EvilEngine/CoreRendering/Camera/RenderCamera.h"

CameraFollowObjectEffect::CameraFollowObjectEffect( float runTime, bool onlyXY ) :
  BaseEffect( runTime ),
  m_OnlyXY( onlyXY )
{
}

void CameraFollowObjectEffect::Start( CoreBaseObject* runObject )
{
  m_CurrentPos = runObject->GetPosition();
  RenderCamera::GetInstance()->MoveToPosition( m_CurrentPos );
}

void CameraFollowObjectEffect::End( CoreBaseObject* runObject )
{
  const Vector3& targetPosition = runObject->GetPosition();
  RenderCamera::GetInstance()->MoveToPosition( targetPosition );
}

bool CameraFollowObjectEffect::Run( CoreBaseObject* runObject, float deltaTime )
{
  RenderCamera* camera = RenderCamera::GetInstance();
  if ( m_OnlyXY )
  {
    const Vector3& targetPosition = runObject->GetPosition();
    m_CurrentPos.x = targetPosition.x;
    m_CurrentPos.y = targetPosition.y;
    camera->MoveToPosition( m_CurrentPos );
  }
  else
  {
    const Vector3& targetPosition = runObject->GetPosition();
    camera->MoveToPosition( targetPosition );
  }
  return BaseEffect::Run( runObject, deltaTime );
}
