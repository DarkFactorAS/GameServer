/*************************************************************************************************
* Project     : EvilEngine Multi-Platform 2014-2015
* File        : CameraFollowObjectEffect
*
* Description : Moves the camera to always look at the GameObject
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "PreCompile.h"
#include "CameraRotateAroundObjectEffect.h"
#include "EvilEngine/CoreRendering/Camera/RenderCamera.h"

CameraRotateAroundObjectEffect::CameraRotateAroundObjectEffect( float runTime, float speed, bool loop ) :
  BaseEffect( runTime ),
  m_Speed( speed ),
  m_Loop( loop )
{
}

void CameraRotateAroundObjectEffect::Start( CoreBaseObject* runObject )
{
  if ( runObject != NULL )
  {
    RenderCamera::GetInstance()->MoveToPosition( runObject->GetPosition() );
  }
}

void CameraRotateAroundObjectEffect::End( CoreBaseObject* runObject )
{
  if ( runObject != NULL )
  {
    RenderCamera::GetInstance()->MoveToPosition( runObject->GetPosition() );
  }
}

bool CameraRotateAroundObjectEffect::Run( CoreBaseObject* runObject, float deltaTime )
{
  RenderCamera* camera = RenderCamera::GetInstance();
  camera->UpdateYaw( deltaTime * m_Speed );

  if ( !m_Loop )
  {
    return BaseEffect::Run( runObject, deltaTime );
  }
  return true;
}
