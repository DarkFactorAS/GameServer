#ifndef EVILENGINE_COREGAMEENGINE_COREEFFECTSYSTEM_EFFECTS_ROTATEEFFECT
#define EVILENGINE_COREGAMEENGINE_COREEFFECTSYSTEM_EFFECTS_ROTATEEFFECT 1
#pragma once

#include "EvilEngine/CoreEngine/CoreEffectSystem/Effects/BaseEffect.h"
#include "EvilEngine/CoreLib/CoreMath/CoreMath.h"

class RotateEffect : public BaseEffect
{
public:
  RotateEffect( float runTime, const Vector3& rotationAxis );
  virtual ~RotateEffect(){};

  virtual bool  Run( CoreBaseObject* runObject, float deltaTime ) DF_OVERRIDE;

private:

  Vector3                           m_Rotation;
};

#endif /// EVILENGINE_COREGAMEENGINE_COREEFFECTSYSTEM_EFFECTS_ROTATEEFFECT