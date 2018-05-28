/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : Windows ResourceManager
* Description : Windows implementation of the resource loading
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/
#include "Precompile.h"

#ifdef PLATFORM_LINUX

#include "CoreResourceManager.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/************************************************************************************************
* LoadResource:
* Loads a resource from disk.
*
* @param  (const char*) resourcename - Filename for the resource to load
* @return (uint8_t*)                 - Returns a pointer to the buffer that we just loaded. If
*                                      the file doesnt exist, return NULL.
*
* @author Thor Richard Hansen
*************************************************************************************************/
ResourceRef* CoreResourceManager::LoadResource( const char* resourcename )
{
  const char* filename = GetFilenamePath( resourcename );

  FILE * file = fopen( filename,"rb" );
  if ( file == NULL )
  {
    return NULL;
  }

  fseek( file, 0 , SEEK_END );
  long fileSize = ftell (file);
  fseek( file, 0 , SEEK_SET );

  uint8_t* rawMemory = (uint8_t*)malloc( fileSize + 1 );
  if ( rawMemory == NULL )
  {
    return NULL;
  }
  memset( rawMemory, 0, fileSize + 1 );

  //int readPos = 0;
  //readPos = 
  fread(rawMemory,1,fileSize,file);

  fclose( file );

  return new ResourceRef( fileSize, rawMemory );
}

bool CoreResourceManager::SaveResource( const char* /*resourcename*/, ResourceRef* /*resourceRef*/ )
{
  return false;
}

#endif
