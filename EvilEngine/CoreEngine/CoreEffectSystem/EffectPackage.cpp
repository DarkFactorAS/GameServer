/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : EffectPackage
*
* Description : An effectpackage can hold and run multiple effects. All effects in the effect
*               package will run in sequence.
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "EffectPackage.h"
#include "Effects/BaseEffect.h"
#include "EvilEngine/CoreLib/Assert/DFAssert.h"

EffectPackage::EffectPackage( CoreBaseObject* runObject, uint32 effectFlags ) :
  m_RunObject( runObject ),
  m_Flags( effectFlags ),
  m_Destroy( false ),
  m_DestroyObjectOnDeath( false )
{
  static uint32 s_Id = 0;
  m_Id = ++s_Id;

  // When the runobject dies, so does the effect packages on it
  if ( runObject != NULL )
  {
    runObject->m_SignalOnDeath.Connect( this, &EffectPackage::Delete );
  }
}

EffectPackage::~EffectPackage()
{
  SignalDone( m_RunObject );

  if ( m_DestroyObjectOnDeath && m_RunObject )
  {
    m_RunObject->Destroy();
    m_RunObject = NULL;
  }
}

void EffectPackage::Delete( const CoreBaseObject* runObject )
{
  if (m_RunObject != runObject)
  {
    LogFatal("EffectPackage", "EffectPackage. RunObjects does not match!");
  }
  m_Destroy               = true;
  m_DestroyObjectOnDeath  = false;
  m_RunObject             = NULL;
}

/************************************************************************************************
* AddEffect:
* Adds an effect to the run list. 
*
* @param  (const BaseEffect*)   effect - The effect to add
*
* @author Thor Richard Hansen
*************************************************************************************************/
BaseEffect* EffectPackage::AddEffect( BaseEffect* effect )
{
  m_EffectList.push_back( effect );
  return effect;
}

/************************************************************************************************
* Run:
* Runs the effects in sequence. If the effect is done executing ( returns false in Run ), that 
* effect is done and we continue to the next effect until the list is empty.
*
* @param  (float)   deltaTime - Amount of seconds that have passed since last call
*
* @author Thor Richard Hansen
*************************************************************************************************/
bool EffectPackage::Run( float deltaTime )
{
  while( !m_EffectList.empty() )
  {
    std::vector< BaseEffect* >::iterator itEffect = m_EffectList.begin();
    if ( itEffect != m_EffectList.end() )
    {
      BaseEffect* effect = *itEffect;
      if ( !effect->Started() )
      {
        effect->Start( m_RunObject );
      }

      if ( !effect->Run( m_RunObject, deltaTime ) )
      {
        effect->End( m_RunObject );
        m_EffectList.erase( itEffect );
        continue;
      }
    }
    break;
  }

  return !m_EffectList.empty();
}
