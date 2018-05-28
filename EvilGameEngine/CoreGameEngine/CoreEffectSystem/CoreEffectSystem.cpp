/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : CoreEffectSystem
*
* Description : Basic implementation for handling and running effects on gameobjects
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "CoreEffectSystem.h"
#include "EvilEngine/CoreEngine/CoreEffectSystem/EffectPackage.h"
#include "EvilEngine/CoreEngine/CoreBaseObject/CoreBaseObject.h"

CoreEffectSystem::CoreEffectSystem() :
  EngineModule( EngineModule::COREMODULETYPE_EFFECTSYSTEM )
{
}

CoreEffectSystem::~CoreEffectSystem()
{
  for ( std::vector< EffectPackage* >::iterator itEffectPackage = m_EffectPackageList.begin(); itEffectPackage != m_EffectPackageList.end(); ++itEffectPackage )
  {
    EffectPackage* effectPackage = *itEffectPackage;
    delete effectPackage;
  }
  m_EffectPackageList.clear();
}

void CoreEffectSystem::FrameProcess( float deltaTime )
{
  for ( std::vector< EffectPackage* >::iterator itEffectPackage = m_EffectPackageList.begin(); itEffectPackage != m_EffectPackageList.end(); )
  {
    EffectPackage* effectPackage = *itEffectPackage;
    if ( !effectPackage->ShouldDestroy() && effectPackage->Run( deltaTime ) )
    {
      itEffectPackage++;
    }
    else
    {
      delete effectPackage;
      itEffectPackage = m_EffectPackageList.erase( itEffectPackage );
    }
  }
}

/************************************************************************************************
* AddEffectPackage:
* Adds an effectpackage to the run list. The effectpackage will live until all effects in it
* is done executing
*
* @param  (const EffectPackage*)   effectPackage - The effectpackage to add
*
* @author Thor Richard Hansen
*************************************************************************************************/
void CoreEffectSystem::AddEffectPackage( EffectPackage* effectPackage )
{
  m_EffectPackageList.push_back( effectPackage );
}

/************************************************************************************************
* Create an effect package and add it to the CoreBaseObject:
*
* @param  (CoreBaseObject*)   runObject - The object that we want to create an effect package on
*
* @author Thor Richard Hansen
*************************************************************************************************/
EffectPackage* CoreEffectSystem::CreateEffectPackage( CoreBaseObject* runObject )
{
  EffectPackage* effectPackage = new EffectPackage( runObject, 0 );
  m_EffectPackageList.push_back( effectPackage );
  return effectPackage;
}

/************************************************************************************************
* RemoveEffectPackage:
* Removes an effectpackage from the run list. The effectpackage will live until all effects in it
* is done executing
*
* @param  (const EffectPackage*)   effectPackage - The effectpackage to add
*
* @author Thor Richard Hansen
*************************************************************************************************/
void CoreEffectSystem::RemoveEffectPackage( EffectPackage* removeEffectPackage )
{
  for ( std::vector< EffectPackage* >::iterator itEffectPackage = m_EffectPackageList.begin(); itEffectPackage != m_EffectPackageList.end(); ++itEffectPackage )
  {
    EffectPackage* effectPackage = *itEffectPackage;
    if ( effectPackage->GetId() == removeEffectPackage->GetId() )
    {
      m_EffectPackageList.erase( itEffectPackage );
      delete effectPackage;
      return;
    }
  }
}

/************************************************************************************************
* RemoveEffectPackage:
* Removes an effectpackage from the run list. The effectpackage will live until all effects in it
* is done executing
*
* @param  (const EffectPackage*)   effectPackage - The effectpackage to add
*
* @author Thor Richard Hansen
*************************************************************************************************/
void CoreEffectSystem::RemoveEffectpackagesOnObject( CoreBaseObject* runObject )
{
  if ( runObject == NULL )
  {
    return;
  }

  for ( std::vector< EffectPackage* >::iterator itEffectPackage = m_EffectPackageList.begin(); itEffectPackage != m_EffectPackageList.end(); )
  {
    EffectPackage* effectPackage = *itEffectPackage;
    if ( effectPackage->GetRunObject() == runObject )
    {
      itEffectPackage = m_EffectPackageList.erase( itEffectPackage );
      delete effectPackage;
      continue;
    }
    else
    {
      itEffectPackage++;
    }
  }
}
