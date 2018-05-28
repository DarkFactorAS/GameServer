
#include "Precompile.h"

#if defined PLATFORM_WINDOWS

#include "Precompile.h"
#include "DFAssert.h"
#include "EvilEngine/CoreIO/Logger/Logger.h"
#include <string.h>
#include "DFCallstack.h"
#include "EvilEngine/CoreLib/BasicTypes/String/StringList.h"

#include "Windows.h"

#include <signal.h>
#include <stdlib.h>

void SignalHandler(int signalNumber) 
{
  switch( signalNumber )
  {
    case SIGILL:
      dfAssert( false, "SignalHandler:SIGILL" );
      break;
    case SIGSEGV:
      dfAssert( false, "SignalHandler:SIGSEGV" );
      break;
    default:
      dfAssert( false, "SignalHandler:Unknown" );
      break;
  }
}

LONG WINAPI windows_exception_handler(EXCEPTION_POINTERS * ExceptionInfo)
{
  switch(ExceptionInfo->ExceptionRecord->ExceptionCode)
  {
    case EXCEPTION_ACCESS_VIOLATION:
    case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
    case EXCEPTION_BREAKPOINT:
    case EXCEPTION_DATATYPE_MISALIGNMENT:
    case EXCEPTION_FLT_DENORMAL_OPERAND:
    case EXCEPTION_FLT_DIVIDE_BY_ZERO:
    case EXCEPTION_FLT_INEXACT_RESULT:
    case EXCEPTION_FLT_INVALID_OPERATION:
    case EXCEPTION_FLT_OVERFLOW:
    case EXCEPTION_FLT_STACK_CHECK:
    case EXCEPTION_FLT_UNDERFLOW:
    case EXCEPTION_ILLEGAL_INSTRUCTION:
    case EXCEPTION_IN_PAGE_ERROR:
    case EXCEPTION_INT_DIVIDE_BY_ZERO:
    case EXCEPTION_INT_OVERFLOW:
    case EXCEPTION_INVALID_DISPOSITION:
    case EXCEPTION_NONCONTINUABLE_EXCEPTION:
    case EXCEPTION_PRIV_INSTRUCTION:
    case EXCEPTION_SINGLE_STEP:
    case EXCEPTION_STACK_OVERFLOW:
      dfAssert( false, "windows_exception_handler" );
      break;
    default:
      break;
  }
  return EXCEPTION_EXECUTE_HANDLER;
}

void dfAssertHandler::SetupSignalHandlers() 
{
  signal(SIGILL, SignalHandler );
  signal(SIGFPE, SignalHandler );
  signal(SIGSEGV, SignalHandler );
  signal(SIGABRT, SignalHandler );
  signal(SIGINT, SignalHandler );
  signal(SIGTERM, SignalHandler );

  SetUnhandledExceptionFilter(windows_exception_handler);

  // Uncomment to test the handlers
  //dfAssertHandler::cause_segfault();
}

void dfAssertHandler::AssertWithMessage( bool condition, bool useCallstack, const char* errorMessage, va_list pArgs, bool isFatal )
{
  if ( !condition )
  {
    //int maxLen  = 2048;
    int len       = 0;

    char pzBuffer[ 4096 ];
    memset( pzBuffer, 0, 4096 );

    len = vsnprintf_s( pzBuffer, sizeof( pzBuffer ), sizeof( pzBuffer ) / sizeof( pzBuffer[0] ), errorMessage, pArgs );

    String debugString = "ASSERT";
    if (!isFatal)
    {
      debugString = "BUGREPORT";
    }

    LogError(debugString, pzBuffer );

    // Print the callstack
    if ( useCallstack )
    {
      String callstack = PrintStackTrace();
      StringList strList = StringList::Split(callstack, "\n");
      LogError(debugString, "=== Callstack ===");
      for (uint32 index = 0; index < strList.size(); index++)
      {
        const String& str = strList.at(index);
        LogErrorFMT(debugString, "%s", str.c_str());
      }
      LogError(debugString, "=== End ===");

      SignalAssertMessage(pzBuffer, callstack.c_str(), isFatal);
    }
    else
    {
      SignalAssertMessage(pzBuffer, "", isFatal );
    }

    if ( isFatal )
    {
      MessageBoxA(NULL, pzBuffer, "ASSERT!", MB_OK | MB_ICONHAND);
      if ( AmIBeingDebugged() )
      {
        ::DebugBreak();
      }
      exit(-1);
    }
    // Only break into code in debugger
    else if ( AmIBeingDebugged() )
    {
      String str("\n\nPress ABORT to Exit client\nPress RETRY to debug client\nPress IGNORE to continue\n");
      if (len + str.length() + 1 > sizeof(pzBuffer))
      {
        ::DebugBreak();
      }
      else
      {
        memcpy(pzBuffer + len, str.c_str(), str.length());
        memset(pzBuffer + len + str.length(), 0, 1);
      }

      int btn = MessageBoxA(NULL, pzBuffer, debugString.c_str(), MB_ABORTRETRYIGNORE | MB_ICONHAND);
      if (btn == IDABORT)
      {
        exit(-1);
      }
      else if (btn == IDRETRY)
      {
        ::DebugBreak();
      }
    }
  }
}

bool dfAssertHandler::AmIBeingDebugged()
{
  return ( IsDebuggerPresent() > 0 );
}

#endif