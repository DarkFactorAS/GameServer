
#include "Precompile.h"
#include "Resource.h"
#include "CoreResourceManager.h"
#include "EvilEngine/CoreIO/Logger/Logger.h"

#include <stdlib.h>

int Resource::s_ResourceId = 0;

Resource::Resource( int resourceType, int resourceId, const char* filename ) : 
  m_RawMemory( NULL ),
  m_IsLoaded( false ),
  m_Data( NULL ),
  //m_Filename( filename ),
  m_ResourceId( resourceId ),
  m_ResourceType( resourceType ),
  m_Flags( 0 )
{
  uint32 len = strlen(filename);
  m_Filename = new char[ len + 1 ];
  m_Filename[ len ] = 0;
  memcpy(m_Filename, filename, len);
}

Resource::Resource( int resourceType, const char* filename ) : 
  m_RawMemory( NULL ),
  m_IsLoaded( false ),
  m_Data( NULL ),
  //m_Filename( filename ),
  m_ResourceId( 0 ),
  m_ResourceType( resourceType ),
  m_Flags( 0 )
{
  m_ResourceId = ++s_ResourceId;

  uint32 len = strlen(filename);
  m_Filename = new char[len + 1];
  m_Filename[len] = 0;
  memcpy(m_Filename, filename, len);
}

Resource::~Resource()
{ 
  free(m_Data);
  if ( m_RawMemory != NULL )
  {
    free( m_RawMemory->m_Data );
  }
}

ResourceRef* Resource::LoadFileToMemory( const char * filename )
{
  CoreResourceManager* resourceManager = CoreResourceManager::GetInstance();
  if ( resourceManager != NULL )
  {
    ResourceRef* ref = resourceManager->LoadResource( filename );
    if ( ref != NULL )
    {
      return ref;
    }
  }

  dfAssertFMT( false, "Failed to load file '%s'", filename );
  return NULL;
}

bool Resource::SaveMemoryToFile( const char * filename )
{
  CoreResourceManager* resourceManager = CoreResourceManager::GetInstance();
  if ( resourceManager != NULL )
  {
    return resourceManager->SaveResource( filename, m_RawMemory );
  }
  return false;
}

void Resource::OnDoneLoading()
{
  m_SignalResourceReady( this );
}

bool Resource::ShouldCache()
{
  return ( HasFlag( RESOURCEFLAG_CACHE ) || HasFlag( RESOURCEFLAG_PARENTCACHE ) );
}
