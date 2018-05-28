
#include "Precompile.h"
#include "DFAssert.h"

#if defined PLATFORM_IOS

#include "EvilEngine/CoreIO/Logger/Logger.h"
#include <string.h>
#include "DFCallstack.h"

#include <assert.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/sysctl.h>

#include <signal.h>

void SignalHandler(int signal)
{
}

void dfAssertHandler::SetupSignalHandlers()
{
  //NSSetUncaughtExceptionHandler(&HandleException);
  signal(SIGABRT, SignalHandler);
  signal(SIGILL, SignalHandler);
  signal(SIGSEGV, SignalHandler);
  signal(SIGFPE, SignalHandler);
  signal(SIGBUS, SignalHandler);
  signal(SIGPIPE, SignalHandler);
}

void dfAssertHandler::AssertWithMessage( bool condition, bool useCallstack, const char* errorMessage, va_list pArgs, bool isFatal )
{
  if ( !condition )
  {
    //int maxLen  = 2048;
    int len       = 0;

    char pzBuffer[ 4096 ];
    memset( pzBuffer, 0, 4096 );

    len = vsprintf( pzBuffer, errorMessage, pArgs );

    LogError( "ASSERT", pzBuffer );

    if ( AmIBeingDebugged() )
    {
      [NSException raise:@"AssertWithMessage" format:@"%s", errorMessage];
    }
    
    if ( isFatal )
    {
      abort();
    }
  }
}

bool dfAssertHandler::AmIBeingDebugged()
{
  int                 junk;
  int                 mib[4];
  struct kinfo_proc   info;
  size_t              size;

  // Initialize the flags so that, if sysctl fails for some bizarre
  // reason, we get a predictable result.

  info.kp_proc.p_flag = 0;

  // Initialize mib, which tells sysctl the info we want, in this case
  // we're looking for information about a specific process ID.

  mib[0] = CTL_KERN;
  mib[1] = KERN_PROC;
  mib[2] = KERN_PROC_PID;
  mib[3] = getpid();

  // Call sysctl.

  size = sizeof(info);
  junk = sysctl(mib, sizeof(mib) / sizeof(*mib), &info, &size, NULL, 0);
  assert(junk == 0);

  // We're being debugged if the P_TRACED flag is set.

  return ( (info.kp_proc.p_flag & P_TRACED) != 0 );
}

#endif
