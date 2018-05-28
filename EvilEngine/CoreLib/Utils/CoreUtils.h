#ifndef EVILENGINE_CORELIB_COREUTILS_COREUTILS
#define EVILENGINE_CORELIB_COREUTILS_COREUTILS 1

class CoreUtils
{
public:

  enum CURRENT_PLATFORM
  {
    CURRENT_PLATFORM_NONE     = 0,
    CURRENT_PLATFORM_WINDOWS  = 1,
    CURRENT_PLATFORM_LINUX    = 2,
    CURRENT_PLATFORM_IOS      = 3,
    CURRENT_PLATFORM_ANDROID  = 4,
    CURRENT_PLATFORM_OSX      = 5,
  };

  static uint32   GetPowerOfTwo( uint32 number );
  static bool     IsPowerOfTwo( uint32 number );
  static float    Deg2Rad( uint32 degree );
  static uint32   RemoveFlag( uint32 flags, uint32 flag );
  static uint32   GetThreadId();

  static uint8    GetCurrentPlatform();
  static float    GetCoordFromScreenPixel( int screenPixel, int maxPixelSize, float ratio );

  static uint64   Atoll(const char *zString);
  static uint32   GetHour();
  static uint32   GetDay();
  static unsigned long     GetTime();
  static uint32   GetTimestamp();
  static char*    CharToUpper(char* data, uint32 datalength);
  static bool     FindString(char* data, char* searchData);
  static uint32   Pow2(uint32 number);
  static String   ToHexString(const char* data, uint32 offset, uint32 datalength);
#ifdef WINDOWS
  static wchar_t* ConvertCharArrayToLPCWSTR(const char* charArray);
#endif
};

#endif /// EVILENGINE_CORELIB_COREUTILS_COREUTILS