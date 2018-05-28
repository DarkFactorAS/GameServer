/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : SetGridpositionEffect
*
* Description : Sets a gridposition on a gameobject
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "PreCompile.h"
#include "SetGridpositionEffect.h"
#include "EvilGameEngine/CoreBoardGame/GameLogic/GameObject/GameObject.h"

SetGridpositionEffect::SetGridpositionEffect( float runTime, const Vector2& gridPosition ) :
  BaseEffect( runTime ),
  m_GridPosition( gridPosition )
{
}

void SetGridpositionEffect::Start( CoreBaseObject* /*runObject*/ )
{
}

void SetGridpositionEffect::End( CoreBaseObject* runObject )
{
  GameObject* gameObject = safe_cast< GameObject* > (runObject);
  if ( gameObject != NULL )
  {
    gameObject->SetGridPosition( m_GridPosition );
  }
}

bool SetGridpositionEffect::Run( CoreBaseObject* runObject, float deltaTime )
{
  return BaseEffect::Run( runObject, deltaTime );
}
