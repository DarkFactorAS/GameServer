/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : CoreResourceManager
*
* Description : Manages all the resources in the game ( sound, images, textfiles etc )
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "CoreResourceManager.h"
#include "EvilEngine/CoreLib/BasicTypes/String/CoreString.h"
#include "EvilEngine/CoreLib/Assert/DFAssert.h"
#include "EvilEngine/CoreIO/Logger/Logger.h"

#include <string.h>
#include <stdlib.h>

CoreResourceManager* CoreResourceManager::s_Instance = NULL;

CoreResourceManager::CoreResourceManager()
{
  m_ResourcePath = NULL;
  m_UserPath     = NULL;
#ifdef ANDROID
  m_AssetManager = NULL;
#endif
}

CoreResourceManager* CoreResourceManager::GetInstance()
{
  if ( s_Instance == NULL )
  {
    s_Instance = new CoreResourceManager();
  }
  return s_Instance; 
}


/************************************************************************************************
* AddSearchPath:
* Sets a global path to where the resource folder is so that we can use filenames without
* paths when loading them
*
* @param  (const char*)   searchPath - The path to add to file handling
*
* @author Thor Richard Hansen
*************************************************************************************************/
void CoreResourceManager::AddSearchPath( const char* searchPath )
{
  size_t len = strlen( searchPath );
  m_ResourcePath = (char*) malloc( len + 1 );
  if ( m_ResourcePath != NULL )
  {
    memset( m_ResourcePath, 0, len + 1 );
    memcpy( m_ResourcePath, searchPath, len );
  }
}

/************************************************************************************************
 * SetUserFolder:
 * Set a path
 *
 * @param  (const char*)   searchPath - The path to add to file handling
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
void CoreResourceManager::SetUserFolder( const char* searchPath )
{
  size_t len = strlen( searchPath );
  m_UserPath = (char*) malloc( len + 1 );
  if ( m_UserPath != NULL )
  {
    memset( m_UserPath, 0, len + 1 );
    memcpy( m_UserPath, searchPath, len );
  }
}

/************************************************************************************************
* GetFilenamePath:
* Returns the full path for a file
*
* @param  (const char*)   filename - The file to get the full path for
* @return (const char*)            - The full path
*
* @author Thor Richard Hansen
*************************************************************************************************/
const char* CoreResourceManager::GetFilenamePath( const char* filename )
{
  if ( m_ResourcePath == NULL )
  {
    return filename;
  }

  size_t resPathLength = strlen( m_ResourcePath );
  size_t filenameLength = strlen( filename );

  char* ret = (char*) malloc( resPathLength + filenameLength + 1 );
  if ( ret != NULL )
  {
    memcpy( ret, m_ResourcePath, resPathLength );
    memcpy( ret + resPathLength, filename, filenameLength );
    memset( ret + resPathLength + filenameLength, 0, 1 );
  }
  return ret;
}

/************************************************************************************************
* AddResource:
* Adds a resource to the resource list.
*
* @param  (Resource*)  resource - The resource to add/load
* @return (int)                 - Return the resourceId for the resource
*
* @author Thor Richard Hansen
*************************************************************************************************/
uint32 CoreResourceManager::AddResource( Resource* resource, uint32 flags  )
{
  uint32 resourceId = resource->GetInstance();

  dfAssert( resourceId != 0, "CoreResourceManager::AddResource. ResourceId == 0!" );
  dfAssert( m_ResourceList.find( resourceId ) == m_ResourceList.end(), "CoreResourceManager::AddResource. Resource already exist!" );

  LogInfoFMT("CoreResourceManager", "AddResource: Added %d/%s to loadqueue", resource->GetId(), resource->GetFilename());

  m_ResourceList[ resourceId ] = resource;
  m_LoadQueue[ resourceId ] = resource;

  resource->SetFlags( flags );

  return resourceId;
}

/************************************************************************************************
* AddResource:
* Adds a resource to the resource list.
*
* @param  (int)        resourceId - Id of the resource we want to add
* @param  (Resource*)  resource   - The resource to add/load
* @return (int)                   - Return the resourceId for the resource
*
* @author Thor Richard Hansen
*************************************************************************************************/
uint32 CoreResourceManager::AddResource( uint32 resourceId, Resource* resource, uint32 flags )
{
  dfAssert( resourceId != 0, "CoreResourceManager::AddResource. ResourceId == 0" );
  dfAssert( resource->GetInstance() == 0, "CoreResourceManager::AddResource. Resource.Id != 0" );
  dfAssert( m_ResourceList.find( resourceId ) == m_ResourceList.end(), "CoreResourceManager::AddResource. Resource already exist!" );

  resource->SetInstance( resourceId );
  resource->SetFlags( flags );

  LogInfoFMT("CoreResourceManager", "AddResource2: Added %d/%s to loadqueue", resource->GetId(), resource->GetFilename());

  m_ResourceList[ resourceId ] = resource;
  m_LoadQueue[ resourceId ] = resource;
  return resourceId;
}

/************************************************************************************************
* RemoveResource:
* Remove a specific resource from the resource list
*
* @param  (Resource*)  resource   - The resource to remove
*
* @author Thor Richard Hansen
*************************************************************************************************/
void CoreResourceManager::RemoveResource( Resource* resource )
{
  if ( resource != NULL )
  {
    RemoveResource( resource->GetInstance() );
  }
}

void CoreResourceManager::RemoveResource( uint32 resourceId )
{
  std::map< uint32_t,Resource* >::iterator itFind = m_ResourceList.find( resourceId );
  if ( itFind != m_ResourceList.end() )
  {
    Resource* resource = itFind->second;
    if ( resource != NULL )
    {
      LogErrorFMT("CoreResourceManager", "Deleting resource [%d::%s]", resource->GetInstance(), resource->GetFilename() );
      delete resource;
    }
    m_ResourceList.erase( itFind );
  }
}

bool CoreResourceManager::ReloadResource(const String& resourceFileName)
{
  for (std::map< uint32_t, Resource* >::iterator itResource = m_ResourceList.begin(); itResource != m_ResourceList.end(); ++itResource)
  {
    Resource* resource = itResource->second;
    if (resource != NULL && resourceFileName.EqualsWithCase( resource->GetFilename() ) )
    {
      resource->SetLoaded(false);
      m_LoadQueue[ resource->GetInstance() ] = resource;
      return true;
    }
  }
  return false;
}

Resource* CoreResourceManager::SyncLoadResource( Resource* resource, uint32 flags )
{
  // Check if it is in the cache first
  Resource* cachedResource = GetResource( resource );
  if ( cachedResource != NULL )
  {
    delete resource;
    return cachedResource;
  }

  if ( ( flags & Resource::RESOURCEFLAG_CACHE ) == Resource::RESOURCEFLAG_CACHE )
  {
    AddResource( resource, flags );
  }
    
  // Force load the resource right away
  if ( !resource->IsLoaded() )
  {
    if ( !resource->LoadData() )
    {
      return NULL;
    }
    resource->OnDoneLoading();
  }
  return resource;
}

Resource* CoreResourceManager::GetResource( Resource* findResource )
{
  if ( !findResource->UniqueByName() )
  {
    std::map< uint32_t,Resource* >::iterator itFind = m_ResourceList.find( findResource->GetInstance() );
    if ( itFind != m_ResourceList.end() )
    {
      Resource* resource = itFind->second;
      if ( !resource->HasFlag( Resource::RESOURCEFLAG_DELETE ) )
      {
        return itFind->second;
      }
    }
  }
  else
  {
    // Scan the resource list for a matching filename
    for ( std::map< uint32_t,Resource* >::iterator itFind = m_ResourceList.begin(); itFind != m_ResourceList.end(); ++itFind )
    {
      Resource* resource = itFind->second;
      if ( resource->HasFlag( Resource::RESOURCEFLAG_DELETE ) )
      {
        continue;
      }

      if ( !resource->UniqueByName() )
      {
        continue;
      }

      if ( strcmp( resource->GetFilename(),  findResource->GetFilename() ) == 0 )
      {
        return resource;
      }
    }
  }
  return NULL;
}

/************************************************************************************************
* UpdateOrAddResource:
* Adds a resource to the resource list if it doesn't exist from before. This function will take
* ownership of the resource, so if the resource exists, the new resource will be deleted and 
* the cached one will be returned.
*
* @param  (Resource*)  resource - The resource to add/load
* @return (int)                 - Return the resourceId for the resource
*
* @author Thor Richard Hansen
*************************************************************************************************/
uint32 CoreResourceManager::UpdateOrAddResource( Resource* resource )
{
  uint32 resourceId = resource->GetInstance();
  std::map< uint32, Resource* >::const_iterator itObj = m_ResourceList.find( resourceId );
  if ( itObj != m_ResourceList.end() )
  {
    delete resource;
    const Resource* existingResource = itObj->second;
    return existingResource->GetInstance();
  }
  else
  {
    LogInfoFMT("CoreResourceManager","UpdateOrAddResource: Added %d/%s to loadqueue", resource->GetId(), resource->GetFilename());
    resource->SetFlags( Resource::RESOURCEFLAG_CACHE );
    m_ResourceList[ resourceId ] = resource;
    m_LoadQueue[ resourceId ] = resource;
  }
  return resourceId;
}

/************************************************************************************************
* GetResource:
* Returns a resource in the resourcelist with a specific ID ( if it doesnt exist it will return
* NULL )
*
* @param  (int)       resourceId   - The ID of the resource to search for
* @return (Resource*)              - The resource matching the ID
*
* @author Thor Richard Hansen
*************************************************************************************************/
Resource* CoreResourceManager::GetResource( uint32 resourceId )
{
  std::map< uint32, Resource* >::const_iterator itObj = m_ResourceList.find( resourceId );
  if ( itObj != m_ResourceList.end() )
  {
    return itObj->second;
  }
  return NULL;
}

/************************************************************************************************
* GetResource:
* Returns a resource in the resourcelist with a specific ID and type ( if it doesnt exist it 
* will return NULL )
*
* @param  (int)       resourceType - The ID of the resource to search for
* @param  (int)       resourceId   - The ID of the resource to search for
* @return (Resource*)              - The resource matching the ID
*
* @author Thor Richard Hansen
*************************************************************************************************/
Resource* CoreResourceManager::GetResource( int resourceType, uint32 resourceId )
{
  std::map< uint32, Resource* >::const_iterator itObj = m_ResourceList.find( resourceId );
  if ( itObj != m_ResourceList.end() )
  {
    Resource* resource = itObj->second;
    if ( resource->GetType() == resourceType )
    {
      return resource;
    }
  }
  return NULL;
}

/************************************************************************************************
* GetResource:
* Calls the LoadData function on all resources that havent loaded their data already. This will
* force all resources to load their data in sequence. This is done sync, so it will stall the 
* entire engine while loading. TODO : Change this to be async and/or spread over multiple frames
*
* @author Thor Richard Hansen
*************************************************************************************************/
void CoreResourceManager::LoadResources()
{
  for ( std::map< uint32_t,Resource* >::iterator itObj = m_LoadQueue.begin(); itObj != m_LoadQueue.end(); ++itObj )
  {
    Resource* obj = itObj->second;
    if ( !obj->IsLoaded() )
    {
      if ( !obj->LoadData() )
      {
        LogErrorFMT("CoreResourceManager", "Loading resource [%d::%s] [FAILED]", obj->GetInstance(), obj->GetFilename() );
        RemoveResource( obj );
      }
      else
      {
        obj->SetLoaded(true);
        obj->OnDoneLoading();
        if ( !obj->ShouldCache() )
        {
          RemoveResource( obj );
        }
      }
    }
  }
  m_LoadQueue.clear();
}

void CoreResourceManager::ReloadResources()
{
  for ( std::map< uint32, Resource* >::const_iterator itObj = m_ResourceList.begin(); itObj != m_ResourceList.end(); itObj++ )
  {
    Resource* resource = itObj->second;
    if ( !resource->HasFlag( Resource::RESOURCEFLAG_DELETE) && resource->ReloadData() )
    {
      //resource->OnDoneLoading();
    }
  }
}

bool CoreResourceManager::IsResourceLoaded(uint32 resourceId)
{
  // Use cache if we have it in the cache or is already loading it
  Resource* cachedResource = GetResource(resourceId);
  if (cachedResource != NULL)
  {
    return cachedResource->IsLoaded();
  }
  return false;
}
