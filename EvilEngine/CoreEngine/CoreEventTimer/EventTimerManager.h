#ifndef EVILENGINE_COREENGINE_EventTimerManager_COREEVENTHANDLDER
#define EVILENGINE_COREENGINE_EventTimerManager_COREEVENTHANDLDER 1
#pragma once

#include "EventTimer.h"
#include "CoreLib/BasicTypes/FixedMaps/FixedSizeVector.h"

class EventTimerManager
{
public:
  static EventTimerManager*       GetInstance();

  void                            AddEvent( EventTimer* coreEvent );
  void                            RemoveEvent( EventTimer* coreEvent );
  void                            FrameProcess( float deltaTime );

private:
  EventTimerManager();

  FixedSizeVector< EventTimer* >  m_EventList;
};

#endif /// EVILENGINE_COREENGINE_EventTimerManager_COREEVENTHANDLDER