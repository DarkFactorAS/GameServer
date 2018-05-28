
#include "PreCompile.h"
#include "CoreAndroidUtils.h"

/************************************************************************************************
* ConvertJString:
* Helper function to convert a Java string to a c++ std::string
*
* @param  (JNIEnv*)     env           - The Java environment
* @param  (jstring)     str           - The Java string we want to convert
* @return (std::string)               - The new std::string  
*
* @author Thor Richard Hansen
*************************************************************************************************/
#if defined PLATFORM_ANDROID
std::string AndroidUtils::ConvertJString(JNIEnv* env, jstring str)
{
  //if ( !str ) LString();

  const jsize len = env->GetStringUTFLength(str);
  const char* strChars = env->GetStringUTFChars(str, (jboolean *)0);

  std::string Result(strChars, len);

  env->ReleaseStringUTFChars(str, strChars);

  return Result;
}
#endif
