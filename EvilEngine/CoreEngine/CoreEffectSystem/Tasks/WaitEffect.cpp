/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : WaitEffect
*
* Description : Waits x seconds until next effect is executed
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "PreCompile.h"
#include "WaitEffect.h"

WaitEffect::WaitEffect( float runTime ) :
  BaseEffect( runTime )
{
}

bool WaitEffect::Run( CoreBaseObject* runObject, float deltaTime )
{
  return BaseEffect::Run( runObject, deltaTime );
}
