
#include "Precompile.h"
#include "ConfigReader.h"
#include "../File/IOFile.h"
#include "EvilEngine/CoreLib/BasicTypes/String/StringList.h"

ConfigReader* ConfigReader::m_Instance = NULL;

ConfigReader::ConfigReader( const String& fileName ) :
  m_FileName(fileName),
  m_HasErrors( false ),
  m_Comment( 0 )
{
  m_File = new IOFile();
  if( m_File->OpenRead( fileName ) )
  {
    String line;
    ReadConfig( line, 0, String::zero, String::zero );

    if (m_KeyMap.empty())
    {
      AddError("Keymap is empty");
    }
  }
  else
  {
    AddError(String::FormatString("Failed to open file %s", fileName.c_str()));
  }
}

ConfigReader* ConfigReader::CreateInstance(const String& fileName)
{
  m_Instance = new ConfigReader( fileName );
  return m_Instance;
}

int ConfigReader::ReadConfig( String& line, int startPos, String keyPath, String tagName )
{
  String key = tagName;
  if ( !keyPath.IsEmpty() )
  {
    key = String::FormatString("%s/%s",keyPath.c_str(),tagName.c_str());
  }

  int failSafe = 100;

  while( failSafe > 0 )
  {
    failSafe--;
    if ( startPos >= (int)line.Length() )
    {
      // We cannot read more from the file
      if ( m_File->IsEOF() )
      {
        return -1;
      }
      String newLine = m_File->ReadLine();
      if ( newLine.IsEmpty() )
      {
        return -1;
      }
      line += newLine;
    }

    int startTag = line.FindChar( startPos, '<' );
    if ( startTag != -1 )
    {
      int endTag        = line.FindChar( startTag + 1, ">");
      int closeTag      = line.FindChar( startTag + 1, "/");

      if ( endTag == -1 )
      {
        return -1;
      }

      if ( closeTag > endTag || closeTag == -1 )
      {
        // This is a start tag, so process it
        String tag = line.SubString( startTag + 1, endTag - startTag - 1 );

        // Check if this is a comment
        if ( tag.StartsWithCase("!--") )
        {
          m_Comment++;
          startPos = endTag + 1;
        }
        // Check if the comment ends
        else if ( m_Comment > 0 && tag.EndsWithCase("--") )
        {
          m_Comment--;
          startPos = endTag + 1;
        }
        else if ( tag.StartsWithCase("?") )
        {
          // Ignore
          startPos = endTag + 1;
        }
        else
        {
          // If we have multiple entries inside this tag, use the
          // first one as the tagname. The other ones are actually values.
          StringList strList = StringList::Split( tag, " " );
          if ( strList.size() > 1 )
          {
            tag = strList.at( 0 );
          }

          int retPos = ReadConfig( line, endTag + 1, key, tag );
          if ( retPos == -1 )
          {
            return -1;
          }
          startPos = retPos;
        }
      }
      else
      {
        // Check if we have the start tag for this tag
        String tag    = line.SubString( closeTag + 1, endTag - closeTag - 1);
        if ( !tag.EqualsNoCase( tagName ) )
        {
          AddError(String::FormatString("Starttag for %s not found", tag.c_str()));
          return -1;
        }

        // Add the key
        if ( startPos < startTag && m_Comment == 0 )
        {
          String value  = line.SubString( startPos, startTag - startPos );
          m_Keys[ key ] = value;

          // Add the key to the short to full key lookup
          std::map<String,String>::const_iterator it = m_KeyMap.find( tag );
          if ( it != m_KeyMap.end() )
          {
            // If we have multiple keys with the same shortname, just disable the lookup
            m_KeyMap[ tag ] = String::zero;
          }
          else
          {
            m_KeyMap[ tag ] = key;
          }
        }

        // Return the position after the end of the close tag
        return endTag + 1;
      }
    }
  }
  return -1;
}

const String& ConfigReader::GetErrorMessage()
{
  if (m_HasErrors)
  {
    return m_ErrorReason;
  }

  return String::zero;
}

void ConfigReader::AddError(const String& reason)
{
  if (!m_HasErrors)
  {
    m_ErrorReason = "ConfigReader Error : ";
    m_HasErrors = true;
  }
  m_ErrorReason += reason;
}

const String& ConfigReader::GetFullKey( const String& key )
{
  std::map<String,String>::const_iterator it = m_KeyMap.find( key );
  if ( it != m_KeyMap.end() )
  {
    const String& fullKey = it->second;
    if ( !fullKey.IsEmpty() )
    {
      return fullKey;
    }
  }
  return key;
}

uint32 ConfigReader::GetIntKey( const String& key, uint32 defaultValue )
{
  const String& fullKey = GetFullKey( key );
  std::map<String,String>::const_iterator it = m_Keys.find( fullKey );
  if ( it != m_Keys.end() )
  {
    const String& str = it->second;
    return str.ToInt();
  }
  return defaultValue;
}

String ConfigReader::GetStringKey( const String& key, const String& defaultValue )
{
  const String& fullKey = GetFullKey( key );
  std::map<String,String>::const_iterator it = m_Keys.find( fullKey );
  if ( it != m_Keys.end() )
  {
    return it->second;
  }
  return defaultValue;
}

/************************************************************************************************
* GetBoolKey:
* Return true if the value if the field is different from false/FALSE. 
*
* @param  (String) key          - Name of the key to get value from
* @param  (bool) defaultValue   - The value to return of the entry was not found
* @return (bool)                - Return the value of the key
*
* @author Thor Richard Hansen
*************************************************************************************************/
bool ConfigReader::GetBoolKey(const String& key, bool defaultValue)
{
  const String& fullKey = GetFullKey(key);
  std::map<String, String>::const_iterator it = m_Keys.find(fullKey);
  if (it != m_Keys.end())
  {
    const String& str = it->second;
    return ( !str.CompareNoCase("false") );
  }
  return defaultValue;
}
