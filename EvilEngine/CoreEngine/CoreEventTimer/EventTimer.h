#ifndef EVILENGINE_COREENGINE_EVENTTIMER_EVENTTIMER
#define EVILENGINE_COREENGINE_EVENTTIMER_EVENTTIMER 1
#pragma once

class EventTimer
{
public:
  EventTimer();
  ~EventTimer();

  enum RunTime
  {
    RunType_None,
    RunType_OneOff,
    RunTime_Interval,
  };

  void Start();
  void Stop();
  void Pause();
  void Run( float deltaTime );

  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Interval( float tickInterval, CALLBACK_TYPE* obj, void ( OBJ_CLASS::*callback)() )
  {
    m_Callback.Connect( obj, callback );
    m_RunType = RunTime_Interval;
    m_EndTime = tickInterval;
  }

  Signal0<void>     m_Callback;
  int               m_RunType;
  float             m_EndTime;
  float             m_RunTime;
  bool              m_IsRunning;
};

#endif /// EVILENGINE_COREENGINE_EVENTTIMER_EVENTTIMER