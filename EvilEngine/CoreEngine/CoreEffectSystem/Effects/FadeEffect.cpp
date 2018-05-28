/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : FadeOutEffect
*
* Description : Fades out an object from current fade to 0.0 in the runTime period
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "PreCompile.h"
#include "FadeEffect.h"

FadeEffect::FadeEffect( float runTime, float startAlpha, float endAlpha ) :
  BaseEffect( runTime ),
  m_FadeSpeed( 0.0f )
{
  if ( runTime > 0.0f )
  {
    m_FadeSpeed = ( endAlpha - startAlpha ) / runTime;
  }
  m_Alpha       = startAlpha;
  m_StartAlpha  = startAlpha;
  m_EndAlpha    = endAlpha;
}

void FadeEffect::Start( CoreBaseObject* runObject )
{
  BaseEffect::Start( runObject );
  m_Alpha = m_StartAlpha;
  runObject->SetAlpha( m_Alpha );
}

void FadeEffect::End( CoreBaseObject* runObject )
{
  runObject->SetAlpha( m_EndAlpha );
}

bool FadeEffect::Run( CoreBaseObject* runObject, float deltaTime )
{
  m_Alpha += m_FadeSpeed * deltaTime;
  runObject->SetAlpha( m_Alpha );
  return BaseEffect::Run( runObject, deltaTime );
}
