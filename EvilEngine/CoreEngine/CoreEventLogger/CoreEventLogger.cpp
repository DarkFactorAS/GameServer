
#include "PreCompile.h"

#ifdef PLATFORM_WINDOWS

#include "CoreEventLogger.h"
#include <strsafe.h>

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
void CoreEventLogger::WriteEventLogEntry(PWSTR serviceName, PWSTR pszMessage, WORD wType)
{
  HANDLE hEventSource = NULL;
  LPCWSTR lpszStrings[2] = { NULL, NULL };

  hEventSource = RegisterEventSource(NULL, serviceName);
  if (hEventSource)
  {
    lpszStrings[0] = serviceName;
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


//
//   FUNCTION: CoreWindowsService::WriteErrorLogEntry(PWSTR, DWORD)
//
//   PURPOSE: Log an error message to the Application event log.
//
//   PARAMETERS:
//   * pszFunction - the function that gives the error
//   * dwError - the error code
//
void CoreEventLogger::WriteErrorLogEntry(PWSTR serviceName,PWSTR pszFunction, DWORD dwError)
{
  wchar_t szMessage[260];
  StringCchPrintf(szMessage, ARRAYSIZE(szMessage),
    L"%s failed w/err 0x%08lx", pszFunction, dwError);
  WriteEventLogEntry(serviceName,szMessage, EVENTLOG_ERROR_TYPE);
}

#endif
