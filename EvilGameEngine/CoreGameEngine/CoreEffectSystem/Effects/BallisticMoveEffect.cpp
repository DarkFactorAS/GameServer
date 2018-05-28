/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : BallisticMoveEffect
*
* Description : Moves an object from A to B with a ballist path ( like a catapult trajectory )
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "PreCompile.h"
#include "BallisticMoveEffect.h"

BallisticMoveEffect::BallisticMoveEffect( float runTime, const Vector3& fromPosition, const Vector3& toPosition, const Vector3& path ) :
  BaseEffect( runTime ),
  m_RunTime( 0.0f ),
  m_TotalTime( runTime ),
  m_FromPosition( fromPosition ),
  m_ToPosition( toPosition ),
  m_Path( path )
{
}

void BallisticMoveEffect::Start( CoreBaseObject* runObject )
{
  runObject->SetPosition( m_FromPosition );
}

void BallisticMoveEffect::End( CoreBaseObject* runObject )
{
  runObject->SetPosition( m_ToPosition );
}

bool BallisticMoveEffect::Run( CoreBaseObject* runObject, float deltaTime )
{
  m_RunTime += deltaTime;
  float vTime = ( m_RunTime / m_TotalTime );

  // Move towards the end position
  Vector3 position = Vec3Lerp( m_FromPosition, m_ToPosition, vTime );

  // Add the arch
  position.z += m_Path.z * sinf( 3.14f * vTime );

  // Set the position
  runObject->SetPosition( position );

  return BaseEffect::Run( runObject, deltaTime );
}

Vector3 BallisticMoveEffect::GetZArch( const Vector3& fromPosition, const Vector3& endPositon, float miniumValue )
{
  Vector3 xzPlane = endPositon - fromPosition; xzPlane.y = 0;
  float len = Vec3Length( xzPlane ) / 3.0f;
  len = std::max( len, miniumValue );
  return Vector3( 0, 0, len );
}
