
#include "Precompile.h"
#include "XMLParser.h"
#include "EvilEngine/CoreIO/Logger/Logger.h"
#include "EvilEngine/Corelib/BasicTypes/String/CoreString.h"

XMLNode::XMLNode( const char* str, uint32 length ) : 
  m_Name(NULL)
{
  ParseArguments( str, length );
}

XMLNode::~XMLNode()
{
  for ( size_t index = 0; index < m_Children.size(); index++ )
  {
    delete m_Children[ index ];
  }
  m_Children.clear();
}

void XMLNode::SetName( char* name )
{
  if ( m_Name == NULL )
  {
    m_Name = name;
  }
}
void XMLNode::AddMember( char* memberKey, char* memberValue )
{
  m_Members.push_back( KeyValue(memberKey, memberValue ) );
}

void XMLNode::ParseArguments( const char* arguments, uint32 length )
{
  if ( arguments == NULL || length == 0 )
  {
    return;
  }

  uint32 mode       = XMLParser::SEARCHMODE_NONE;
  uint32 currentPos = 0;
  uint32 argStart   = 0;
  uint32 argEnd     = 0;
  char*  key        = NULL;

  while( currentPos < length )
  {
    char c = arguments[ currentPos ];
    switch ( c )
    {
      case '\"':
        {
          if ( ( mode & XMLParser::SEARCHMODE_QUOTE ) == XMLParser::SEARCHMODE_QUOTE )
          {
            argEnd = currentPos;
          }
          else
          {
            argStart = currentPos + 1;
          }
          mode ^= XMLParser::SEARCHMODE_QUOTE;
        }
        break;

      case '=':
        {
          if ( ( mode & XMLParser::SEARCHMODE_QUOTE ) == XMLParser::SEARCHMODE_QUOTE )
          {
            break;
          }

          if ( currentPos > argStart )
          {
            uint32 strLength = currentPos - argStart;

            key = (char*) malloc( strLength + 1 );
            key[ strLength ] = 0;
            memcpy( key, arguments + argStart, strLength );
            argStart = currentPos + 1;
            //LogInfo("XMLNode::Key", key );
          }
        }
        break;

      case ' ':
        {
          if ( ( mode & XMLParser::SEARCHMODE_QUOTE ) == XMLParser::SEARCHMODE_QUOTE )
          {
            break;
          }

          if ( argEnd <= argStart )
          {
            argEnd = currentPos;
          }

          if ( argEnd > argStart )
          {
            uint32 strLength = argEnd - argStart;

            char* str = (char*) malloc( strLength + 1 );
            str[ strLength ] = 0;
            memcpy( str, arguments + argStart, strLength );

            //LogInfo("XMLNode::Argument", str );

            if ( key == NULL )
            {
              SetName( str );
            }
            else
            {
              AddMember( key, str );
            }

            key       = NULL;
            argStart  = currentPos + 1;
            argEnd    = currentPos + 1;
          }
        }
        break;
    }
    currentPos++;
  }

  // Take the rest as the value
  if ( length > argStart + 2 )
  {
    uint32 strLength = length - argStart;
    char* str = (char*) malloc( strLength + 1 );
    str[ strLength ] = 0;
    memcpy( str, arguments + argStart, strLength );

    //LogInfo("XMLNode::EndArgument", str );

    if ( key == NULL )
    {
      SetName( str );
    }
    else
    {
      AddMember( key, str );
    }

    key       = NULL;
    argStart  = currentPos;
  }
}

XMLParser::XMLParser( const char* data, uint32 dataLength ) : 
  m_Data( data ),
  m_Length( dataLength )
{
  m_RootNode = new XMLNode( NULL, 0 );
  //m_RootNode->SetName( "ROOT" );
  ReadData( m_RootNode, 0, String::zero );
}

int XMLParser::ReadData( XMLNode* parentNode, uint32 startPos, String tagName )
{
  if ( startPos >= m_Length )
  {
    return -1;
  }

  SEARCHMODE mode   = SEARCHMODE_NONE;
  int currentPos    = startPos;
  uint32 startTag   = 0;
  //uint32 argTag     = 0;

  while( currentPos < (int)m_Length )
  {
    char c = m_Data[ currentPos ];

    switch( c )
    {
      case '<':
        {
          // Ignore < if we are not in this mode in case
          // we use it inside strings
          if ( mode == SEARCHMODE_NONE )
          {
            mode      = SEARCHMODE_STARTTAG;
            startTag  = currentPos + 1;
            //argTag    = startTag;
          }
        }
        break;

      case '/':
        {
          if ( mode == SEARCHMODE_STARTTAG )
          {
            mode = SEARCHMODE_ENDTAG;
            startTag  = currentPos + 1;
          }
        }
        break;

      case '>':
        {
          if ( mode != SEARCHMODE_STARTTAG && mode != SEARCHMODE_ENDTAG )
          {
            break;
          }

          // Fetch the text and parse it!
          uint32 length = currentPos - startTag;
          if ( length > 0 )
          {
            char* str = (char*) malloc( length + 1 );
            str[ length ] = 0;
            memcpy( str, m_Data + startTag, length );

            if ( mode == SEARCHMODE_STARTTAG )
            {
              //LogInfo("XMLParser::StartTag", str );

              mode = SEARCHMODE_NONE;
              XMLNode* childNode = new XMLNode( str, length );
              parentNode->AddChild( childNode );
              currentPos = ReadData( childNode, currentPos + 1, childNode->GetName() );
              if ( currentPos == -1 )
              {
                return -1;
              }
            }
            else if ( mode == SEARCHMODE_ENDTAG )
            {
              //if ( tagName.CompareWithCase( childName ) == 0 )
              {
                return currentPos;
              }

              //LogWarningFMT("XMLParser", "Broken endtag '%s'", str );
              //return -1;
            }
          }
        }
        break;

      default:
        break;
    }
    currentPos++;
  }

  //LogWarning("XMLParser", "Ended parsing of file" );
  return -1;
}
