/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : RotatePackage
*
* Description : Rotates an object around x,y,z
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "PreCompile.h"
#include "RunInParallelEffect.h"

RunInParallelEffect::RunInParallelEffect() :
  BaseEffect( 0.0f )
{
}

/************************************************************************************************
* AddEffect:
* Adds an effect to the run list. 
*
* @param  (const BaseEffect*)   effect - The effect to add
*
* @author Thor Richard Hansen
*************************************************************************************************/
void RunInParallelEffect::AddEffect( BaseEffect* effect )
{
  m_EffectList.push_back( effect );
}

bool RunInParallelEffect::Run( CoreBaseObject* runObject, float deltaTime )
{
  for ( std::vector< BaseEffect* >::iterator itEffect = m_EffectList.begin(); itEffect != m_EffectList.end(); )
  {
    BaseEffect* effect = *itEffect;
    if ( !effect->Started() )
    {
      effect->Start( runObject );
    }

    if ( !effect->Run( runObject, deltaTime ) )
    {
      effect->End( runObject );
      itEffect = m_EffectList.erase( itEffect );
    }
    else
    {
      itEffect++;
    }
  }

  return ( !m_EffectList.empty() );
}
