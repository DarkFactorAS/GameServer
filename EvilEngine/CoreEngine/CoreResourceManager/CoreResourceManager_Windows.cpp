/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : Windows ResourceManager
* Description : Windows implementation of the resource loading
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/
#include "Precompile.h"

#ifdef WINDOWS

#include "CoreResourceManager.h"
#include <stdio.h>
#include <string.h>

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
  FILE * file = NULL;

  const char* filename = GetFilenamePath( resourcename );

  fopen_s(&file, filename,"rb");

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

  int readPos = 0;
  readPos = fread(rawMemory,1,fileSize,file);

  fclose( file );

  return new ResourceRef( fileSize, rawMemory );
}

/************************************************************************************************
* SaveResource:
* Saves a resource to disk. 
*
* @param  (const char*)   resourcename - Name of the resource to load
* @param  (ResourceRef*)  resourceRef  - Pointer to the buffer and datalength that we want to write
* @return (bool)                       - Returns true if the save was successful
*
* @author Thor Richard Hansen
*************************************************************************************************/
bool CoreResourceManager::SaveResource( const char* resourcename, ResourceRef* resourceRef )
{
  FILE * file = NULL;

  const char* filename = GetFilenamePath( resourcename );

  fopen_s(&file, filename,"wb");

  if ( file == NULL )
  {
    return NULL;
  }

  long bytesWritten = (long) fwrite( resourceRef->m_Data, sizeof(char), resourceRef->m_Filesize, file );
  fclose( file );

  return ( bytesWritten == resourceRef->m_Filesize );
}

#endif
