#ifndef EVILENGINE_COREENGINE_COREWINDOWSCLIENT
#define EVILENGINE_COREENGINE_COREWINDOWSCLIENT 1
#pragma once

#if defined PLATFORM_WINDOWS

#include "CoreWindowsCommon.h"
#include <windef.h>

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

class CoreWindowsClient : public CoreWindowsCommon
{
public:
  CoreWindowsClient( const char* winName, const char* winDisplayName );
  HWND    CreateClientWindow( int iconId );

  int     m_Width;
  int     m_Height;
};

#endif /// PLATFORM_WINDOWS
#endif /// EVILENGINE_COREENGINE_COREWINDOWSCLIENT
