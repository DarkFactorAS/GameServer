/*************************************************************************************************
 * Project     : EvilEngine Multiplatform 2014
 * File        : ResourceManager_IOS
 * Description : IOS implementation of the resource loading
 *
 * Author      : Thor Richard Hansen
 *************************************************************************************************/
#include "PreCompile.h"

#ifdef PLATFORM_IOS

#include "CoreResourceManager.h"
#include <stdlib.h>
#include "EvilEngine/CoreLib/BasicTypes/String/CoreString.h"
#include "EvilEngine/CoreIO/Logger/Logger.h"
#import <UIKit/UIKit.h>

ResourceRef* CoreResourceManager::LoadResource( const char* localResourceName )
{
  CFBundleRef mainBundle = CFBundleGetMainBundle();
  CFURLRef resourcesURL = CFBundleCopyBundleURL(mainBundle);
  CFStringRef str = CFURLCopyFileSystemPath( resourcesURL, kCFURLPOSIXPathStyle );
  CFRelease(resourcesURL);
  char resourcePath[PATH_MAX];
  
  CFStringGetCString( str, resourcePath, FILENAME_MAX, kCFStringEncodingASCII );
  CFRelease(str);

  String resourceName = String::FormatString("%s/%s", resourcePath, localResourceName );
  
  FILE* file = fopen( resourceName.c_str(), "rb");
  if ( file == NULL )
  {
    LogErrorFMT("CoreResourceManager", "Loading resource [%s] [FAILED]", resourceName.c_str() );
    return NULL;
  }
  
  fseek( file, 0 , SEEK_END );
  long fileSize = ftell (file);
  fseek( file, 0 , SEEK_SET );
  //rewind( file );
  
  uint8_t* rawMemory = (uint8_t*)malloc( fileSize + 1 );
  memset( rawMemory, 0, fileSize + 1 );
  
  //{
  // Read the actual data from the file into the buffer
  //size_t readPos =
  fread(rawMemory,1,fileSize,file);
  //}
  
  fclose( file );
  
  return new ResourceRef( fileSize, rawMemory );
}

bool CoreResourceManager::SaveResource( const char* /*resourcename*/, ResourceRef* /*resourceRef*/ )
{
  return false;
}

#endif
