#ifndef EVILENGINE_COREENGINE_COREANDROIDUTILS
#define EVILENGINE_COREENGINE_COREANDROIDUTILS 1
#pragma once

#if defined PLATFORM_ANDROID

class AndroidUtils
{
public:
  static std::string  ConvertJString(JNIEnv* env, jstring str);
};

#endif

#endif /// EVILENGINE_COREENGINE_COREANDROIDUTILS