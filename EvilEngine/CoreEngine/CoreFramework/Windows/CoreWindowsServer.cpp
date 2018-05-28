#include "Precompile.h"

#if defined PLATFORM_WINDOWS

#include "CoreWindowsServer.h"

CoreWindowsServer::CoreWindowsServer( const char* winName, const char* winDisplayName) :
  CoreWindowsCommon( winName, winDisplayName )
{
}

#endif /// PLATFORM_WINDOWS
