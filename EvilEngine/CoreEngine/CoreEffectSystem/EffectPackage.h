#ifndef EVILENGINE_COREENGINE_COREEFFECTSYSTEM_EFFECTPACKAGE
#define EVILENGINE_COREENGINE_COREEFFECTSYSTEM_EFFECTPACKAGE 1
#pragma once

#include "EvilEngine/CoreLib/Signal/SignalTarget.h"
//#include "EvilGameEngine/CoreGameEngine/CoreEffectSystem/Effects/WaitEffect.h"
//#include "EvilGameEngine/CoreGameEngine/CoreEffectSystem/Effects/RotateEffect.h"
//#include "EvilGameEngine/CoreGameEngine/CoreEffectSystem/Effects/MoveEffect.h"
//#include "EvilGameEngine/CoreGameEngine/CoreEffectSystem/Effects/FadeEffect.h"
//#include "EvilGameEngine/CoreGameEngine/CoreEffectSystem/Effects/RunInParallelEffect.h"
//#include "EvilGameEngine/CoreGameEngine/CoreEffectSystem/Effects/RunInSequenceEffect.h"

class BaseEffect;
class CoreBaseObject;

class EffectPackage : public SignalTarget
{
public:
                              EffectPackage( CoreBaseObject* runObject, uint32 effectFlags );
  virtual                     ~EffectPackage();

  BaseEffect*                 AddEffect( BaseEffect* effect );
  uint32                      GetId() const { return m_Id; }
  bool                        Run( float deltaTime );
  CoreBaseObject*             GetRunObject() const{ return m_RunObject; }
  void                        Delete( const CoreBaseObject* runObject );
  bool                        ShouldDestroy() const{ return m_Destroy; }
  void                        DestroyObjectOnDeath(){ m_DestroyObjectOnDeath = true; }
  uint32                      GetFlags(){ return m_Flags; }

  Signal1< void, CoreBaseObject* >  SignalDone;

private:

  CoreBaseObject*             m_RunObject;
  uint32                      m_Id;
  std::vector< BaseEffect* >  m_EffectList;
  uint32                      m_Flags;
  bool                        m_Destroy;
  bool                        m_DestroyObjectOnDeath;
};

#endif /// EVILENGINE_COREENGINE_COREEFFECTSYSTEM_EFFECTPACKAGE