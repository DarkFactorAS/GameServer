#ifndef EVILENGINE_COREIO_USERCONFIG
#define EVILENGINE_COREIO_USERCONFIG 1

#include <map>
#include <vector>

#include "EvilEngine/CoreLib/BasicTypes/String/CoreString.h"
#include "EvilEngine/CoreLib/BasicTypes/Variant/Variant.h"
#include "EvilEngine/CoreLib/BasicTypes/Variant/VariantTree.h"

class IOFile;

class UserConfig
{
public:
  UserConfig( const String& configName );
  ~UserConfig();

  uint32                        GetUInt32( const String& key, uint32 defaultValue = 0 );
  const String&                 GetString( const String& key, const String& defaultValue = String::zero );
  bool                          GetBool( const String& key, bool defaultValue = false );

  void                          SetUInt32( const String& key, uint32 fieldValue );
  void                          SetString( const String& key, const String& fieldValue );
  void                          SetBool( const String& key, bool fieldValue );

  static void                   SetApplicationName( const String& applicationName ){ s_ApplicationName = applicationName; }

  bool                          Load();
  bool                          Save();
  bool                          Delete();
  static bool                   Remove( const String& configName );
  bool                          CreatePath( const String& path );
  static void                   SetConfigPath( const String path );

  bool                          EnterGroup( String group );
  bool                          LeaveGroup();
  bool                          DeleteGroup();

  void                          AddChild( VariantTree* tree );
  bool                          DeleteChild(String childName);
  bool                          DeleteChild(VariantTree* node);

  VariantTree*                  GetConfigTree() const { return m_RootNode;}

private:

  // OS/Platform specific implementation of storing the config values in registry/file etc
  bool                          PlatformDeleteChild(VariantTree* node);


  bool                          LoadChildren( VariantTree* parentNode, String parentPath );
  bool                          SaveChildren( VariantTree* parentNode, String parentPath );
  bool                          DeleteChildren( VariantTree* parentNode );
  static bool                   DeleteChildren(const String& parentKey);
  Variant*                      GetValue( const String& key );
  String                        ResolveNodePath();
  String                        ResolveNodePath( VariantTree* node );

  //String                        m_ConfigName;
  static String                 s_ApplicationName;

  VariantTree*                  m_RootNode;
  VariantTree*                  m_CurrentNode;

  static String                 s_ConfigPath;

  IOFile*                       m_File;
};

#endif /// EVILENGINE_COREIO_USERCONFIG
