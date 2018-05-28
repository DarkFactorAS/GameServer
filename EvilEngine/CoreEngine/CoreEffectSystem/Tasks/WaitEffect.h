#ifndef EVILENGINE_COREGAMEENGINE_COREEFFECTSYSTEM_EFFECTS_WAITEFFECT
#define EVILENGINE_COREGAMEENGINE_COREEFFECTSYSTEM_EFFECTS_WAITEFFECT 1
#pragma once

#include "EvilEngine/CoreEngine/CoreEffectSystem/Effects/BaseEffect.h"
#include "EvilEngine/CoreLib/CoreMath/CoreMath.h"

class WaitEffect : public BaseEffect
{
public:
  WaitEffect( float runTime );
  virtual ~WaitEffect(){};

  virtual bool  Run( CoreBaseObject* runObject, float deltaTime ) DF_OVERRIDE;
};

#endif /// EVILENGINE_COREGAMEENGINE_COREEFFECTSYSTEM_EFFECTS_WAITEFFECT