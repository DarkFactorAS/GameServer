#ifndef EVILENGINE_COREGAMEENGINE_COREEFFECTSYSTEM_EFFECTS_FADEEFFECT
#define EVILENGINE_COREGAMEENGINE_COREEFFECTSYSTEM_EFFECTS_FADEEFFECT 1
#pragma once

#include "BaseEffect.h"
#include "EvilEngine/CoreLib/CoreMath/CoreMath.h"

class FadeEffect : public BaseEffect
{
public:
  FadeEffect( float runTime, float startAlpha, float endAlpha );
  virtual ~FadeEffect(){};

  virtual void  Start( CoreBaseObject* runObject )                DF_OVERRIDE;
  virtual void  End( CoreBaseObject* runObject )                  DF_OVERRIDE;
  virtual bool  Run( CoreBaseObject* runObject, float deltaTime ) DF_OVERRIDE;

private:

  float                             m_RunTime;
  float                             m_FadeSpeed;
  float                             m_Alpha;
  float                             m_StartAlpha;
  float                             m_EndAlpha;
};

#endif /// EVILENGINE_COREGAMEENGINE_COREEFFECTSYSTEM_EFFECTS_FADEEFFECT