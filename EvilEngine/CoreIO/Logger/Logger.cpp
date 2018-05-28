/*************************************************************************************
* Project     : Evil Engine 2012
* File        : Log
* Description : Will log debug information to file and the debugger output window.
*               
* Author      : Thor Richard Hansen
 *************************************************************************************/
#include "Precompile.h"
#include "Logger.h"
#include <string.h>
#include "EvilEngine/CoreLib/Assert/DFAssert.h"
#include "EvilEngine/CoreLib/Utils/CoreUtils.h"

#define  LOG_TAG    "DarkFactor"

#ifdef PLATFORM_WINDOWS
  #include <Windows.h>
  #include <strsafe.h>
  #include "boost/date_time/posix_time/posix_time_types.hpp"
#elif ANDROID
  #define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
  #define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
  #define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
  #define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
  #define  LOGF(...)  __android_log_print(ANDROID_LOG_FATAL,LOG_TAG,__VA_ARGS__)
#else
  #include "boost/date_time/posix_time/posix_time_types.hpp"
#endif

Log* Log::s_Instance = NULL;

Log::Log( const String& fileName )
{
  m_EnforceSpace    = true;
  m_MinimumLogLevel = e_LogInformation;
  m_PrintTimeStamp  = true;

  m_LogFile.OpenWrite( fileName );
}

Log::~Log()
{
  m_LogFile.Close();
}

void Log::Initialize( const String& /*logFolder*/, const String& fileName )
{
  Shutdown();

  if ( s_Instance == NULL )
  {
    s_Instance = new Log( fileName );
  }

  //String path = JoinPath(logFolder, fileName);
  //s_Settings->m_LogFile.Open( fileName );
}

void Log::Initialize( const String& fileName )
{
  Shutdown();

  if ( s_Instance == NULL )
  {
    s_Instance = new Log( fileName );
  }

  //String path = JoinPath(logFolder, fileName);
  //s_Settings->m_LogFile.Open( fileName );
}

void Log::SetMinimumLogLevel(LogLevel minimumLogLevel)
{ 
  if ( s_Instance != NULL )
  {
    s_Instance->m_MinimumLogLevel = minimumLogLevel;
  }
}

void Log::Shutdown()
{
  if ( s_Instance != NULL )
  {
    delete s_Instance;
    s_Instance = NULL;
  }
}

bool Log::LogStaticMsg(LogLevel level, const String& group, const char* message, ... )
{
  bool didSucceed = false;
  if ( s_Instance != NULL )
  {
    va_list sArgList;
    va_start( sArgList, message );
    didSucceed = LogStaticMsg( level, group, message, sArgList );
    va_end( sArgList );
  }
  return didSucceed;
}

bool Log::LogStaticMsg(LogLevel level, const String& group, const char* message, va_list pArgs )
{
  bool didSucceed = false;
  if ( s_Instance != NULL )
  {
    String str = String::ForwardFormatString( message, pArgs );
    didSucceed = s_Instance->LogMsg( level, group, str.c_str());
  }
  return didSucceed;
}

bool Log::LogStaticMsg(LogLevel level, const String& group, const String& message)
{
  bool didSucceed = false;
  if ( s_Instance != NULL )
  {
    didSucceed = s_Instance->LogMsg( level, group, message );
  }
  return didSucceed;
}

bool Log::LogMsg(LogLevel level, const String& group, const String& message )
{
  if ( level < m_MinimumLogLevel )
  {
    return true;
  }

  String levelStr;
  switch (level)
  {
    case e_LogDebug:        levelStr = "[DEBUG   ] ";   break;
    case e_LogInformation:  levelStr = "[INFO    ] ";   break;
    case e_LogWarning:      levelStr = "[WARNING ] ";   break;
    case e_LogError:        levelStr = "[ERROR   ] ";   break;
    case e_LogFatal:        levelStr = "[FATAL   ] ";   break;
    case e_LogUnittest:     levelStr = "[UNITTEST] ";   break;
    default:                levelStr = "[UNKNOWN ] ";   break;
  }

#if defined PLATFORM_ANDROID

  String outputStr = String::FormatString("%s [%s] %s", levelStr.c_str(), group.c_str(), message.c_str() );
  switch( level )
  {
    case e_LogDebug:
      LOGD( outputStr.c_str() );
      break;
    case e_LogInformation:
      LOGI( outputStr.c_str() );
      break;
    case e_LogWarning:
      LOGW( outputStr.c_str() );
      break;
    case e_LogError:
      LOGE( outputStr.c_str() );
      break;
    case e_LogFatal:
      LOGF( outputStr.c_str() );
      break;
    default:
      LOGI( outputStr.c_str() );
      break;
  }
#else

  boost::posix_time::ptime localTime = boost::posix_time::microsec_clock::local_time();   // USe 'universal_time' for GMT instead of 'local_time'

  uint32 milliseconds=localTime.time_of_day().total_milliseconds()%1000;
  uint32 seconds     =localTime.time_of_day().seconds();
  uint32 minutes     =localTime.time_of_day().minutes();
  uint32 hours       =localTime.time_of_day().hours();
  uint32 day         =localTime.date().day();
  uint32 month       =localTime.date().month();
  uint32 year        =localTime.date().year();
  String timeString  = String::FormatString( "[%u-%02u-%02u %02u:%02u:%02u.%04u] ", year,month,day,hours,minutes,seconds,milliseconds );

  #if defined PLATFORM_WINDOWS

    String outputStr = String::FormatString("%s%s[%s] %s\n", timeString.c_str(), levelStr.c_str(), group.c_str(), message.c_str() );
    printf( outputStr.c_str() );
    printf( "\r");
    OutputDebugStringA( outputStr.c_str() );

    // Log to event viewer
    switch (level)
    {
      case e_LogWarning:
        WriteEventLogEntry(outputStr.c_str(), EVENTLOG_INFORMATION_TYPE);
        break;
      case e_LogError:
      case e_LogFatal:
        WriteEventLogEntry(outputStr.c_str(), EVENTLOG_ERROR_TYPE);
        break;
      default:
        break;
    }


  #elif defined PLATFORM_IOS

    String outputStr = String::FormatString("%s%s[%s] %s\n", timeString.c_str(), levelStr.c_str(), group.c_str(), message.c_str() );
    printf( "%s", outputStr.c_str() );

  #else
  
    String outputStr = String::FormatString("%s%s[%s] %s \n", timeString.c_str(), levelStr.c_str(), group.c_str(), message.c_str() );
    printf( "%s", outputStr.c_str() );

  #endif

#endif

  // Log to file if we can
  bool wasLogged = false;
  if ( m_LogFile.IsOpen() )
  {
    m_LogMutex.Aquire();
    m_Stream << outputStr;
    m_LogFile.WriteString(m_Stream);
#ifdef PLATFORM_WINDOWS
    m_LogFile.WriteString("\r\n");
#else
    m_LogFile.WriteString("\n");
#endif
    m_LogFile.Flush();
    m_Stream.Clear();
    m_LogMutex.Release();
    wasLogged = true;
  }

  // Log to event log?

  // Stop in debugger on fatal logs
#ifdef PLATFORM_WINDOWS
  if (level == e_LogFatal && dfAssertHandler::AmIBeingDebugged())
  {
    ::DebugBreak();
  }
#endif

  return wasLogged;
}

void Log::AddTimeStamp()
{
  //QDateTime cNow = QDateTime::currentDateTime();
  //QString timeNow = cNow.toString("[MM.dd.yyyy hh:mm:ss] ");
  //m_Stream << timeNow;
}

void Log::AddLogLevel(LogLevel level)
{
  switch(level)
  {
    case e_LogDevNull:
    {
      break;
    }

    case e_LogDebug:
    {
      m_Stream << "[Debug] ";
      break;
    }
    case e_LogInformation:
    {
      m_Stream << "[Information] ";
      break;
    }
    case e_LogWarning:
    {
      m_Stream << "[Warning] ";
      break;
    }
    case e_LogError:
    {
      m_Stream << "[Error] ";
      break;
    }
    case e_LogFatal:
    {
      m_Stream << "[Fatal] ";
      break;
    }
    case e_LogUnittest:
    {
      m_Stream << "[UNITTEST] ";
      break;
    }
  }
}

void Log::AddGroup(const String& group)
{
  m_Stream << "[" << group << "] ";
}

//
//   FUNCTION: CoreWindowsService::WriteEventLogEntry(PWSTR, WORD)
//
//   PURPOSE: Log a message to the Application event log.
//
//   PARAMETERS:
//   * pszMessage - string message to be logged.
//   * wType - the type of event to be logged. The parameter can be one of 
//     the following values.
//
//     EVENTLOG_SUCCESS
//     EVENTLOG_AUDIT_FAILURE
//     EVENTLOG_AUDIT_SUCCESS
//     EVENTLOG_ERROR_TYPE
//     EVENTLOG_INFORMATION_TYPE
//     EVENTLOG_WARNING_TYPE
//
#ifdef PLATFORM_WINDOWS
void Log::WriteEventLogEntry(const char* pszMessage, unsigned short wType)
{
  HANDLE hEventSource = NULL;
  const char* lpszStrings[2] = { NULL, NULL };

  hEventSource = RegisterEventSource(NULL, LOG_TAG);
  if (hEventSource)
  {
    lpszStrings[0] = LOG_TAG;
    lpszStrings[1] = pszMessage;

    ReportEvent(hEventSource,  // Event log handle
      wType,                 // Event type
      0,                     // Event category
      0,                     // Event identifier
      NULL,                  // No security identifier
      2,                     // Size of lpszStrings array
      0,                     // No binary data
      lpszStrings,           // Array of strings
      NULL                   // No binary data
    );

    DeregisterEventSource(hEventSource);
  }
}
#endif

//
//   FUNCTION: CoreWindowsService::WriteErrorLogEntry(PWSTR, DWORD)
//
//   PURPOSE: Log an error message to the Application event log.
//
//   PARAMETERS:
//   * pszFunction - the function that gives the error
//   * dwError - the error code
//
#ifdef PLATFORM_WINDOWS
void Log::WriteErrorLogEntry(const char* pszFunction)
{
  //unsigned long dwError = GetLastError();

  //wchar_t szMessage[260];
  //StringCchPrintf(szMessage, ARRAYSIZE(szMessage), L"%s failed w/err 0x%08lx", pszFunction, dwError);
  WriteEventLogEntry(pszFunction, EVENTLOG_ERROR_TYPE);
}
#endif