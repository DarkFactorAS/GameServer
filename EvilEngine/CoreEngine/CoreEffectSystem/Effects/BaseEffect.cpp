/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : BasePackage
*
* Description : Basic implementation of an effect.
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "PreCompile.h"
#include "BaseEffect.h"

BaseEffect::BaseEffect( float runTime ) :
  m_RunTime( runTime ),
  m_TotalRunTime( runTime ),
  m_Started( false )
{
  static uint32 s_Id = 0;
  m_Id = ++s_Id;
}

bool BaseEffect::Started()
{
  if ( !m_Started )
  {
    m_Started = true;
    return false;
  }
  return true;
}

void BaseEffect::Start(CoreBaseObject* /*runObject*/)
{
  m_RunTime = m_TotalRunTime;
  m_Started = true;
}

bool BaseEffect::Run( CoreBaseObject* /*runObject*/, float deltaTime )
{
  m_RunTime -= deltaTime;
  return ( m_RunTime > 0.0f );
}
