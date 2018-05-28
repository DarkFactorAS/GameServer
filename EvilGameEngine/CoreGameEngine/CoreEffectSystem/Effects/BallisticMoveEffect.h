#ifndef EVILENGINE_COREGAMEENGINE_COREEFFECTSYSTEM_EFFECTS_BALLISTICMOVEEFFECT
#define EVILENGINE_COREGAMEENGINE_COREEFFECTSYSTEM_EFFECTS_BALLISTICMOVEEFFECT 1
#pragma once

#include "EvilEngine/CoreEngine/CoreEffectSystem/Effects/BaseEffect.h"
#include "EvilEngine/CoreLib/CoreMath/CoreMath.h"

class BallisticMoveEffect : public BaseEffect
{
public:
  BallisticMoveEffect( float runTime, const Vector3& fromPosition, const Vector3& toPosition, const Vector3& path );
  virtual ~BallisticMoveEffect(){};

  virtual void    Start( CoreBaseObject* runObject )                DF_OVERRIDE;
  virtual void    End( CoreBaseObject* runObject )                  DF_OVERRIDE;
  virtual bool    Run( CoreBaseObject* runObject, float deltaTime ) DF_OVERRIDE;

  static Vector3  GetZArch( const Vector3& fromPosition, const Vector3& endPositon, float miniumValue = 0.1f ); 

private:

  float                             m_TotalTime;
  float                             m_RunTime;
  Vector3                           m_FromPosition;
  Vector3                           m_ToPosition;
  Vector3                           m_Path;
};

#endif /// EVILENGINE_COREGAMEENGINE_COREEFFECTSYSTEM_EFFECTS_BALLISTICMOVEEFFECT