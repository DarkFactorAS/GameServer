#ifndef EVILENGINE_COREIO_LOGGER_H
#define EVILENGINE_COREIO_LOGGER_H

#include "../../CoreLib/BasicTypes/Mutex/Mutex.h"
#include "../../CoreIO/File/IOFile.h"

class Log
{
public:

  enum LogLevel
  {
    e_LogDevNull,
    e_LogDebug,
    e_LogInformation,
    e_LogWarning,
    e_LogError,
    e_LogUnittest,
    e_LogFatal,
  };

  Log( const String& fileName );
  ~Log();

  bool LogMsg(LogLevel level, const String& group, const String& message);
  static bool LogStaticMsg(LogLevel level, const String& group, const char*, ... );
  static bool LogStaticMsg(LogLevel level, const String& group, const char* message, va_list pArgs );
  static bool LogStaticMsg(LogLevel level, const String& group, const String& message);

  static Log* s_Instance;

  static void Initialize(const String& logFolder, const String& fileName);
  static void Initialize(const String& fileName);
  static void Shutdown();

  static void SetMinimumLogLevel( LogLevel minimumLogLevel );

#ifdef PLATFORM_WINDOWS
  // Log a message to the Application event log.
  void WriteEventLogEntry(const char* pszMessage, unsigned short wType);
  // Log an error message to the Application event log.
  void WriteErrorLogEntry(const char* pszFunction);
#endif

private:
  void AddTimeStamp();
  void AddLogLevel(LogLevel level);
  void AddGroup(const String& group);

  String    m_Stream;
  IOFile    m_LogFile;
  Mutex     m_LogMutex;

  bool      m_EnforceSpace;
  bool      m_PrintTimeStamp;
  LogLevel  m_MinimumLogLevel;
};

#define LogDebug(group, information)  Log::LogStaticMsg(Log::e_LogDebug, group, information)
#define LogDebugFMT(group, information,...)  Log::LogStaticMsg(Log::e_LogDebug, group, information, __VA_ARGS__ )
#define LogInfo(group, information)  Log::LogStaticMsg(Log::e_LogInformation, group, information)
#define LogInfoFMT(group, information,...)  Log::LogStaticMsg(Log::e_LogInformation, group, information, __VA_ARGS__ )
#define LogWarning(group, information)  Log::LogStaticMsg(Log::e_LogWarning, group, information)
#define LogWarningFMT(group, information,...)  Log::LogStaticMsg(Log::e_LogWarning, group, information, __VA_ARGS__ )
#define LogError(group, information)  Log::LogStaticMsg(Log::e_LogError, group, information)
#define LogErrorFMT(group, information,...)  Log::LogStaticMsg(Log::e_LogError, group, information, __VA_ARGS__ )
#define LogFatal(group, information)  Log::LogStaticMsg(Log::e_LogFatal, group, information)
#define LogFatalFMT(group, information,...)  Log::LogStaticMsg(Log::e_LogFatal, group, information, __VA_ARGS__ )
#define LogUnittest(group, information)  Log::LogStaticMsg(Log::e_LogUnittest, group, information)
#define LogUnittestFMT(group, information,...)  Log::LogStaticMsg(Log::e_LogUnittest, group, information, __VA_ARGS__ )

#endif /// EVILENGINE_COREIO_LOGGER_H