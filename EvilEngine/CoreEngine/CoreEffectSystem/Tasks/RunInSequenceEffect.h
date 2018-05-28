#ifndef EVILENGINE_COREGAMEENGINE_COREEFFECTSYSTEM_EFFECTS_RUNINSEQUENCEEFFECT
#define EVILENGINE_COREGAMEENGINE_COREEFFECTSYSTEM_EFFECTS_RUNINSEQUENCEEFFECT 1
#pragma once

#include "EvilEngine/CoreEngine/CoreEffectSystem/Effects/BaseEffect.h"
#include "EvilEngine/CoreLib/CoreMath/CoreMath.h"

class RunInSequenceEffect : public BaseEffect
{
public:
  RunInSequenceEffect();
  virtual ~RunInSequenceEffect(){};

  virtual bool                Run( CoreBaseObject* runObject, float deltaTime ) DF_OVERRIDE;

  void                        AddEffect( BaseEffect* effect );

private:

  std::vector< BaseEffect* >  m_EffectList;
};

#endif /// EVILENGINE_COREGAMEENGINE_COREEFFECTSYSTEM_EFFECTS_RUNINSEQUENCEEFFECT