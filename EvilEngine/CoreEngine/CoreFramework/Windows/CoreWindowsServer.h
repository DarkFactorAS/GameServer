#ifndef EVILENGINE_COREENGINE_COREWINDOWSSERVER
#define EVILENGINE_COREENGINE_COREWINDOWSSERVER 1
#pragma once

#if defined PLATFORM_WINDOWS

#include "CoreWindowsCommon.h"

class CoreWindowsServer : public CoreWindowsCommon
{
public:
  CoreWindowsServer( const char* winName, const char* winDisplayNam);
};

#endif /// PLATFORM_WINDOWS
#endif /// EVILENGINE_COREENGINE_COREWINDOWSSERVER
