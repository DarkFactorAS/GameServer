#ifndef EVILENGINE_COREENGINE_CORERESOURCEMANAGER
#define EVILENGINE_COREENGINE_CORERESOURCEMANAGER 1

#include <map>
#include <vector>
#include "../../CoreLib/BasicTypes/Definitions/BasicTypes.h"
#include "EvilEngine/CoreLib/Signal/SignalTarget.h"
#include "Resource.h"

#include <string.h>

#ifdef ANDROID
  class AAssetManager;
#endif

class CoreResourceManager : public SignalTarget
{
public:

  CoreResourceManager();

  static CoreResourceManager*     GetInstance();
  static CoreResourceManager*     GetInstanceIfAny(){ return s_Instance; }

  uint32_t                        AddResource( Resource* resource, uint32 flags );
  uint32_t                        AddResource( uint32_t resourceId, Resource* resource, uint32 flags );
  Resource*                       SyncLoadResource( Resource* resource, uint32 flags = 0 );
  void                            RemoveResource( Resource* resource );
  void                            RemoveResource( uint32 resourceId );
  bool                            ReloadResource(const String& resourceFileName);

  uint32_t                        UpdateOrAddResource( Resource* resource );
  Resource*                       GetResource( uint32 resourceId );
  Resource*                       GetResource( int resourceType, uint32 resourceId );
  void                            AddSearchPath( const char* searchPath );
  void                            SetUserFolder( const char* searchPath );
  const char*                     GetSearchPath(){ return m_ResourcePath; }
  const char*                     GetFilenamePath( const char* filename );

  ResourceRef*                    LoadResource( const char* resourcename );
  bool                            SaveResource( const char* resourcename, ResourceRef* resourceRef );
  void                            LoadResources();
  void                            ReloadResources();
  bool                            IsResourceLoaded( uint32 resourceId );

  /// Android specific
#ifdef ANDROID
  void                            SetAssetManager( AAssetManager* );
#endif

  void                            SilentCallback( Resource* /*resource*/ ){};

  void AsyncLoadResource( Resource* resource, uint32 flags )
  {
    AsyncLoadResource( resource, flags, this, &CoreResourceManager::SilentCallback );
  }

  template < typename CALLBACK_OBJECT, typename OBJ_CLASS >
  void AsyncLoadResource( Resource* resource, uint32 flags, CALLBACK_OBJECT* obj, void ( OBJ_CLASS::*callback)( Resource* ) )
  {
    uint32 resourceId = resource->GetInstance();

    // Use cache if we have it in the cache or is already loading it
    Resource* cachedResource = GetResource( resource );
    if ( cachedResource != NULL )
    {
      // If it is the same resourceId but different filename, fail
      int cmp = strcmp( cachedResource->GetFilename(), resource->GetFilename() ) ; 
      if ( cmp == 0 )
      {
        cachedResource->Copy(resource);
        cachedResource->AddCallback(obj, callback);

        delete resource;
        resource = cachedResource;

        // Call the signal directly if the resource was already loaded
        if (resource->IsLoaded())
        {
          //        ((obj)->*callback)( resource );
          resource->OnDoneLoading();
        }
        else
        {
          m_LoadQueue[resourceId] = resource;
        }
        return;
      }
      else
      {
        //cachedResource->OnFailedLoading()
        RemoveResource( resourceId );
      }
    }

    resource->SetFlags( flags );
    resource->AddCallback( obj, callback );
    m_ResourceList[ resourceId ] = resource;
    m_LoadQueue[ resourceId ] = resource;
  }

protected:

  Resource*                       GetResource( Resource* resource );

  static CoreResourceManager*     s_Instance;
  std::map< uint32_t,Resource* >  m_ResourceList;
  std::map< uint32_t,Resource* >  m_LoadQueue;
  char*                           m_ResourcePath;
  char*                           m_UserPath;

#ifdef ANDROID
  AAssetManager*                  m_AssetManager;
#endif
};

#endif /// EVILENGINE_COREENGINE_CORERESOURCEMANAGER
