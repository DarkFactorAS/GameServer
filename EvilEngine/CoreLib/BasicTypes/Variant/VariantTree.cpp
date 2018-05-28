
#include "Precompile.h"
#include "VariantTree.h"

VariantTree::VariantTree()
{
  m_Parent = NULL;
}

VariantTree::~VariantTree()
{
  Clear();
  m_Parent = NULL;
}

void VariantTree::Clear()
{
  for ( uint32 index = 0; index < m_Children.size(); index++ )
  {
    delete m_Children[index];
  }
  m_Children.clear();
  for ( std::map< String, Variant* >::iterator itValue = m_KeyValueList.begin() ; itValue != m_KeyValueList.end(); ++itValue )
  {
    delete itValue->second;
  }
  m_KeyValueList.clear();
}

void VariantTree::SetValue(const String& key, Variant* var )
{
  String lowKey = key.GetLower();
  std::map< String, Variant* >::iterator itValue = m_KeyValueList.find(lowKey);
  if (itValue != m_KeyValueList.end())
  {
    Variant* oldVar = itValue->second;
    oldVar->Copy( *var );
    delete var;
  }
  else
  {
    m_KeyValueList[lowKey] = var;
  }
}

void VariantTree::SetUInt32(const String& key, uint32 intValue)
{
  SetValue(key, new Variant(intValue));
}
void VariantTree::SetString(const String& key, String stringValue)
{
  SetValue(key, new Variant(stringValue));
}
void VariantTree::SetBool(const String& key, bool boolValue)
{
  SetValue( key, new Variant(boolValue) );
}

Variant* VariantTree::GetValue(const String& key)
{
  String lowKey = key.GetLower();
  std::map< String, Variant* >::iterator itValue = m_KeyValueList.find(lowKey);
  if (itValue != m_KeyValueList.end())
  {
    return itValue->second;
  }
  return NULL;
}

uint32 VariantTree::GetUInt32(const String& key, uint32 defaultValue )
{
  Variant* variant = GetValue(key);
  if (variant != NULL)
  {
    return variant->GetUInt32();
  }
  return defaultValue;
}

const String& VariantTree::GetString(const String& key, const String& defaultValue)
{
  Variant* variant = GetValue(key);
  if (variant != NULL)
  {
    return variant->GetString();
  }
  return defaultValue;
}

bool VariantTree::GetBool(const String& key, bool defaultValue)
{
  Variant* variant = GetValue(key);
  if (variant != NULL)
  {
    return variant->GetBool();
  }
  return defaultValue;
}

void VariantTree::AddChild( VariantTree* tree )
{
  tree->SetParent( this );
  m_Children.push_back( tree );
}

VariantTree* VariantTree::GetChild( const String& childName )
{
  for ( std::vector< VariantTree* >::iterator itTree = m_Children.begin(); itTree != m_Children.end(); ++itTree )
  {
    VariantTree* child = *itTree;
    if ( child->GetName().Equals(childName) )
    {
      return child;
    }
  }
  return NULL;
}

void VariantTree::RemoveChild( VariantTree* tree )
{
  if ( tree->GetParent() == this )
  {
    tree->SetParent( NULL );
  }
  for ( std::vector< VariantTree* >::iterator itTree = m_Children.begin(); itTree != m_Children.end(); ++itTree )
  {
    VariantTree* child = *itTree;
    if ( child == tree )
    {
      m_Children.erase( itTree );
      return;
    }
  }
}

bool VariantTree::IsEmpty()
{
  return ( m_Children.empty() && m_KeyValueList.empty() );
}

String VariantTree::ToString()
{
  String str = GetName() + String::FormatString("[%d keys] [%d children]\n", m_KeyValueList.size(), m_Children.size() );
  for ( uint32 index = 0; index < m_Children.size(); index++ )
  {
    VariantTree* child = m_Children[ index ];
    str += String::FormatString(" => Child '%s'\n", child->GetName().c_str() );
  }
  for ( std::map< String, Variant* >::iterator itValue = m_KeyValueList.begin() ; itValue != m_KeyValueList.end(); ++itValue )
  {
    String keyName = itValue->first;
    str += String::FormatString(" => Key '%s'\n", keyName.c_str() );
  }
  return str;
}

String VariantTree::DumpTreeToString( String preFix )
{
  String str = preFix + ">" + GetName() + "\n";
  for ( std::map< String, Variant* >::iterator itValue = m_KeyValueList.begin() ; itValue != m_KeyValueList.end(); ++itValue )
  {
    String keyName = itValue->first;
    Variant* value = itValue->second;
    str += String::FormatString("%s=> Key '%s' = '%s'\n", preFix.c_str(), keyName.c_str(), value->ToString().c_str() );
  }
  for ( uint32 index = 0; index < m_Children.size(); index++ )
  {
    
    VariantTree* child = m_Children[ index ];
    str += child->DumpTreeToString( preFix + "==" );
  }
  return str;
}

String VariantTree::DumpTreeToString()
{
  String str = "\n";
  str += DumpTreeToString( "" );
  if ( str.length() > String::DEFAULT_FORMAT_SIZE )
  {
    str = str.SubString( 0, String::DEFAULT_FORMAT_SIZE );
  }
  return str;
}
