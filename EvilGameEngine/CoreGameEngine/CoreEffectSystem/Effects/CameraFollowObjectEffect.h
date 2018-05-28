#ifndef EVILENGINE_COREGAMEENGINE_COREEFFECTSYSTEM_EFFECTS_CAMERAFOLLOWOBJECTEFFECT
#define EVILENGINE_COREGAMEENGINE_COREEFFECTSYSTEM_EFFECTS_CAMERAFOLLOWOBJECTEFFECT 1
#pragma once

#include "EvilEngine/CoreEngine/CoreEffectSystem/Effects/BaseEffect.h"
#include "EvilEngine/CoreLib/CoreMath/CoreMath.h"

class CameraFollowObjectEffect : public BaseEffect
{
public:
  CameraFollowObjectEffect( float runTime, bool onlyXY );
  virtual ~CameraFollowObjectEffect(){};

  virtual void  Start( CoreBaseObject* runObject )                DF_OVERRIDE;
  virtual void  End( CoreBaseObject* runObject )                  DF_OVERRIDE;
  virtual bool  Run( CoreBaseObject* runObject, float deltaTime ) DF_OVERRIDE;

private:

  bool        m_OnlyXY;
  Vector3     m_CurrentPos;
};

#endif /// EVILENGINE_COREGAMEENGINE_COREEFFECTSYSTEM_EFFECTS_CAMERAFOLLOWOBJECTEFFECT