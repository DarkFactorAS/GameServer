/*************************************************************************************
* Project     : EvilEngine CoreLib 2012
* File        : String
* Description : A custom string class to make it easier to change the underlying type
*               used for strings. This class also has some utility functions to handle
*               comparing, finding and converting strings.
*
* Author      : Thor Richard Hansen
 *************************************************************************************/

#include "Precompile.h"
#include "CoreString.h"
#include "CoreLib/Utils/CoreUtils.h"

#include <algorithm>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

String String::zero = "";

String::String()
{
}

String::String( const char* otherString )
{
  m_String  = FormatString("%s", otherString).c_str();
}

String::String( std::string otherString )
{
  m_String  = otherString;
}

String::String( uint32 number )
{
  m_String  = FormatString("%d", number).c_str();
}

String::~String()
{
}


String& String::operator+=( const String& otherString )
{
  m_String += otherString.m_String; 
  return *this;
}
String& String::operator+=( const char* otherString )
{
  m_String += otherString; 
  return *this; 
}
String& String::operator+=( const char otherChar )
{
  m_String += otherChar; 
  return *this; 
}

String& String::operator=( const String& otherString )
{
  m_String = otherString.m_String; 
  return *this; 
}

String& String::operator=( const std::string& otherString )
{
  m_String = otherString; 
  return *this; 
}

String& String::operator=( const char* otherString )
{
  m_String = otherString;
  return *this; 
}

//String& String::operator<<(int i)
//{
//  //m_String << i;
//  return *this;
//}

String& String::operator<<(const String& otherString)
{
  m_String += otherString.m_String;
  return *this;
}

String String::GenerateRandomString( uint strLength )
{
  String retStr;
  // TODO : Check bounds on memory block
  while( retStr.length() < strLength )
  {
    char c = ( rand() % 123 ) + 65;
    if ( c >= 'A' && c <= 'Z' )
    {
      retStr += c;
    }
    else if ( c >= 'a' && c <= 'z' )
    {
      retStr += c;
    }
    else if ( c >= '0' && c <= '9' )
    {
      retStr += c;
    }
  }
  return retStr;
}

int String::CompareWithCase( const String& otherString, int maxLength ) const
{
  int i1 = 0;
  int i2 = 0;
  for ( ;; ) {
    if ( maxLength > 0 && i1 >= maxLength ) {
      return( 0 );
    }
    int c1 = m_String[ i1 ];
    i1 ++;
    int c2 = otherString.m_String[ i2 ];
    i2 ++;

    if ( c1 != c2 || c1 == 0 || c2 == 0 ) {
      return( c1 - c2 );
    }
  }
}

int String::CompareNoCase( const String& otherString, int maxLength ) const
{
  int i1 = 0;
  int i2 = 0;
  for ( ;; ) {
    if ( maxLength > 0 && i1 >= maxLength ) {
      return( 0 );
    }
    int c1 = m_String[ i1 ];
    i1 ++;
    int c2 = otherString.m_String[ i2 ];
    i2 ++;

    // Make sure both are uppwercase
    if ( c1 >= 'a' && c1 <= 'z' )
    {
      c1 -= ( 'a' - 'A' );
    }

    if ( c2 >= 'a' && c2 <= 'z' )
    {
      c2 -= ( 'a' - 'A' );
    }


    if ( c1 != c2 || c1 == 0 || c2 == 0 ) {
      return( c1 - c2 );
    }
  }
}

bool String::EqualsNoCase( const String& otherString ) const
{
  if ( CompareNoCase( otherString ) == 0 )
  {
    return true;
  }
  return false;
}

bool String::Equals( const String& otherString ) const
{
  if ( CompareWithCase( otherString ) == 0 )
  {
    return true;
  }
  return false;
}

bool String::EqualsWithCase( const String& otherString ) const
{
  if ( CompareWithCase( otherString ) == 0 )
  {
    return true;
  }
  return false;
}

bool String::StartsWithCase( const String& otherString ) const
{
  return ( CompareWithCase( otherString, otherString.Length() ) == 0);
}

bool String::EndsWithCase( const String& otherString ) const
{
  if ( m_String.length() >= otherString.length() )
  {
    String subString = SubString( (uint32) m_String.length() - otherString.length(), otherString.length() );
    return ( subString.CompareWithCase( otherString, otherString.Length() ) == 0);
  }
  return false;
}

bool String::StartsWithNoCase( const String& otherString ) const
{
  return ( CompareNoCase( otherString, otherString.Length() ) == 0);
}

char String::CharacterAt( size_t position ) const
{
  if ( position <= m_String.length() )
  {
    return m_String[ position ];
  }
  return 0;
}

/***********************************************************************************
* SubString:
* Returns a string that is a sub section of this string ( between start and end pos )
* 
* @param (int)     startPos - From ( and including ) position for the string
* @param (int)     endPos   - To ( and not including ) position for the string
* @return (String)          - The substring to return
* 
* @author Thor Richard Hansen
***********************************************************************************/
String String::SubString( uint32 startPos, uint32 numCharacters ) const
{
  if ( startPos >= Length() )
  {
    return String::zero;
  }

  if ( startPos + numCharacters > Length() )
  {
    numCharacters = Length() - startPos;
  }
  return String( m_String.substr( startPos, numCharacters ) );
}

String String::Remove( int startPos, int endPos ) const
{
  if ( startPos < endPos && startPos > -1 && endPos <= (int)Length() )
  {
    String str = m_String.substr( 0, startPos ) + m_String.substr( endPos, Length() - endPos );
    return str;
  }
  return m_String;
}

bool String::Replace( const String& searchString, const String& replaceString )
{
  // TODO : Check memory bounds

  bool replacedString = false;
  int pos = FindString( 0, searchString );
  while( pos != String::e_InvalidPos )
  {
    replacedString = true;

    m_String = m_String.substr( 0, pos ) 
      + replaceString.c_str()
      + m_String.substr( pos + searchString.length(), Length() - pos - searchString.length() );

    pos += replaceString.length();
    pos = FindString( pos, searchString );
  }
  return replacedString;
}

class WhitespacePredicate
{
public:
  bool operator() (const char& character) const { return (((character & 0x80) == 0) && isspace( character ) ); }
};

String& String::Trim()
{
  if (m_String.empty())
  {
    return (*this);
  }

  m_String.erase( std::remove_if (m_String.begin(), m_String.end(), WhitespacePredicate() ), m_String.end());
  return (*this);
}

String String::Trim() const
{
  String newString( *this );
  if (!newString.m_String.empty())
  {
    newString.m_String.erase( std::remove_if (newString.m_String.begin(), newString.m_String.end(), WhitespacePredicate() ), newString.m_String.end());
  }

  return newString;
}

String& String::ToLower()
{
  for ( size_t i = 0 ; i < m_String.size() ; ++i ) 
  {
    m_String[i] = static_cast<std::string::value_type>(tolower( m_String[i] ));
  }
  return( *this );
}

String& String::ToUpper()
{
  for ( size_t i = 0 ; i < m_String.size() ; ++i ) 
  {
    m_String[i] = static_cast<std::string::value_type>(toupper( m_String[i] ));
  }
  return( *this );
}

String& String::Reverse()
{
  std::string m_NewString = m_String;

  uint32 len = (uint32) m_String.size();
  for ( size_t i = 0 ; i < m_String.size() ; ++i ) 
  {
    m_String[i] = static_cast<std::string::value_type>(tolower( m_NewString[ len - i - 1 ] ));
  }
  return( *this );
}

String String::GetLower() const
{
  String str( m_String );
  for ( size_t i = 0 ; i < m_String.size() ; ++i ) 
  {
    str.m_String[i] = static_cast<std::string::value_type>(tolower( m_String[i] ));
  }
  return str;
}

String String::GetUpper() const
{
  String str( m_String );
  for ( size_t i = 0 ; i < m_String.size() ; ++i ) 
  {
    str.m_String[i] = static_cast<std::string::value_type>(toupper( m_String[i] ));
  }
  return str;
}

String String::AsHex() const
{
  String str;
  for ( size_t i = 0 ; i < m_String.size() ; ++i ) 
  {
    uint8 hex = m_String[i];
    str += String::FormatString("%.2x", hex );
    if ( i % 4 == 0 && i > 0 )
    {
      str += " ";
    }
  }
  return str;
}

String String::AsReadable() const
{
  String str;
  for ( size_t i = 0 ; i < m_String.size() ; ++i ) 
  {
    uint8 hex = m_String[i];
    if ( hex >= 30 && hex <= 123 )
    {
      str += hex;
    }
    else
    {
      str += String::FormatString("\\x%.2x", hex );
    }
  }
  return str;
}

String& String::Format( const char* pzFormat, va_list pArgs )
{
  int allocateBuffer = DEFAULT_FORMAT_SIZE;

#ifdef WINDOWS
  int maxLen = _vscprintf( pzFormat, pArgs );
  if ( maxLen > allocateBuffer )
  {
    allocateBuffer = CoreUtils::Pow2( maxLen );
  }
#endif

  char* pzBuffer = NULL;
  int len = -1;
  int numTries = 10;
  while ( ( len < 0 || len >= allocateBuffer ) && allocateBuffer < MAXCAP_FORMAT_SIZE && numTries > 0 )
  {
    free(pzBuffer);

    pzBuffer = new char[allocateBuffer];
    memset(pzBuffer, 0, allocateBuffer);

#ifdef WINDOWS
    len = vsnprintf_s(pzBuffer, allocateBuffer, allocateBuffer / sizeof(pzBuffer[0]), pzFormat, pArgs);
#else
    len = vsnprintf(pzBuffer, allocateBuffer, pzFormat, pArgs);
#endif

    numTries--;
    if (len < 0 || len >= allocateBuffer)
    {
      allocateBuffer = CoreUtils::Pow2(allocateBuffer + 1);
    }
  }

  if (len >= 0 && len < allocateBuffer)
  {
    m_String.assign( pzBuffer );
    free(pzBuffer);
  }
  else
  {
    dfBugreportFMT(false, "String buffer too small! (Wanted:%d, Is:%d)", len, allocateBuffer);
  }

  return( *this );
}

String& String::Format( const char* pzFormat, ... )
{
  va_list sArgList;
  va_start( sArgList, pzFormat );
  Format( pzFormat, sArgList );
  va_end( sArgList );
  return( *this );
}

String String::ForwardFormatString(const char* pzFormat, va_list sArgList )
{
  String cStr;
  cStr.Format(pzFormat, sArgList);
  return(cStr);
}

String String::FormatString( const char* pzFormat, ... )
{
  String cStr;
  va_list sArgList;
  va_start( sArgList, pzFormat );
  cStr.Format( pzFormat, sArgList );
  va_end( sArgList );

  return( cStr );
}

String String::Translate( const char* pzFormat, ... )
{
  String cStr;
  va_list sArgList;
  va_start( sArgList, pzFormat );
  cStr.Format( pzFormat, sArgList );
  va_end( sArgList );

  return( cStr );
}

String String::FromInt(int number)
{
  return String::FormatString("%d", number );
}

bool String::IsNumeric() const
{
  if ( m_String.empty() )
  {
    return false;
  }

  size_t i = 0;
  bool allowHex = false;
  if ( SubString( 0, 2 ) == "0x")
  {
    allowHex = true;
    i = 2;
  }

  for ( ; i < m_String.length(); i++ )
  {
    if ( m_String[i] >= '0' && m_String[i] <= '9' )
    {
      continue;
    }

    if ( allowHex && m_String[i] >= 'A' && m_String[i] <= 'F' )
    {
      continue;
    }

    if ( allowHex && m_String[i] >= 'a' && m_String[i] <= 'f' )
    {
      continue;
    }

    return false;
  }
  return true;
}

char* String::ToCharPointer() const
{
  char* cString = (char*)malloc( m_String.length() + 1 );
#ifdef PLATFORM_WINDOWS
  strcpy_s(cString, m_String.length() + 1, m_String.c_str());
#else
  strcpy(cString, m_String.c_str());
#endif
  return cString;
}

int String::ToInt( int base ) const
{
  int val = 0;
  if ( !m_String.empty() )
  {
    if ( base == 16 )
    {
#ifdef WINDOWS
      sscanf_s(m_String.c_str(), "%x", &val);
#else
      scanf(m_String.c_str(), "%x", &val);
#endif
    }
    else
    {
      val = atoi( m_String.c_str() );
    }
  }
  return val;
}

uint32 String::ToUInt32(int base) const
{
  uint32 val = 0;
  if (!m_String.empty())
  {
    if (base == 16)
    {
#ifdef WINDOWS
      sscanf_s(m_String.c_str(), "%x", &val);
#else
      scanf(m_String.c_str(), "%x", &val);
#endif
    }
    else
    {
      val = atoi(m_String.c_str());
    }
  }
  return val;
}

int String::FindChar( uint start, char findChar ) const
{
  if ( start > m_String.length() )
  {
    return -1;
  }

  for ( uint i = start; i < m_String.length(); i++ )
  {
    if ( m_String[i] == findChar )
    {
      return i;
    }
  }

  return e_InvalidPos;
}

int String::FindChar( String findStr ) const
{
  return FindChar( 0, findStr );
}

int String::FindChar( uint start, String findStr ) const
{
  if ( findStr.length() == 1 )
  {
    return FindChar( start, findStr.CharacterAt( 0 ) );
  }
  return e_InvalidPos;
}

int String::FindString( uint start, String findStr ) const
{
  if ( start > m_String.length() )
  {
    return e_InvalidPos;
  }

  if ( findStr.Length() == 1 )
  {
    return FindChar( start, findStr.CharacterAt(0) );
  }

  for ( uint i = start; i < m_String.length(); i++ )
  {
    bool foundStr = true;
    for ( uint j = 0; j < findStr.Length(); j++ )
    {
      if ( m_String[i+j] != findStr.m_String[j] )
      {
        foundStr = false;
        break;
      }
    }
    if ( foundStr )
    {
      return i;
    }
  }
  return e_InvalidPos;
}
