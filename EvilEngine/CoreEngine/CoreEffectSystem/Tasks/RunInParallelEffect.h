#ifndef EVILENGINE_COREGAMEENGINE_COREEFFECTSYSTEM_EFFECTS_RUNINPARALLELEFFECT
#define EVILENGINE_COREGAMEENGINE_COREEFFECTSYSTEM_EFFECTS_RUNINPARALLELEFFECT 1
#pragma once

#include "EvilEngine/CoreEngine/CoreEffectSystem/Effects/BaseEffect.h"
#include "EvilEngine/CoreLib/CoreMath/CoreMath.h"

class RunInParallelEffect : public BaseEffect
{
public:
  RunInParallelEffect();
  virtual ~RunInParallelEffect(){};

  virtual bool                Run( CoreBaseObject* runObject, float deltaTime ) DF_OVERRIDE;

  void                        AddEffect( BaseEffect* effect );

private:

  std::vector< BaseEffect* >  m_EffectList;
};

#endif /// EVILENGINE_COREGAMEENGINE_COREEFFECTSYSTEM_EFFECTS_RUNINPARALLELEFFECT