
#include "Precompile.h"
#include "EventTimerManager.h"

#define MAX_EVENTS 100

EventTimerManager::EventTimerManager() :
  m_EventList( MAX_EVENTS, false )
{
}

EventTimerManager* EventTimerManager::GetInstance()
{
  static EventTimerManager* s_Instance = NULL;
  if ( s_Instance == NULL )
  {
    s_Instance = new EventTimerManager();
  }
  return s_Instance;
}

void EventTimerManager::AddEvent( EventTimer* coreEvent )
{
  m_EventList.Add( coreEvent );
}

void EventTimerManager::RemoveEvent( EventTimer* coreEvent )
{
  m_EventList.Remove( coreEvent );
}

void EventTimerManager::FrameProcess( float deltaTime )
{
  uint32 numElements = m_EventList.GetSize();
  for ( uint32 index = 0; index < numElements; index++ )
  {
    EventTimer* timer = m_EventList.Get( index );
    if ( timer != NULL )
    {
      timer->Run( deltaTime );
    }
  }
}

