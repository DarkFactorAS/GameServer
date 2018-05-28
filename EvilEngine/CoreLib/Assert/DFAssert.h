#ifndef EVILENGINE_CORELIB_ASSERT_DFASSERT
#define EVILENGINE_CORELIB_ASSERT_DFASSERT 1

#include <stdarg.h>

#if defined PLATFORM_WINDOWS
  class siginfo_t;
#endif

#if defined PLATFORM_LINUX || defined PLATFORM_OSX
  #include <signal.h>
#endif

#include "EvilEngine/CoreLib/Signal/Signal.h"

#define MAX_CALLSTACK_LENGTH 15

#define dfAssert(condition, errorMessage)  dfAssertHandler::Assert(condition, errorMessage )
#define dfAssertFMT(condition, errorMessage, ...)  dfAssertHandler::Assert(condition, errorMessage, __VA_ARGS__ )
#define dfBugreport(condition, errorMessage)  dfAssertHandler::Bugreport(condition, true, errorMessage )
#define dfBugreportNoCallstack(condition, errorMessage)  dfAssertHandler::Bugreport(condition, false, errorMessage )
#define dfBugreportFMT(condition, errorMessage, ...)  dfAssertHandler::Bugreport(condition, true, errorMessage, __VA_ARGS__ )
#define dfBugreportNoCallstackFMT(condition, errorMessage, ...)  dfAssertHandler::Bugreport(condition, false, errorMessage, __VA_ARGS__ )

class dfAssertHandler
{
public:

  static void                 SetupSignalHandlers();
  static void                 Assert(bool condition, const char* errorMessage, ...);
  static void                 Bugreport(bool condition, bool sendCallstack, const char* errorMessage, ...);

  
  static void                 MuteSignal(int /*signalNumber*/, siginfo_t* /*sigInfo*/, void* /*context*/){};
  static void                 HandleSystemAssert(int signalNumber, siginfo_t *sigInfo, void *context);
  static String               DumpCallstack( bool returnString );
  static String               PrintStackTrace();
  static dfAssertHandler*     GetInstance();

  /// SignalAssertMessage : const char* errorMessage, const char* callstack, bool isFatal
  static Signal3< void, const char*, const char*, bool > SignalAssertMessage;

  /// SignalCrash
  static Signal0< void > SignalCrash;

  // If the program is started from a debugger
  static bool                 AmIBeingDebugged();

  //void                        SignalHandler(int signalNumber);
  static void                 cause_segfault();

private:

  dfAssertHandler();

  void                        AssertWithMessage(  bool condition, bool useCallstack, const char* errorMessage, va_list pArgs, bool isFatal );

  static void                 SetBlockAllSignals(bool block);
  static void                 SetBlockSignal(int signal, bool block);
};

//#define dbBugReport(condition, errorMessage)      dfAssert(condition, information)
//#define dbBugReport(condition, errorMessage,...)  dfAssert(condition, errorMessage, __VA_ARGS__ )

// Object deletion macro
#define SAFE_DELETE(x) \
{ \
  delete x; \
  x = NULL; \
}

template<class T, class U> T safe_cast(U p)
{
#if defined DEBUG && !defined PLATFORM_IOS
  dfAssert(dynamic_cast<T> (p) == static_cast<T> (p), ("safe_cast failed!"));
#endif
  return static_cast<T> (p);
}

#endif /// EVILENGINE_CORELIB_ASSERT_DFASSERT
