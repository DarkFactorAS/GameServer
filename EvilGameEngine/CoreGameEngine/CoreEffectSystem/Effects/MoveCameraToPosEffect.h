#ifndef EVILENGINE_COREGAMEENGINE_COREEFFECTSYSTEM_EFFECTS_MOVECAMERATOPOSEFFECT
#define EVILENGINE_COREGAMEENGINE_COREEFFECTSYSTEM_EFFECTS_MOVECAMERATOPOSEFFECT 1
#pragma once

#include "EvilEngine/CoreEngine/CoreEffectSystem/Effects/BaseEffect.h"
#include "EvilEngine/CoreLib/CoreMath/CoreMath.h"

class MoveCameraToPosEffect : public BaseEffect
{
public:
  MoveCameraToPosEffect( float runTime, const Vector3& toPosition );
  virtual ~MoveCameraToPosEffect(){};

  virtual void  Start( CoreBaseObject* runObject )                DF_OVERRIDE;
  virtual void  End( CoreBaseObject* runObject )                  DF_OVERRIDE;
  virtual bool  Run( CoreBaseObject* runObject, float deltaTime ) DF_OVERRIDE;

private:

  Vector3     m_StartPosition;
  Vector3     m_EndPosition;
  float       m_RunTime;
  float       m_TotalTime;
};

#endif /// EVILENGINE_COREGAMEENGINE_COREEFFECTSYSTEM_EFFECTS_MOVECAMERATOPOSEFFECT