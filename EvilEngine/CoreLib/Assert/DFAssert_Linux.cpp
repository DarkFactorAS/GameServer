
#include "Precompile.h"

#if defined PLATFORM_LINUX

#include "DFAssert.h"
#include "EvilEngine/CoreIO/Logger/Logger.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <ctype.h>
#include <limits.h>
#include <string.h>
#include <unistd.h> 
#include <sys/ptrace.h>
#include <sys/wait.h>

#include <getopt.h>
#include <signal.h>

#define HAVE_DLADDR 1
#define HAVE_CXXABI 1

#if HAVE_DLADDR
  #include <dlfcn.h>
#endif

#if HAVE_CXXABI
  #include <cxxabi.h>
#endif

#include <unwind.h>

typedef struct 
{
  size_t count;
  size_t ignore;
  const void** addrs;
} stack_crawl_state_t;

static _Unwind_Reason_Code trace_function(_Unwind_Context *context, void *arg)
{
  stack_crawl_state_t* state = (stack_crawl_state_t*)arg;
  if (state->count) {
    void* ip = (void*)_Unwind_GetIP(context);
    if (ip) {
      if (state->ignore) {
        state->ignore--;
      } else {
        state->addrs[0] = ip; 
        state->addrs++;
        state->count--;
      }
    }
  }
  return _URC_NO_REASON;
}

static int unwind_backtrace(const void** addrs, size_t ignore, size_t size)
{
  stack_crawl_state_t state;
  state.count = size;
  state.ignore = ignore;
  state.addrs = addrs;
  _Unwind_Backtrace(trace_function, (void*)&state);
  return size - state.count;
}

static const char *lookup_symbol(const void* addr, void **offset, char* name, size_t bufSize)
{
#if HAVE_DLADDR
  Dl_info info;
  if (dladdr(addr, &info)) 
  {
    *offset = info.dli_saddr;
    return info.dli_sname;
  }
#endif
  return NULL;
}

static 
  int32_t linux_gcc_demangler(const char *mangled_name, char *unmangled_name, size_t buffersize)
{
  size_t out_len = 0;
#if HAVE_CXXABI
  int status = 0;
  char *demangled = abi::__cxa_demangle(mangled_name, 0, &out_len, &status);
  if (status == 0) {
    // OK
    if (out_len < buffersize) memcpy(unmangled_name, demangled, out_len);
    else out_len = 0;
    free(demangled);
  } else {
    out_len = 0;
  }
#endif
  return out_len;
}

struct sigaction new_action, old_action, no_action;

/// SetupSignalHandlers
void dfAssertHandler::SetupSignalHandlers() 
{
  sigemptyset (&new_action.sa_mask);
  new_action.sa_sigaction = &dfAssertHandler::HandleSystemAssert;
  new_action.sa_flags = SA_SIGINFO;

  sigemptyset(&no_action.sa_mask);
  no_action.sa_sigaction = &dfAssertHandler::MuteSignal;
  no_action.sa_flags = SA_SIGINFO;

  sigaction (SIGHUP, &new_action, NULL);
  sigaction (SIGILL, &new_action, NULL);
  sigaction (SIGFPE, &new_action, NULL);
  sigaction (SIGSEGV, &new_action, NULL);
  sigaction (SIGABRT, &new_action, NULL);
  sigaction (SIGINT, &new_action, NULL);
  sigaction (SIGTERM, &new_action, NULL);
  sigaction (SIGQUIT, &new_action, NULL);
  sigaction (SIGUSR1, &new_action, NULL);
  sigaction (SIGUSR2, &new_action, NULL);
  //sigaction (SIGUSR3, &new_action, NULL);

  sigaction(SIGTRAP, &no_action, NULL);

  LogInfo("DarkFactor", "dfAssertHandler::SetupSignalHandlers" );

  // Uncomment to test the handlers
  //dfAssertHandler::cause_segfault();
}

void dfAssertHandler::SetBlockAllSignals(bool block)
{
  SetBlockSignal(SIGHUP,block);
  SetBlockSignal(SIGILL,block);
  SetBlockSignal(SIGFPE,block);
  SetBlockSignal(SIGSEGV,block);
  SetBlockSignal(SIGABRT,block);
  SetBlockSignal(SIGINT,block);
  SetBlockSignal(SIGTERM,block);
  SetBlockSignal(SIGQUIT,block);
  SetBlockSignal(SIGUSR1,block);
  SetBlockSignal(SIGUSR2,block);
  SetBlockSignal(SIGTRAP, block);
  //SetBlockSignal(SIGUSR3,block);
  LogInfo("DarkFactor", "dfAssertHandler::SetBlockAllSignals" );
}


void dfAssertHandler::SetBlockSignal(int signal, bool block)
{
  sigset_t signalData;
  sigemptyset (&signalData);
  sigaddset(&signalData, signal);
  if (block)
    sigprocmask(SIG_BLOCK, &signalData, NULL);
  else
    sigprocmask(SIG_UNBLOCK, &signalData, NULL);
}

/************************************************************************************************
* DumpCallstack:
* Dumps the callstack for the current thread in the console. If the bool argument is true, it 
* will also return the callstack in a neatly formatted string.
*
* @param  (bool)    returnString  - Set to true to dump the callstack into the returnstring
* @return (String)                - The callstack formatted in a string
*
* @author Thor Richard Hansen
*************************************************************************************************/
String dfAssertHandler::DumpCallstack( bool returnString )
{
  char namebuf[1024];
  void *offs;
  char tmp[256];

  const void* frames[256];
  int count = unwind_backtrace(frames, 0, sizeof(frames));

  String callstackStr;

  //backtrace_symbol_t symbols[count];
  for ( int index = 0; index < count; index++ )
  {
    const char* name = lookup_symbol( frames[index], &offs, namebuf, sizeof(namebuf));
    if ( name != NULL )
    {
      if ( linux_gcc_demangler(name, tmp, 256) != 0 ) 
      {
        name = tmp;
      }
      LogErrorFMT( "AssertWithMessage", "[%02d] [%08x] %s", index, frames[index], name );

      if ( returnString )
      {
        callstackStr += String::FormatString("[%02d] [%08x] %s", index, frames[index], name );
      }
    }
    else
    {
      LogErrorFMT( "AssertWithMessage", "[%02d] [%08x] <unknown>", index, frames[index] );
      if ( returnString )
      {
        callstackStr += String::FormatString("[%02d] [%08x] <unknown>", index, frames[index] );
      }
    }
  }

  if ( returnString )
  {
    return callstackStr;
  }
  return String::zero;
}

void dfAssertHandler::HandleSystemAssert(int signalNumber, siginfo_t *sigInfo, void *context) 
{
  static volatile sig_atomic_t fatal_error_in_progress = 0;
  static volatile sig_atomic_t max_retries_until_death = 0;
  if (fatal_error_in_progress)
  {
    max_retries_until_death++;

    // If we have chaincrashed 1000 times without getting through -> exit application
    if ( max_retries_until_death > 1000 )
    {
      exit(-1);
    }
    return;
  }
  fatal_error_in_progress = 1;
  //max_retries_until_death = 0;

  // Pre callstack handing of the signal
  String segName;
  switch( signalNumber )
  {
    case SIGSEGV:
      segName = "SIGSEGV";
      break;
    case SIGINT:
    case SIGHUP:
      SignalCrash();
      fatal_error_in_progress = 0;
      return;
    default:
      segName = String::FormatString("SIG[%d]", signalNumber );
      break;
  }

  //
  // Check if I can fetch the linenumber/file of the actual crash
  //
  char namebuf[1024];
  void *offs;
  char tmp[256];

  const char* name = lookup_symbol( sigInfo->si_addr, &offs, namebuf, sizeof(namebuf));
  if ( name != NULL )
  {
    if ( linux_gcc_demangler(name, tmp, 256) != 0 ) 
    {
      name = tmp;
    }
    LogErrorFMT( "handleCrash", "%s [%08x] %s\n", segName.c_str(), (long) sigInfo->si_addr, name );
  }
  else
  {
    LogErrorFMT( "handleCrash", "%s [%08x] <unknown>\n", segName.c_str(), (long) sigInfo->si_addr );
  }

  //const void* frames[256];
  //int count = unwind_backtrace(frames, 0, sizeof(frames));
  ////backtrace_symbol_t symbols[count];
  //for ( int index = 0; index < count; index++ )
  //{
  //  LogErrorFMT( "handleCrash", "[%02d] [%08x]", index, frames[index] );
  //}

  //CESignalHandler::getStackTrace();
  //sigaction(signalNumber, &old_action, NULL); 

  DumpCallstack( false );

  fatal_error_in_progress = 0;

  switch( signalNumber )
  {
  case SIGILL: 
  case SIGBUS:  
  case SIGFPE:  
  case SIGINT:
    SetBlockAllSignals( true );
    if ( !SignalCrash() )
    {
      LogErrorFMT("HandleSystemAssert","Nothing handled SignalCrash for signal[%d]. Closing application.", signalNumber);
      abort();
    }
    break;

  case SIGSEGV:
    SetBlockAllSignals(true);
    exit(-1);

  case SIGABRT:
    SetBlockAllSignals( true );
    exit(-1);
    break;

  default:
    break;
  }
}

void dfAssertHandler::AssertWithMessage( bool condition, bool useCallstack, const char* errorMessage, va_list pArgs, bool isFatal )
{
  if ( !condition )
  {
    LogErrorFMT( "AssertWithMessage", errorMessage, pArgs );

    String callstackStr = (useCallstack) ? DumpCallstack(true) : String::zero;

    char pzBuffer[4096];
    memset(pzBuffer, 0, 4096);
    int len = vsprintf(pzBuffer, errorMessage, pArgs);
    if ( len > 0 )
    {
      SignalAssertMessage(pzBuffer, callstackStr.c_str(), isFatal);
    }
    else
    {
      SignalAssertMessage(errorMessage, callstackStr.c_str(), isFatal);
    }

    // Should break in debugger
    if (AmIBeingDebugged())
    {
      raise(SIGTRAP);
    }

    if ( isFatal )
    {
      LogError("AssertWithMessage","Crash is fatal. Closing application.");
      abort();
    }
  }
}

bool dfAssertHandler::AmIBeingDebugged()
{
    int pid = fork();
    int status;
    int res;

    if (pid == -1)
    {
      perror("fork");
      return -1;
    }

    if (pid == 0)
    {
      int ppid = getppid();

      /* Child */
      if (ptrace(PTRACE_ATTACH, ppid, NULL, NULL) == 0)
      {
        /* Wait for the parent to stop and continue it */
        waitpid(ppid, NULL, 0);
        ptrace(PTRACE_CONT, NULL, NULL);

        /* Detach */
        ptrace(PTRACE_DETACH, getppid(), NULL, NULL);

        /* We were the tracers, so gdb is not present */
        res = 0;
      }
      else
      {
        /* Trace failed so gdb is present */
        res = 1;
      }
      exit(res);
    }
    else
    {
      waitpid(pid, &status, 0);
      res = WEXITSTATUS(status);
    }
    return res;
}

#endif
