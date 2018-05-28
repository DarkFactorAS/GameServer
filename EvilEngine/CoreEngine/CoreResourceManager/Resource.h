#ifndef EVILENGINE_CORERENDERING_CORERESOURCEMANAGER_RESOURCE
#define EVILENGINE_CORERENDERING_CORERESOURCEMANAGER_RESOURCE 1

#include "EvilEngine/CoreLib/Signal/Signal.h"

struct ResourceRef
{
  long      m_Filesize;
  uint8_t*  m_Data;
  ResourceRef( long fileSize, uint8_t* data ){ m_Filesize = fileSize; m_Data = data; }
};

class Resource
{
public:

  enum RESOURCE_FLAGS
  {
    RESOURCEFLAG_NONE           = 0,
    RESOURCEFLAG_CACHE          = 1,
    RESOURCEFLAG_PARENTCACHE    = 2,
    RESOURCEFLAG_DELETE         = 3,
  };

  enum ResourceType
  {
    ResourceType_Shader,
    ResourceType_Png,
    ResourceType_Bmp,
    ResourceType_Texture,
    ResourceType_AtlasTexture,
    ResourceType_FontAtlasTexture,
    ResourceType_Gui,
  };

  Resource( int resourceType, int resourceId, const char* filename );
  Resource( int resourceType, const char* filename );
  virtual ~Resource();

  virtual bool                        LoadData() = 0;
  virtual bool                        ReloadData(){ return false; };
  virtual void                        Copy( const Resource* /*other*/ ){};

  uint32                              GetId() const{ return m_ResourceId;}
  uint32                              GetResourceId() const{ return m_ResourceId;}
  int                                 GetType() const{ return m_ResourceType; }
  uint32                              GetInstance() const { return m_ResourceId;}
  void                                SetInstance( uint32 instanceId ){ m_ResourceId = instanceId; }
  const char*                         GetFilename() const { return m_Filename; }
  virtual bool                        UniqueByName() const{ return false; }

  bool                                HasData(){ return  ( m_Data != NULL ); }
  bool                                IsLoaded() const{ return m_IsLoaded; }
  void                                SetLoaded( bool isLoaded ){ m_IsLoaded = isLoaded; }
  virtual void                        OnDoneLoading();

  void                                AddFlag( int flag ){ m_Flags |= flag; }
  void                                SetFlags( uint32 flags ){ m_Flags = flags; }
  bool                                HasFlag( uint32 flags ){ return ( ( m_Flags & flags ) == flags ); }
  void                                ClearFlag( int flag ){ m_Flags &= ~flag; }
  bool                                ShouldCache();

  // Signal when the resource is done loading
  template < typename CALLBACK_OBJECT, typename OBJ_CLASS >
  void                                AddCallback( CALLBACK_OBJECT* obj, void ( OBJ_CLASS::*callback)( Resource* resource ) )
  {
    m_SignalResourceReady.Disconnect( obj, callback );
    m_SignalResourceReady.Connect( obj, callback );
  }

  ResourceRef*                        m_RawMemory;

protected:

  ResourceRef*                        LoadFileToMemory( const char * filename );
  bool                                SaveMemoryToFile( const char * filename );

  bool                                m_IsLoaded;
  uint8_t*                            m_Data;
  char*                               m_Filename;

  static int                          s_ResourceId;
  uint32                              m_ResourceId;
  int                                 m_ResourceType;
  uint32                              m_Flags;

private:

  Signal1< void, Resource* >          m_SignalResourceReady;
};

#endif
