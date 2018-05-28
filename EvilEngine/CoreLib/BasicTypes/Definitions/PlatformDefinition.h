#ifndef EVILENGINE_CORELIB_BASICTYPES_DEFINITIONS_PLATFORMDEFINITIONS
#define EVILENGINE_CORELIB_BASICTYPES_DEFINITIONS_PLATFORMDEFINITIONS

#if defined ANDROID

  #define PLATFORM_ANDROID
  #define DF_OVERRIDE
  #define DF_FINAL 

  #include <jni.h>
  #include <android/log.h>

  #define nullptr NULL

  #include "../String/CoreString.h"
  static const String    s_platform = "android";

#elif defined WIN64

  // C++ 11 feature. Will give an error if a virtual function does not have a base to overload from
  // This usually happens when we change the signature of the baseclass.
  // The DF_OVERRIDE is added on the overloaded function like
  // virtual void DummyFunc() DF_OVERRIDE;
  #define DF_OVERRIDE override

  // C++ 11 feature. Will give an error if a child class is trying to overload a class/function
  // class DummyClass DF_FINAL {}
  // virtual void DummyFunc() DF_FINAL;
  #define DF_FINAL final

  // Make sure that we are not using Windows 8.1 features since they
  // conflict with previous versions
  #define _USING_V110_SDK71_

  #define WIN32_LEAN_AND_MEAN

  #define WINDOWS 1
  #define PLATFORM_WINDOWS 1

  #ifndef _WIN32_WINNT
    #define _WIN32_WINNT    0x0501
  #endif
  #ifndef WINVER
    #define WINVER          0x0501    // Windows XP and newer. 
  #endif

  #include "../String/CoreString.h"
  static const String    s_platform = "win64";

#elif defined WIN32

  // C++ 11 feature. Will give an error if a virtual function does not have a base to overload from
  // This usually happens when we change the signature of the baseclass.
  // The DF_OVERRIDE is added on the overloaded function like
  // virtual void DummyFunc() DF_OVERRIDE;
  #define DF_OVERRIDE override      

  // C++ 11 feature. Will give an error if a child class is trying to overload a class/function
  // class DummyClass DF_FINAL {}
  // virtual void DummyFunc() DF_FINAL;
  #define DF_FINAL final

  // Make sure that we are not using Windows 8.1 features since they
  // conflict with previous versions
  #define _USING_V110_SDK71_

  #define WIN32_LEAN_AND_MEAN

  #define WINDOWS 1
  #define PLATFORM_WINDOWS 1

  #ifndef _WIN32_WINNT
    #define _WIN32_WINNT    0x0501
  #endif
  #ifndef WINVER
    #define WINVER          0x0501    // Windows XP and newer. 
  #endif

  #include "../String/CoreString.h"
  static const String    s_platform = "win32";

#elif defined (__APPLE__) || defined ( MAC )

  #include <TargetConditionals.h>
  #include <stdlib.h>

  #if TARGET_OS_IPHONE

    // C++ 11 feature. Will give an error if a virtual function does not have a base to overload from
    // This usually happens when we change the signature of the baseclass.
    // The DF_OVERRIDE is added on the overloaded function like
    // virtual void DummyFunc() DF_OVERRIDE;
    #define DF_OVERRIDE override

    // C++ 11 feature. Will give an error if a child class is trying to overload a class/function
    // class DummyClass DF_FINAL {}
    // virtual void DummyFunc() DF_FINAL;
    #define DF_FINAL final

    #define PLATFORM_IOS 1

    // Define this to enable use the gamecenter login code
    #define GAMECENTER_ENABLED 1

    // Enables code for in app purchase
    #define IN_APP_PURCHASE 1

  #elif TARGET_OS_MAC
    #define PLATFORM_OSX 1
    #define DF_OVERRIDE
    #define DF_FINAL
  #else
    #error Apple platform is not defined!
  #endif

  #include "../String/CoreString.h"
  static const String    s_platform = "apple";

#elif defined LINUX

  //
  // override is defined in C++ 11 ( gcc 4.8 and newer )
  // This engine is currently linking towards gcc 4.7
  //
  #define DF_OVERRIDE
  #define DF_FINAL

  //#define LINUX 1
  #define PLATFORM_LINUX 1

  #include "../String/CoreString.h"
  static const String    s_platform = "linux32";

  // define this since Im using an old gcc
  #define nullptr NULL

#else
  #error Platform is not defined!
#endif

#if defined(_DEBUG) && !defined(DEBUG)
  #define DEBUG
#endif

#if defined(NDEBUG) && !defined(RELEASE)
  #define RELEASE
#endif

#endif // EVILENGINE_CORELIB_BASICTYPES_DEFINITIONS_PLATFORMDEFINITIONS
