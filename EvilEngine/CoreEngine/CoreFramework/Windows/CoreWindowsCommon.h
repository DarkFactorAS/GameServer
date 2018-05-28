#ifndef EVILENGINE_COREENGINE_COREWINDOWSCOMMON
#define EVILENGINE_COREENGINE_COREWINDOWSCOMMON 1
#pragma once

#if defined PLATFORM_WINDOWS

#include "windows.h"
#include "EvilEngine/CoreLib/Signal/Signal.h"
#include "EvilEngine/CoreEngine/CoreEngine/CoreEngine.h"

BOOL WINAPI SetConsoleIcon(HICON hIcon);

class CoreWindowsCommon
{
public:
  CoreWindowsCommon( const char* winName, const char* winDisplayName);

  int           Run();
  bool          CreateAsService(int argc, char* argv[]);
  bool          CreateAsConsoleApp(int icon1, int icon2);
  void          SetEngine(CoreEngine* engine) { m_CoreEngine = engine; };

  bool          CreateConsole( int iconId );
  void          SetConsoleIcon( int iconId );
  void          Run( float deltaTime );
  void          SetFramerate( int frameratre );
  static bool   s_RunEngine;

  const char*   m_WinShortName;
  const char*   m_WinDisplayName;
  
  static Signal0<void>  s_SignalShutdown;

  CoreEngine*   m_CoreEngine;

  const char*   GetExePath();
  static char   s_DefaultPath[MAX_PATH];
  const char*   GetDefaultPath(){ return s_DefaultPath; }

private:

  int           RunService();
  int           RunApplication();

  bool          m_RunAsService;
  bool          m_ShowConsoleWindow;

  int           m_Icon;

  uint32        m_Frames;
  uint32        m_LastUpdateFrame;
  float         m_DeltaTime;

  unsigned long m_StartTime;
  long          m_LastSleep;
  unsigned long m_SleepTime;
  int           m_WantedFPS;
  int           m_LogCounter;
};

#endif /// PLATFORM_WINDOWS
#endif /// EVILENGINE_COREENGINE_COREWINDOWSCOMMON