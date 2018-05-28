
#include "Precompile.h"
#include "DFAssert.h"
#include "EvilEngine/CoreIO/Logger/Logger.h"
#include <string.h>
#include "DFCallstack.h"

Signal3< void, const char*, const char*, bool > dfAssertHandler::SignalAssertMessage;
Signal0< void >                                 dfAssertHandler::SignalCrash;

void PureVirtualFunctionCallHandler()
{
}

dfAssertHandler::dfAssertHandler(  )
{
}

dfAssertHandler* dfAssertHandler::GetInstance()
{
  static dfAssertHandler* instance = NULL;
  if ( instance == NULL )
  {
    instance = new dfAssertHandler();
  }
  return instance;
}

void dfAssertHandler::cause_segfault()
{
  int * p = (int*)0x12345678;
  *p = 0;
}

void dfAssertHandler::Assert(bool condition, const char* errorMessage, ...)
{
  if ( !condition )
  {
    va_list sArgList;
    va_start( sArgList, errorMessage );
    GetInstance()->AssertWithMessage( condition, true, errorMessage, sArgList, true );
    va_end( sArgList );
  }
}

void dfAssertHandler::Bugreport(bool condition, bool sendCallstack, const char* errorMessage, ...)
{
  if ( !condition )
  {
    va_list sArgList;
    va_start( sArgList, errorMessage );
    GetInstance()->AssertWithMessage( condition, sendCallstack, errorMessage, sArgList, false );
    va_end( sArgList );
  }
}

String dfAssertHandler::PrintStackTrace()
{
  String callstack;
  DFCallstack* dfCallstack = DFCallstack::GetInstanceIfAny();
  if ( dfCallstack != NULL )
  {
    dfCallstack->GetCallStack( callstack );
  }
  return callstack;
}
