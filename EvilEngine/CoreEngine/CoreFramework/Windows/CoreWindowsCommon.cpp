
#include "PreCompile.h"

#if defined PLATFORM_WINDOWS

#include "CoreWindowsCommon.h"
#include "EvilEngine/CoreLib/Assert/DFCallstack.h"
#include "EvilEngine/CoreLib/Utils/CoreUtils.h"
#include "EvilEngine/CoreIO/Logger/Logger.h"

#include <windows.h>
#include <tchar.h>
#include <signal.h>
#include <algorithm>

#include "EvilEngine/CoreEngine/CoreFramework/WindowsService/ServiceInstaller.hpp"
#include "EvilEngine/CoreEngine/CoreFramework/WindowsService/CoreWindowsService.h"
#include "EvilEngine/CoreEngine/CoreEventLogger/CoreEventLogger.h"

#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

// Service start options. 
#define SERVICE_START_TYPE       SERVICE_DEMAND_START 
// List of service dependencies - "dep1\0dep2\0\0" 
#define SERVICE_DEPENDENCIES     L"" 
// The name of the account under which the service should run 
#define SERVICE_ACCOUNT          L"NT AUTHORITY\\LocalService" 
// The password to the service account name 
#define SERVICE_PASSWORD         NULL 

bool CoreWindowsCommon::s_RunEngine = true;
Signal0< void > CoreWindowsCommon::s_SignalShutdown;

static long           s_LastTime = 0;
char CoreWindowsCommon::s_DefaultPath[MAX_PATH];

void shutdown(int /*sig*/)
{
  LogInfo("Main","User aborted. Closing down...");
  CoreWindowsCommon::s_RunEngine = false;
  CoreWindowsCommon::s_SignalShutdown();
}

BOOL WINAPI ConsoleHandler(DWORD dwType)
{
  switch(dwType) 
  {
  case CTRL_CLOSE_EVENT:
  case CTRL_LOGOFF_EVENT:
  case CTRL_SHUTDOWN_EVENT:
    LogInfo("Main","User closed console window..killing server");
    CoreWindowsCommon::s_RunEngine = false;
    boost::this_thread::sleep(boost::posix_time::seconds(60));
    //Returning would make the process exit!
    //We just make the handler sleep until the main thread exits,
    //or until the maximum execution time for this handler is reached.
    return TRUE;
  default:
    break;
  }
  return FALSE;
}

CoreWindowsCommon::CoreWindowsCommon( const char* winName, const char* winDisplayName) :
  m_WinShortName( winName ),
  m_WinDisplayName( winDisplayName )
{
  m_Frames          = 0;
  m_StartTime       = 0;
  m_DeltaTime       = 0;
  m_LastUpdateFrame = 0;
  m_LogCounter      = 0;
  m_RunAsService    = false;
  m_ShowConsoleWindow = false;
  SetFramerate( 30 );
}

int CoreWindowsCommon::Run()
{
  if (m_RunAsService)
  {
    return RunService();
  }
  else
  {
    return RunApplication();
  }
}

const char* CoreWindowsCommon::GetExePath()
{
  //char szPath[MAX_PATH];
  char* szPath = (char*) malloc(MAX_PATH);
  if (GetModuleFileNameA(NULL, szPath, MAX_PATH) == 0)
  {
    return "";
  }

  std::string strPath = std::string(szPath);
  std::string::size_type pos = strPath.find_last_of("\\/");
  std::string subStr = strPath.substr(0, pos) + "\\\0";

  strcpy_s(szPath, MAX_PATH, subStr.c_str());
  return szPath;
}

bool CoreWindowsCommon::CreateAsService(int argc, char* argv[])
{
  if ((argc > 1) && ((*argv[1] == '-' || (*argv[1] == '/'))))
  {
    wchar_t* serviceName = CoreUtils::ConvertCharArrayToLPCWSTR(m_WinShortName);

    if (strcmp("install", argv[1] + 1) == 0)
    {
      wchar_t* serviceDisplayName = CoreUtils::ConvertCharArrayToLPCWSTR(m_WinDisplayName);

      const char* configPath = GetExePath();
      if (argc > 2)
      {
        configPath = argv[2];
      }

      wchar_t* wConfigPath = CoreUtils::ConvertCharArrayToLPCWSTR(configPath);

      wchar_t* wServiceName = serviceName;
      if (argc > 2)
      {
        wServiceName = CoreUtils::ConvertCharArrayToLPCWSTR(argv[2]);
      }

      // Always remove service first
      ServiceInstaller::UninstallService(wServiceName);

      // Install the service when the command is  
      // "-install" or "/install". 
      ServiceInstaller::InstallService(
        wServiceName,                // Name of service 
        serviceDisplayName,         // Name to display 
        wConfigPath,                 // Path of the config file
        SERVICE_START_TYPE,         // Service start type 
        SERVICE_DEPENDENCIES,       // Dependencies 
        SERVICE_ACCOUNT,            // Service running account 
        SERVICE_PASSWORD            // Password of the account 
      );
    }
    else if (strcmp("remove", argv[1] + 1) == 0)
    {
      // Uninstall the service when the command is  
      // "-remove" or "/remove". 

      wchar_t* wServiceName = serviceName;
      if (argc > 2)
      {
        wServiceName = CoreUtils::ConvertCharArrayToLPCWSTR(argv[2]);
      }

      ServiceInstaller::UninstallService(wServiceName);
    }
    else if (strcmp("path", argv[1] + 1) == 0)
    {
      if (argc > 2)
      {
        strcpy_s(s_DefaultPath, MAX_PATH, argv[2]);
        m_RunAsService = true;
        return true;
      }
      else
      {
        LogFatal("WindowsCommon", "No argument for path");
      }
    }
    else if (argv[1][0] == '-')
    {
      printf("failed to install/remove service\n");
      printf("-Install [servicename] : Install service. If blank name of .exe is servicenmae");
      printf("-Remove [servicename] : Remove service. If blank name of .exe is servicename");
      printf("-Path <path> : Run folder for service (to find config files)");
    }
    return false;
  }

  m_RunAsService = true;
  return true;
}

int CoreWindowsCommon::RunService()
{
  printf("Parameters:\n");
  printf(" -install  to install the service.\n");
  printf(" -remove   to remove the service.\n");

  wchar_t* serviceName = CoreUtils::ConvertCharArrayToLPCWSTR(m_WinShortName);
  CoreEventLogger::WriteEventLogEntry(serviceName, L"RunService1", EVENTLOG_ERROR_TYPE);

  CoreWindowsService service(serviceName, m_CoreEngine);
  if (!CoreWindowsService::Run(service))
  {
    printf("Service failed to run w/err 0x%08lx\n", GetLastError());
    CoreEventLogger::WriteEventLogEntry(serviceName, L"RunService2", EVENTLOG_ERROR_TYPE);
    return -1;
  }

  return 0;
}

bool CoreWindowsCommon::CreateAsConsoleApp(int icon1, int icon2)
{
  if (icon1 != 0)
  {
    m_ShowConsoleWindow = CreateConsole(icon1);
    m_Icon = icon1;
  }
  if (icon2 != 0)
  {
    m_Icon = icon2;
  }
  return true;
}

int CoreWindowsCommon::RunApplication()
{
  if (m_CoreEngine == NULL || !m_CoreEngine->Init())
  {
    LogFatal("Main", "Could not start gameserver");
    return false;
  }

  if (m_ShowConsoleWindow)
  {
    SetConsoleIcon(m_Icon);
  }

  // Just halt here while the server is running
  while( CoreWindowsCommon::s_RunEngine )
  {
    long nowTime    = CoreUtils::GetTime();
    if ( s_LastTime == 0 )
    {
      s_LastTime = nowTime;
    }
    long deltaTime  = nowTime - s_LastTime;
    float fDelta    = deltaTime / 1000.0f;

    if ( fDelta >= 0.0f )
    {
      CoreWindowsCommon::s_RunEngine = m_CoreEngine->FrameProcess( fDelta );
      Run( fDelta );
    }
    s_LastTime      = nowTime;
  }

  return 0;
}

void CoreWindowsCommon::SetFramerate( int framerate )
{
  m_WantedFPS       = framerate;
  if (m_WantedFPS > 0)
  {
    //m_SleepTime = 1000000 / m_WantedFPS;
    m_SleepTime = 1000 / m_WantedFPS;
  }
  else
  {
    m_SleepTime = 0;
  }
}

bool CoreWindowsCommon::CreateConsole( int iconId )
{
  //
  // Change the console to be bigger with a large scrollback history
  //
  AllocConsole();
  SMALL_RECT r;
  COORD      c;
  HANDLE hConOut = GetStdHandle( STD_OUTPUT_HANDLE );
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  BOOL hasConsole = GetConsoleScreenBufferInfo( hConOut, &csbi );
  if ( hasConsole )
  {
    c.X = 1000;
    c.Y = 9999;
    SetConsoleScreenBufferSize( hConOut, c );
    r.Left   = 0;
    r.Top    = 0;
    r.Right  = 180;
    r.Bottom = 40;
    SetConsoleWindowInfo( hConOut, TRUE, &r );
    SetConsoleTitle( CoreUtils::ConvertCharArrayToLPCWSTR( m_WinDisplayName ) );

    // Grab An Instance For Our Window
    //HINSTANCE hInstance   = GetModuleHandle(NULL);
    //HICON hIcon1          = LoadIcon(hInstance, MAKEINTRESOURCE( iconId ));
    //SetConsoleIcon( hIcon1 );
    SetConsoleIcon( iconId );

    // Windows Console handler. To handle if the user clicks the close button
    SetConsoleCtrlHandler(ConsoleHandler, TRUE);

    // Hook up the callback for the console (closing/killing it)
    if (signal(SIGINT, shutdown) == SIG_ERR || signal(SIGTERM, shutdown) == SIG_ERR)
    {
      LogError("Main", "Problem setting up terminate signal handler") ;
    }
    else
    {
      LogInfo("Main", "To quit press Ctrl-C") ;
      LogInfo("Main", "---------------------------------------------------") ;
    }
    return true;
  }
  return false;
}

BOOL WINAPI SetInternalConsoleIcon(HICON hIcon) 
{
  typedef BOOL (WINAPI *PSetConsoleIcon)(HICON);
  static PSetConsoleIcon pSetConsoleIcon = NULL;
  if(pSetConsoleIcon == NULL)
  {
    pSetConsoleIcon = (PSetConsoleIcon)::GetProcAddress(::GetModuleHandle(_T("kernel32")), "SetConsoleIcon");
  }
  if(pSetConsoleIcon == NULL)
  {
    return FALSE;
  }
  return pSetConsoleIcon(hIcon);
}

void CoreWindowsCommon::SetConsoleIcon( int iconId )
{
  // Grab An Instance For Our Window
  HINSTANCE hInstance   = GetModuleHandle(NULL);
  HICON hIcon1          = LoadIcon(hInstance, MAKEINTRESOURCE( iconId ));
  SetInternalConsoleIcon( hIcon1 );
}

#endif /// PLATFORM_WINDOWS

#if defined PLATFORM_WINDOWS
void CoreWindowsCommon::Run( float deltaTime )
{
  if (m_WantedFPS == 0)
  {
    return;
  }

  m_Frames++;
  m_DeltaTime += deltaTime;

  if ( m_DeltaTime >= 1.0f ) 
  {
    int numFrames = m_Frames - m_LastUpdateFrame;
    int avgFPS  = (int)(numFrames / m_DeltaTime);

    m_LogCounter++;

    if ( m_LogCounter % 5 == 0 && ( avgFPS < 2.0f || avgFPS > 100.0f ) )
    {
      LogInfoFMT( "CoreWindowCommon", "[%d] FPS:%d frames:%d, DeltaTime:%0.2f, SleepTime:%ul, LastSleep:%ul", m_Frames, avgFPS, numFrames, m_DeltaTime, m_SleepTime, m_LastSleep );
      m_LogCounter = 0;
    }

    m_DeltaTime -= 1.0f;
    m_LastUpdateFrame = m_Frames;
  }

  unsigned long lastStart = m_StartTime;
  m_StartTime    = CoreUtils::GetTime();
  if ( lastStart != 0 )
  {
    // Make the thread sleep to keep a steady FPS
    long frameTime = m_SleepTime - min( m_StartTime - lastStart, m_SleepTime );

    if ( frameTime > 0 )
    {
      m_LastSleep = frameTime;
      boost::this_thread::sleep(boost::posix_time::milliseconds(frameTime)); 
      //boost::this_thread::sleep_for(boost::chrono::milliseconds(frameTime));
    }
    else
    {
      m_LastSleep = 0;
      boost::this_thread::sleep(boost::posix_time::milliseconds(1));
    }
  }
}
#endif /// PLATFORM_WINDOWS
