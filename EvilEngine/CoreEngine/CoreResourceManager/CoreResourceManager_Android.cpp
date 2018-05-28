/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : AndroidResourceManager
*
* Description : Android implementation of the resource loading. All resources on Android are 
*               bundled in the APK, so all reading of files are reading the resources directly
*               from the APK. 
*               TODO: Add support for memory card
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/
#include "PreCompile.h"

#ifdef ANDROID

#include "CoreResourceManager.h"
#include "EvilEngine/CoreIO/Logger/Logger.h"

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

/************************************************************************************************
* LoadResource:
* Loads a resource from the APK package. This function is overloaded from the CoreResourceManager
*
* @param  (const char*) resourcename - Name of the resource to load
* @return (ResourceRef*)             - Returns a pointer to the buffer that we just loaded. If
*                                      the file doesnt exist, return NULL.
*
* @author Thor Richard Hansen
*************************************************************************************************/
ResourceRef* CoreResourceManager::LoadResource( const char* resourcename )
{
  if ( m_AssetManager == NULL )
  {
    return NULL;
  }

  AAsset* asset = AAssetManager_open(m_AssetManager, resourcename, AASSET_MODE_STREAMING);
  if ( asset != NULL )
  {
    off_t assetSize = AAsset_getLength(asset);

    if ( assetSize > 0 )
    {
      uint8_t* buf = new uint8_t[assetSize];
      int     nb_read = 0;
      int     readPointer = 0;
      while ((nb_read = AAsset_read(asset, buf + readPointer, assetSize)) > 0 && readPointer < assetSize )
      {
        readPointer += nb_read;
      }
      AAsset_close(asset);

      LogInfoFMT("CoreResourceManager","Loaded resource '%s' size=%d", resourcename, readPointer );

      return new ResourceRef( readPointer, buf );
    }
    AAsset_close(asset);
  }
  LogInfoFMT("CoreResourceManager","Failed to load resource '%s'. Not found in bundle", resourcename );
  return NULL;
}

/************************************************************************************************
* SaveResource:
* Saves a resource to disk. Not implemented just yet since we have no support for it
*
* @param  (const char*)   resourcename - Name of the resource to load
* @return (ResourceRef*)  resourceRef  - Pointer to the buffer and datalength that we want to write
*
* @author Thor Richard Hansen
*************************************************************************************************/
bool CoreResourceManager::SaveResource( const char* /*resourcename*/, ResourceRef* /*resourceRef*/ )
{
  return false;
}

/************************************************************************************************
* SetAssetManager:
* Sets he assetmanager that is used from java. This manager controls the resources that is 
* bundled with the APK.
*
* @return (AAssetManager*)    - The Android asset manager
*
* @author Thor Richard Hansen
*************************************************************************************************/
void CoreResourceManager::SetAssetManager( AAssetManager* assetManager )
{
  m_AssetManager = assetManager;
}

#endif
