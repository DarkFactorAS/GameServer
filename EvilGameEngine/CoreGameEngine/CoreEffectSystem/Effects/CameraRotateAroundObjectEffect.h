#ifndef EVILENGINE_COREGAMEENGINE_COREEFFECTSYSTEM_EFFECTS_CAMERAROTATEAROUNDOBJECTEFFECT
#define EVILENGINE_COREGAMEENGINE_COREEFFECTSYSTEM_EFFECTS_CAMERAROTATEAROUNDOBJECTEFFECT 1
#pragma once

#include "EvilEngine/CoreEngine/CoreEffectSystem/Effects/BaseEffect.h"
#include "EvilEngine/CoreLib/CoreMath/CoreMath.h"

class CameraRotateAroundObjectEffect : public BaseEffect
{
public:
  CameraRotateAroundObjectEffect( float runTime, float speed, bool loop );
  virtual ~CameraRotateAroundObjectEffect(){};

  virtual void  Start( CoreBaseObject* runObject )                DF_OVERRIDE;
  virtual void  End( CoreBaseObject* runObject )                  DF_OVERRIDE;
  virtual bool  Run( CoreBaseObject* runObject, float deltaTime ) DF_OVERRIDE;

private:

  float       m_Speed;
  bool        m_Loop;
};

#endif /// EVILENGINE_COREGAMEENGINE_COREEFFECTSYSTEM_EFFECTS_CAMERAROTATEAROUNDOBJECTEFFECT