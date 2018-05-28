
#include "Precompile.h"
#include "UserConfig.h"

String UserConfig::s_ApplicationName = "DfApp";
String UserConfig::s_ConfigPath = "";

UserConfig::UserConfig( const String& configName )
{
  m_File        = NULL;
  m_RootNode    = new VariantTree();
  m_RootNode->SetName( configName );
  m_CurrentNode = m_RootNode;
}

UserConfig::~UserConfig()
{
  delete m_RootNode;
  m_RootNode    = NULL;
  m_CurrentNode = NULL;
}

void UserConfig::SetConfigPath( const String path )
{
  s_ConfigPath = path;
}

void UserConfig::SetUInt32( const String& key, uint32 intValue )
{
  m_CurrentNode->SetUInt32( key, intValue );
}
void UserConfig::SetString(const String& key, const String& stringValue)
{
  m_CurrentNode->SetString(key, stringValue);
}
void UserConfig::SetBool(const String& key, bool fieldValue)
{
  m_CurrentNode->SetBool(key, fieldValue);
}

Variant* UserConfig::GetValue(const String& key)
{
  return m_CurrentNode->GetValue(key);
}
uint32 UserConfig::GetUInt32(const String& key, uint32 defaultValue)
{
  return m_CurrentNode->GetUInt32(key, defaultValue);
}
const String& UserConfig::GetString(const String& key, const String& defaultValue)
{
  return m_CurrentNode->GetString(key, defaultValue);
}
bool UserConfig::GetBool(const String& key, bool defaultValue)
{
  return m_CurrentNode->GetBool(key, defaultValue);
}

bool UserConfig::EnterGroup( String childName )
{
  VariantTree* child = m_CurrentNode->GetChild( childName );
  if ( child != NULL )
  {
    m_CurrentNode = child;
    return true;
  }
  return false;
}

bool UserConfig::LeaveGroup()
{
  VariantTree* parent = m_CurrentNode->GetParent();
  if ( parent != NULL )
  {
    m_CurrentNode = parent;
    return true;
  }
  return false;
}

void UserConfig::AddChild( VariantTree* tree )
{
  m_CurrentNode->AddChild( tree );
}

bool UserConfig::DeleteChild(String childName)
{
  VariantTree* child = m_CurrentNode->GetChild(childName);
  if (child != NULL)
  {
    return DeleteChild( child );
  }
  return false;
}

bool UserConfig::DeleteChild( VariantTree* node )
{
  bool deletedChild = PlatformDeleteChild( node );
  m_CurrentNode->RemoveChild(node);
  return deletedChild;
}
