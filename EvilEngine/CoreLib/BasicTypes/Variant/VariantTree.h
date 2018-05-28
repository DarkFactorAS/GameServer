#ifndef EVILENGINE_CORELIB_VARIANTTREE
#define EVILENGINE_CORELIB_VARIANTTREE 1

#include <map>
#include <vector>

#include "EvilEngine/CoreLib/BasicTypes/String/CoreString.h"
#include "EvilEngine/CoreLib/BasicTypes/Variant/Variant.h"

class IOFile;

class VariantTree
{
public:

  VariantTree();
  ~VariantTree();

  void                              Clear();

  const String&                     GetName() const{ return m_Name; }
  void                              SetName( const String& name ){ m_Name = name; }

  std::vector< VariantTree* >&      GetGroupList(){ return m_Children; }
  std::vector< VariantTree* >&      GetChildList(){ return m_Children; }
  std::map< String, Variant* >&     GetKeyValueList(){ return m_KeyValueList; }

  void                              SetValue( const String& key, Variant* var );
  void                              SetUInt32( const String& key, uint32 var );
  void                              SetInt32(const String& key, int32 var);
  void                              SetString( const String& key, String var );
  void                              SetBool( const String& key, bool var );

  Variant*                          GetValue( const String& key );
  uint32                            GetUInt32(const String& key, uint32 defaultValue = 0 );
  int32                             GetInt32(const String& key, int32 defaultValue = 0);
  const String&                     GetString(const String& key, const String& defaultValue = String::zero);
  bool                              GetBool(const String& key, bool defaultValue = false);

  bool                              IsEmpty();
  VariantTree*                      GetParent()const { return m_Parent; }
  void                              SetParent( VariantTree* parent ){ m_Parent = parent; }

  void                              AddChild( VariantTree* tree );
  void                              RemoveChild( VariantTree* tree );
  VariantTree*                      GetChild( const String& childName );

  String                            ToString();
  String                            DumpTreeToString();
  String                            DumpTreeToString( String preFix );

private:

  String                            m_Name;
  VariantTree*                      m_Parent;

  std::map< String, Variant* >      m_KeyValueList;
  std::vector< VariantTree* >       m_Children;

};

#endif /// EVILENGINE_CORELIB_VARIANTTREE
