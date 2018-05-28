#ifndef EVILENGINE_CORELIB_XMLPARSER
#define EVILENGINE_CORELIB_XMLPARSER 1

#include <map>
#include <vector>

#include "../BasicTypes/Variant/Variant.h"

class XMLNode
{
public:

  XMLNode( const char* name, uint32 length );
  ~XMLNode();

  void                                              AddChild( XMLNode* child ){ m_Children.push_back( child ); }

  const char*                                       GetName() const { return m_Name; }
  void                                              SetName( char* name );
  void                                              AddMember( char* memberKey, char* memberValue );

  const std::vector< XMLNode* >&                    GetChildList(){ return m_Children; }
  const std::vector< KeyValue >&                    GetMembers(){ return m_Members; }

private:

  void ParseArguments( const char* arguments,  uint32 length );

  const char*                                       m_Name;
  std::vector< KeyValue >                           m_Members;
  std::vector< XMLNode* >                           m_Children;
};

class XMLParser
{
public:

  enum SEARCHMODE
  {
    SEARCHMODE_NONE       = 0,
    SEARCHMODE_STARTTAG   = 1,
    SEARCHMODE_ENDTAG     = 2,
    SEARCHMODE_NAME       = 4,
    SEARCHMODE_QUOTE      = 8,
  };

  XMLParser( const char* memory, uint32 dataLength );

  XMLNode*    GetRootNode() const { return m_RootNode; }

private:

  int         ReadData( XMLNode* node, uint32 startPos, String tagName );

  const char* m_Data;
  uint32      m_Length;
  XMLNode*    m_RootNode;
};

#endif /// EVILENGINE_CORELIB_XMLPARSER
