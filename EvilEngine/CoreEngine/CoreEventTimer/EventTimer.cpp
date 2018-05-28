
#include "Precompile.h"
#include "EventTimer.h"
#include "EventTimerManager.h"

EventTimer::EventTimer() :
  m_RunType( 0 ),
  m_EndTime( 0.0f ),
  m_RunTime( 0.0f ),
  m_IsRunning( false )
{
  EventTimerManager::GetInstance()->AddEvent( this );
}

EventTimer::~EventTimer()
{
  EventTimerManager::GetInstance()->RemoveEvent( this );
}

void EventTimer::Start()
{
  m_IsRunning = true;
}
void EventTimer::Stop()
{
  m_RunTime   = 0.0f;
  m_IsRunning = false;
}
void EventTimer::Pause()
{
  m_IsRunning = false;
}

void EventTimer::Run( float deltaTime )
{
  if ( !m_IsRunning )
  {
    return;
  }

  m_RunTime += deltaTime;
  if ( m_RunTime > m_EndTime )
  {
    switch( m_RunType )
    {
      case RunType_OneOff:
        m_Callback();
        Stop();
        break;
      case RunTime_Interval:
        m_Callback();
        m_RunTime -= m_EndTime;
        break;
      default:
        Stop();
        break;
    }
  }
}
