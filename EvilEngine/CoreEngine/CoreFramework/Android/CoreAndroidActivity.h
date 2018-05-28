#ifndef EVILENGINE_COREENGINE_COREANDROIDACTIVITY
#define EVILENGINE_COREENGINE_COREANDROIDACTIVITY 1
#pragma once

#if defined PLATFORM_ANDROID

//#include <jni.h>

class CoreAndroidActivity
{
public:
  static void HandleTouchEvents( uint32 pointerIndex, uint32 actionId, float xPos, float yPos );
};

#endif

#endif ///  PROJECT_ANDROID_JNI_COM_DARKFACTOR_EVILENGINE