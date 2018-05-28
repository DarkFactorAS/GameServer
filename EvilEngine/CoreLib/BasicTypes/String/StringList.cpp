/*************************************************************************************
* Project     : EvilEngine CoreLib 2012
* File        : StringList
* Description : A custom string class to handle a list (vector) of strings. This class
*               also has some utility functions to handle splitting/joing/sorting of 
*               the strings.
*               
* Author      : Thor Richard Hansen
 *************************************************************************************/

#include "Precompile.h"
#include "StringList.h"

StringList StringList::zero;

/***********************************************************************************
* Constructor:
* Constructs a new stringList with the same list as otherString.
* 
* @param (String)         otherString    - The stringlist to create this one from
* 
* @author Thor Richard Hansen
***********************************************************************************/
StringList::StringList( const StringList& otherList )
{
  m_StringList = otherList.m_StringList;
}

/***********************************************************************************
* Operator +=
* Adds additional strings to the list from the otherList StringList
* 
* @param (StringList)      otherList    - The StringList to  copy from
* @return (StringList)                    Returns a reference to this StringList
* 
* @author Thor Richard Hansen
***********************************************************************************/
StringList& StringList::operator+=( const StringList& otherList )
{
  for ( uint i=0; i< otherList.size(); i++)
  {
    m_StringList.push_back( otherList.at(i) );
  }
  return *this;
}

StringList& StringList::operator=( const StringList& otherList )
{
  m_StringList=otherList.m_StringList; 
  return *this; 
}

bool StringList::removeAt( uint removeIndex )
{
  uint i = 0;
  for ( iterator it = begin(); it != end(); ++it )
  {
    if ( i == removeIndex )
    {
      m_StringList.erase( it );
      return true;
    }
  }
  return false;
}

void StringList::sort()
{
  StringList otherList;

  for ( const_iterator it = begin(); it != end(); ++it )
  {
    String string = *it;
    if ( otherList.empty() )
    {
      otherList.push_back( string );
    }
    else
    {
      for ( iterator it2 = otherList.begin(); it2 != otherList.end(); ++it2 )
      {
        if ( *it < string )
        {
          otherList.insert( it2, string );
          break;
        }
      }
    }
  }

  m_StringList = otherList.m_StringList;
}


/***********************************************************************************
* WordWrap:
* Will split the string for every occurance of the splitChar. The different parts
* of the string is added in a StringList
* 
* @param (String)         originalString    - The string we want to split into multiple parts
* @param (char)           splitChar         - The character used to split the string with
* @return (StringList)                      - Returns a new stringlist with the split string
* 
* @author Thor Richard Hansen
***********************************************************************************/
StringList StringList::WordWrap( const String& originalString, uint32 maxCharacters )
{
  uint startSplit     = 0;
  uint endSplit       = 0;
  StringList stringList;
  bool lastWasSpace  = false;

  for ( uint i = 0; i < originalString.Length(); i++ )
  {
    if ( originalString.CharacterAt( i ) == ' ' )
    {
      if ( !lastWasSpace )
      {
        endSplit = i;
        lastWasSpace = true;
      }
    }
    else if ( i - startSplit > maxCharacters )
    {
      lastWasSpace = false;
      String word = originalString.SubString( startSplit, endSplit - startSplit );
      stringList.push_back(word);
      startSplit = endSplit + 1;
    }
    else
    {
      lastWasSpace = false;
    }
  }

  // Push the remaining to the list
  if ( startSplit < originalString.Length() )
  {
    String word = originalString.SubString( startSplit, originalString.Length() );
    //if ( !word.Trim().empty() )
    {
      stringList.push_back(word);
    }
  }

  return stringList;
}

/***********************************************************************************
* Join:
* Joins all the strings in the stringlist into one string with a seperator character
*
* @param (String)         spacer     - The delimiter to use between the strings
* @return (String)                   - Returns a new string with the joined elements
*
* @author Thor Richard Hansen
***********************************************************************************/
String StringList::Join(String spacer) const
{
  return StringList::Join( *this, spacer );
}

/***********************************************************************************
* Join:
* Joins all the strings in the stringlist into one string with a seperator character
* 
* @param (StringList)     strList    - The list of strings to join
* @param (String)         spacer     - The delimiter to use between the strings
* @return (String)                   - Returns a new string with the joined elements
* 
* @author Thor Richard Hansen
***********************************************************************************/
String StringList::Join( const StringList& strList, String spacer )
{
  String retString;
  std::vector<String> m_StringList;
  for ( uint32 index = 0; index < strList.m_StringList.size(); index++ )
  {
    if ( index > 0 )
    {
      retString += spacer;
    }
    if ( !strList.m_StringList[ index ].empty() )
    {
      retString += strList.m_StringList[ index ];
    }
  }
  return retString;
}

/***********************************************************************************
* Split:
* Will split the string for every occurance of the splitChar. The different parts
* of the string is added in a StringList
* 
* @param (String)         originalString    - The string we want to split into multiple parts
* @param (char)           splitChar         - The character used to split the string with
* @return (StringList)                      - Returns a new stringlist with the split string
* 
* @author Thor Richard Hansen
***********************************************************************************/
StringList StringList::Split( const String& originalString, char splitChar )
{
  uint lastIndex = 0;
  StringList stringList;

  for ( uint i = 0; i < originalString.Length(); i++ )
  {
    if ( originalString.CharacterAt( i ) == splitChar )
    {
      if ( lastIndex < i )
      {
        String word = originalString.SubString( lastIndex, i - lastIndex );
        stringList.push_back(word);
      }
      lastIndex = i + 1;
    }
  }

  // Push the remaining to the list
  if ( lastIndex < originalString.Length() )
  {
    String word = originalString.SubString( lastIndex, originalString.Length() );
    stringList.push_back(word);
  }
  else if ( !originalString.empty() )
  {
    stringList.push_back("");
  }

  return stringList;
}

/***********************************************************************************
* Split:
* Will split the string for every occurance of the splitString. The different parts
* of the string is added in a StringList
* 
* @param (String)         originalString    - The string we want to split into multiple parts
* @param (String)         splitString       - The string used to split the string with
* @return (StringList)                      - Returns a new stringlist with the split string
* 
* @author Thor Richard Hansen
***********************************************************************************/
StringList StringList::Split( const String& originalString, const String& splitString )
{
  uint32 splitStrLen = splitString.Length();

  if ( splitStrLen == 0 )
  {
    StringList stringList;
    stringList.push_back(originalString);
    return stringList;
  }

  // The one character split is faster
  if ( splitStrLen == 1 )
  {
    return Split( originalString, splitString.CharacterAt( 0 ) );
  }

  StringList stringList;
  uint lastIndex = 0;

  uint32 strLen = originalString.Length();
  for (uint i = 0; i < strLen; )
  {
    if ( i + splitStrLen <= strLen )
    {
      // Check if the substring matches
      bool wasMatch = true;
      for ( uint32 j = 0; j < splitStrLen; j++ )
      {
        if ( wasMatch && originalString.CharacterAt( i + j ) != splitString.CharacterAt( j ) )
        {
          wasMatch = false;
        }
      }

      if ( wasMatch )
      {
        if (lastIndex < i)
        {
          String word = originalString.SubString(lastIndex, i - lastIndex);
          stringList.push_back(word);
        }
        lastIndex = i + splitStrLen;
        i += splitStrLen;
        continue;
      }
    }
    i++;
  }

  // Push the remaining to the list
  if (lastIndex < originalString.Length())
  {
    String word = originalString.SubString(lastIndex, originalString.Length());
    stringList.push_back(word);
  }
  else if (!originalString.empty())
  {
    stringList.push_back("");
  }

  return stringList;
}
