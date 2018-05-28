#ifndef EVILENGINE_COREENGINE_COREEFFECTSYSTEM_EFFECTS_BASEEFFECT
#define EVILENGINE_COREENGINE_COREEFFECTSYSTEM_EFFECTS_BASEEFFECT 1
#pragma once

#include "EvilEngine/CoreEngine/CoreBaseObject/CoreBaseObject.h"

class BaseEffect
{
public:
  BaseEffect( float runTime );
  virtual ~BaseEffect(){};

  uint32        GetId() const { return m_Id; }
  bool          Started();
  void          SetStarted( bool isStarted ){ m_Started = isStarted; }

  virtual void  Start( CoreBaseObject* /*runObject*/ );
  virtual void  End( CoreBaseObject* /*runObject*/ ){};
  virtual bool  Run( CoreBaseObject* runObject, float deltaTime );

private:

  uint32                            m_Id;
  float                             m_RunTime;
  float                             m_TotalRunTime;
  bool                              m_Started;
};

#endif /// EVILENGINE_COREENGINE_COREEFFECTSYSTEM_EFFECTS_BASEEFFECT