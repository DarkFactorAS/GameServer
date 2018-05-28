#ifndef EVILENGINE_COREBOARDGAME_GAMELOGIC_EFFECTS_SETGRIDPOSITIONEFFECT
#define EVILENGINE_COREBOARDGAME_GAMELOGIC_EFFECTS_SETGRIDPOSITIONEFFECT 1
#pragma once

#include "EvilEngine/CoreEngine/CoreEffectSystem/Effects/BaseEffect.h"
#include "EvilEngine/CoreLib/CoreMath/CoreMath.h"

class SetGridpositionEffect : public BaseEffect
{
public:
  SetGridpositionEffect( float runTime, const Vector2& gridPosition );
  virtual ~SetGridpositionEffect(){};

  virtual void  Start( CoreBaseObject* runObject )                DF_OVERRIDE;
  virtual void  End( CoreBaseObject* runObject )                  DF_OVERRIDE;
  virtual bool  Run( CoreBaseObject* runObject, float deltaTime ) DF_OVERRIDE;

private:

  Vector2                           m_GridPosition;
};

#endif /// EVILENGINE_COREBOARDGAME_GAMELOGIC_EFFECTS_SETGRIDPOSITIONEFFECT