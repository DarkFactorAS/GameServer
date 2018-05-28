#ifndef EVILENGINE_COREGAMEENGINE_COREEFFECTSYSTEM_EFFECTS_MOVEEFFECT
#define EVILENGINE_COREGAMEENGINE_COREEFFECTSYSTEM_EFFECTS_MOVEEFFECT 1
#pragma once

#include "EvilEngine/CoreEngine/CoreEffectSystem/Effects/BaseEffect.h"
#include "EvilEngine/CoreLib/CoreMath/CoreMath.h"

class MoveEffect : public BaseEffect
{
public:
  MoveEffect( float runTime, const Vector3& fromPosition, const Vector3& toPosition );
  virtual ~MoveEffect(){};

  virtual bool  Run( CoreBaseObject* runObject, float deltaTime ) DF_OVERRIDE;

private:

  float                             m_RunTime;
  Vector3                           m_FromPosition;
  Vector3                           m_ToPosition;
  Vector3                           m_Speed;
};

#endif /// EVILENGINE_COREGAMEENGINE_COREEFFECTSYSTEM_EFFECTS_MOVEEFFECT