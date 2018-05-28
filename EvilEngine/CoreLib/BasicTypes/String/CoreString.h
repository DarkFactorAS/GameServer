
#ifndef CUSTOM_STRING_H
#define CUSTOM_STRING_H

#include <string>
#include <stdarg.h>
#include "../Definitions/BasicTypes.h"

class String
{
public:

  enum 
  {
    e_InvalidPos    = -1,
    DEFAULT_FORMAT_SIZE = 4096,
    MAXCAP_FORMAT_SIZE = 1024000,
  };

  String();
  String( const char* );
  String( std::string );
  String( uint32 );
  ~String();

  static String zero;

  static String GenerateRandomString( uint strLength );

  /// Return the length of the string
  uint32 Length() const { return (uint32) m_String.length(); }
  uint32 length() const { return (uint32) m_String.length(); }

  /// Returns true if the string is empty
  bool IsEmpty() const { return m_String.empty(); }
  bool empty() const { return m_String.empty(); }
  /// Returns true if the value in the string only contains numbers
  bool IsNumeric() const;

  /// Compares the string with the matching characters and casing
  int CompareWithCase( const String& otherString, int maxLength = -1 ) const;
  /// Compare the string with the matching characters regardless of casing
  int CompareNoCase( const String& otherString, int maxLength = -1 ) const;
  /// Returns true if the two strings are equal
  bool EqualsNoCase( const String& otherString ) const;
  /// Returns true if the two strings are equal
  bool EqualsWithCase( const String& otherString ) const;
  /// Returns true if the two strings are equal
  bool Equals( const String& otherString ) const;
  /// Returns true if the string starts with a specific string ( with correct casing )
  bool StartsWith( const String& otherString ) const { return StartsWithCase( otherString); }
  bool StartsWithCase( const String& otherString ) const;
  /// Returns true if the string ends with a specific string ( with correct casing )
  bool EndsWith( const String& otherString ) const{ return EndsWithCase( otherString ) ;}
  bool EndsWithCase( const String& otherString ) const;
  /// Returns true if the string starts with a specific string ( regardless of casing )
  bool StartsWithNoCase( const String& otherString ) const;
  /// Find the position of a specific character
  int FindChar( uint start, char findChar ) const;
  int FindChar( uint start, String findStr ) const;
  int FindChar( String findStr ) const;

  /// Return the position of a specific character
  int FindString( uint start, String findStr ) const;
  /// Return the character at a specific position
  char CharacterAt( size_t position ) const;

  /// Return a copy of the string as char*
  char* ToCharPointer() const;

  /// Return the int value of the string. 0 if it isnt numeric
  int     ToInt( int base = 10) const;
  uint32  ToUInt32(int base = 10) const;

  /// Return the char* to the string
  const char* c_str() const { return m_String.c_str(); }
  std::string stdstr() const{ return m_String; }
  const char* GetQString() const { return m_String.c_str(); }

  /// Erase the entire string
  void Clear(){ m_String.clear(); }
  /// Remove whitespaces at the end of this string
  String& Trim();
  /// Returns a string without the ending whitespaces
  String Trim() const;
  /// Converts the string to lowercase and returns it
  String& ToLower();
  /// Converts the string to uppercase and returns it
  String& ToUpper();
  /// Reverse the string
  String& Reverse();
  /// Converts the string to lowercase and returns it
  String GetLower() const;
  /// Converts the string to uppercase and returns it
  String GetUpper() const;
  /// Return the string as hex formatted string
  String AsHex() const;
  /// Return the string as a readable format ( non-normal characters are showns in hex numbers, the rest is as is).
  String AsReadable() const;
  /// Return the substring
  String SubString( uint32 startPos, uint32 endPos ) const;
  /// Removes the text between startpos and endpos
  String Remove( int startPos, int endPos ) const;
  /// Removes the text between startpos and endpos
  bool Replace( const String& searchString, const String& replaceString );
  /// Format this string with the arguments
  String& Format( const char* pzFormat, va_list pArgs );
  /// Format this string with the arguments
  String& Format( const char* pzFormat, ... );
  /// Return a formatted string with the arguments filled in
  static String ForwardFormatString( const char* pzFormat, va_list sArgList);
  /// Return a formatted string with the arguments filled in
  static String FormatString(const char* pzFormat, ...);
  /// Temp function to wrap all translations for now.
  static String Translate( const char* pzFormat, ... );

  static String FromInt( int number );

  /// Iterators
  typedef std::string::iterator iterator;
  iterator begin(){ return m_String.begin(); }
  iterator end(){ return m_String.end(); }

  /// Stream operators
  String& operator  <<  ( int i );
  String& operator  <<  ( const String& otherString );

  /// Operators
  bool    operator  ==  ( const String& otherString ) const{ return Equals(otherString); }
  String& operator  =   ( const String& otherString );
  String& operator  =   ( const std::string& otherString );
  String& operator  =   ( const char*  otherString );
  bool    operator  !=  ( const String& otherString ) const{ return (CompareNoCase(otherString) != 0); }
  bool    operator  <   ( const String& otherString ) const{ return m_String < otherString.m_String; }
  String& operator  +=  ( const String& otherString );
  String& operator  +=  ( const char* otherString );
  String& operator  +=  ( const char otherChar );

private:
  std::string m_String;
};

class KeyValue
{
public:
  KeyValue(String key, String value) :
    m_Key(key),
    m_Value(value)
  {
  }
  String m_Key;
  String m_Value;
};


inline const String operator+(const String& s1, const String& s2)
{
  String t(s1);
  t += s2;
  return t;
}

inline const String operator+(const String& s1, const char* s2)
{
  String t(s1);
  t += s2;
  return t;
}

#if defined( WINDOWS ) || (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 3))
template<int T>
static const String& StaticStrImpl(const char* i_pzStr)
{
  static String cStr(i_pzStr);
  return cStr;
}

#define StaticStr( a ) StaticStrImpl<__COUNTER__>(a)
#else // defined( WINDOWS ) || (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 3))

#define StaticStr( a ) String(a)

#endif // defined( WINDOWS ) || (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 3))



#endif //CUSTOM_STRING_H
