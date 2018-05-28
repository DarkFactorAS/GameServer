/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : RotatePackage
*
* Description : Rotates an object around x,y,z
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "PreCompile.h"
#include "RunInSequenceEffect.h"

RunInSequenceEffect::RunInSequenceEffect() :
  BaseEffect( 1000000.0f )
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
void RunInSequenceEffect::AddEffect( BaseEffect* effect )
{
  m_EffectList.push_back( effect );
}

bool RunInSequenceEffect::Run( CoreBaseObject* runObject, float deltaTime )
{
  while( !m_EffectList.empty() )
  {
    std::vector< BaseEffect* >::iterator itEffect = m_EffectList.begin();
    if ( itEffect != m_EffectList.end() )
    {
      BaseEffect* effect = *itEffect;
      if ( !effect->Run( runObject, deltaTime ) )
      {
        m_EffectList.erase( itEffect );
        continue;
      }
    }
    break;
  }

  return ( !m_EffectList.empty() );
}
