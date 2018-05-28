#pragma once

#ifdef PLATFORM_WINDOWS

#include "windows.h"

class CoreEventLogger
{
public:
  // Log a message to the Application event log.
  static void WriteEventLogEntry(PWSTR serviceName, PWSTR pszMessage, WORD wType);

  // Log an error message to the Application event log.
  static void WriteErrorLogEntry(PWSTR serviceName, PWSTR pszFunction,  DWORD dwError = GetLastError());
};

#endif
