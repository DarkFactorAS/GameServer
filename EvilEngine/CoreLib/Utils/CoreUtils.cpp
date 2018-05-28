#include "Precompile.h"
#include "CoreUtils.h"
#include "../CoreMath/Math.h"
#include <time.h>
#include <string.h>

#if defined PLATFORM_WINDOWS
  #include <windows.h>
#elif defined PLATFORM_LINUX
  #include <unistd.h>
  #include <sys/time.h>
  #include<stdio.h>
#elif defined PLATFORM_OSX
  #include <unistd.h>
  #include <sys/time.h>
  #include <pthread.h>
#endif

uint32 CoreUtils::GetPowerOfTwo( uint32 number )
{
  uint32 powNumber = 1;
  do
  {
    powNumber = powNumber << 1;
  }
  while ( powNumber < number );

  return powNumber;
}
   
bool CoreUtils::IsPowerOfTwo( uint32 number )
{
  if ( GetPowerOfTwo( number ) == number )
  {
    return true;
  }
  return false;
}

float CoreUtils::Deg2Rad( uint32 degree )
{
  return (float)( degree * M_PI / 180.0f);
}

/************************************************************************************************
* GetDay:
* Return the day of the year
*
* @return  (uint32)        - The current day
*
* @author Thor Richard Hansen
*************************************************************************************************/
uint32 CoreUtils::GetDay()
{
#if defined PLATFORM_WINDOWS

  SYSTEMTIME time;
  GetSystemTime(&time);
  return time.wDay;

#elif defined PLATFORM_ANDROID

  struct timeval  now;
  gettimeofday(&now, NULL);
  return (unsigned long)(now.tv_sec * 1000 + now.tv_usec / 1000);

#elif defined PLATFORM_LINUX

  time_t rawtime;
  tm * timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  return timeinfo->tm_mday;

  struct timeval  now;
  gettimeofday(&now, NULL);
  return (unsigned long)(now.tv_sec * 1000 + now.tv_usec / 1000);

#elif defined PLATFORM_OSX

  struct timeval  now;
  gettimeofday(&now, NULL);
  return (unsigned long)(now.tv_sec * 1000 + now.tv_usec / 1000);

#else
  #pragma error "GetDay not implemented on this platform"
  return 0;
#endif
}

/************************************************************************************************
* GetTime:
* Return the time in milliseconds
*
* @return  (long)        - The current time in milliseconds
*
* @author Thor Richard Hansen
*************************************************************************************************/
uint32 CoreUtils::GetHour()
{
#if defined PLATFORM_WINDOWS

  SYSTEMTIME time;
  GetSystemTime(&time);
  return time.wHour;

#elif defined PLATFORM_ANDROID

  struct timeval  now;
  gettimeofday(&now, NULL);
  return (unsigned long)(now.tv_sec * 1000 + now.tv_usec / 1000);

#elif defined PLATFORM_LINUX

  struct timeval  now;
  gettimeofday(&now, NULL);
  return (unsigned long)(now.tv_sec * 1000 + now.tv_usec / 1000);

#elif defined PLATFORM_OSX

  struct timeval  now;
  gettimeofday(&now, NULL);
  return (unsigned long)(now.tv_sec * 1000 + now.tv_usec / 1000);

#else
  return 0;
#endif
}

/************************************************************************************************
 * GetTime:
 * Return the time in milliseconds
 *
 * @return  (long)        - The current time in milliseconds
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
unsigned long CoreUtils::GetTime()
{
#if defined PLATFORM_WINDOWS

  SYSTEMTIME time;
  GetSystemTime(&time);
  //return ( time.wDay * 86400000 ) + ( time.wHour * 3600000 ) + ( time.wMinute * 60000 ) + (time.wSecond * 1000) + time.wMilliseconds;
  return (time.wMinute * 60000) + (time.wSecond * 1000) + time.wMilliseconds;

#elif defined PLATFORM_ANDROID

  struct timeval  now;
  gettimeofday(&now, NULL);
  return (unsigned long)(now.tv_sec*1000 + now.tv_usec/1000);

#elif defined PLATFORM_LINUX

  struct timeval  now;
  gettimeofday(&now, NULL);
  return (unsigned long)(now.tv_sec*1000 + now.tv_usec/1000);

#elif defined PLATFORM_OSX

  struct timeval  now;
  gettimeofday(&now, NULL);
  return (unsigned long)(now.tv_sec * 1000 + now.tv_usec / 1000);

#else
  return 0;
#endif
}

/************************************************************************************************
* GetTimestamp:
* Return the time in seconds
*
* @return  (uint32)        - The current time in seconds
*
* @author Thor Richard Hansen
*************************************************************************************************/
uint32 CoreUtils::GetTimestamp()
{
#if defined PLATFORM_WINDOWS

  SYSTEMTIME time;
  GetSystemTime(&time);
  return (time.wDay * 86400) + (time.wHour * 3600) + (time.wMinute * 60) + time.wSecond;

#elif defined PLATFORM_ANDROID

  struct timeval  now;
  gettimeofday(&now, NULL);
  return now.tv_sec;

#elif defined PLATFORM_LINUX

  struct timeval  now;
  gettimeofday(&now, NULL);
  return now.tv_sec;

#elif defined PLATFORM_IOS || defined PLATFORM_OSX

  time_t timer;
  time(&timer);
  struct tm y2k = { 0 };
  y2k.tm_hour = 0;
  y2k.tm_min  = 0;
  y2k.tm_sec  = 0;
  y2k.tm_year = 100;
  y2k.tm_mon  = 0;
  y2k.tm_mday = 1;

  double seconds = difftime( timer, mktime(&y2k ) );
  return seconds;
#else
  #error unknown platform
#endif
}

#ifdef WINDOWS
wchar_t* CoreUtils::ConvertCharArrayToLPCWSTR(const char* charArray)
{
  wchar_t* wString = new wchar_t[4096];
  MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
  return wString;
}
#endif

uint32 CoreUtils::RemoveFlag( uint32 flags, uint32 flag )
{
  flags &= flag;
  return flags;
}

uint32 CoreUtils::GetThreadId()
{
#if defined PLATFORM_WINDOWS
  return GetCurrentThreadId();
#elif defined PLATFORM_LINUX
  return pthread_self();
#elif defined PLATFORM_OSX
  pthread_t id = pthread_self();
  return pthread_mach_thread_np(id);
#else
  return 0;
#endif
}

/************************************************************************************************
* GetCurrentPlatform:
* Return the enum with the current platform. This is used for login and statistics.
*
* @return (uint8)                - The ID of the platform
*
* @author Thor Richard Hansen
*************************************************************************************************/
uint8 CoreUtils::GetCurrentPlatform()
{
#ifdef PLATFORM_WINDOWS
  return CURRENT_PLATFORM_WINDOWS;
#elif defined ( PLATFORM_LINUX )
  return CURRENT_PLATFORM_LINUX;
#elif defined ( PLATFORM_IOS )
  return CURRENT_PLATFORM_IOS;
#elif defined ( PLATFORM_ANDROID )
  return CURRENT_PLATFORM_ANDROID;
#elif defined ( PLATFORM_OSX )
  return CURRENT_PLATFORM_OSX;
#else
  return CURRENT_PLATFORM_NONE;
#endif
}

float CoreUtils::GetCoordFromScreenPixel( int screenPixel, int maxPixelSize, float ratio )
{
  if ( maxPixelSize > 0 )
  {
    float ret = ( screenPixel * ratio ) / maxPixelSize;
    return ret;
  }
  return 0.0f;
}

uint64 CoreUtils::Atoll(const char *zString)
{
// Visual Studio 2015 supports atoll properly
#if defined PLATFORM_WINDOWS && defined(_MSC_VER) && (_MSC_VER < 1900)
  uint64 nValue = 0;
  while(*zString && *zString>='0' && *zString<='9')
  {
    nValue = nValue*10 + *zString-'0';
    zString++;
  }
  return nValue;
#else
  return atoll( zString );
#endif
}

char* CoreUtils::CharToUpper( char* data, uint32 datalength )
{
  char* newData = (char*) malloc( datalength );
  for ( uint32 index = 0; index < datalength; index++ )
  {
    char c = data[index];
    if ( c >= 97 && c <=123 )
    {
      c -= 32;
    }
    newData[index] = c;
  }
  return newData;
}

bool CoreUtils::FindString(char* data, char* searchData )
{
  uint32 dataLength = strlen(data);
  uint32 searchLength = strlen(searchData);

  for (uint i = 0; i < dataLength; i++)
  {
    bool foundStr = true;
    for (uint j = 0; j < searchLength; j++)
    {
      if (data[i + j] != searchData[j])
      {
        foundStr = false;
        break;
      }
    }
    if (foundStr)
    {
      return true;
    }
  }

  return false;
}

uint32 CoreUtils::Pow2( uint32 number )
{
  uint32 minNumber = 2;
  while ( minNumber < number && minNumber < 102400000 )
  {
    minNumber *= 2;
  }
  return minNumber;
}

String CoreUtils::ToHexString(const char* data, uint32 offset, uint32 datalength)
{
  String outString;
  char buffer[4];
  for (uint32 index = offset; index < offset + datalength; index++)
  {
    unsigned char c = data[index];
    snprintf(buffer, sizeof(buffer), "%0x", c);

    if (index > offset)
    {
      outString += "-";
    }

    outString += buffer;
  }
  return outString;
}
