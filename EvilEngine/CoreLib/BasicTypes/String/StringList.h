#ifndef CUSTOM_STRINGLIST_H
#define CUSTOM_STRINGLIST_H

#include "CoreString.h"
#include <vector>

class StringList
{
public:
  typedef std::vector<String, std::allocator<String> >::const_iterator const_iterator;
  typedef std::vector<String, std::allocator<String> >::iterator iterator;

  StringList(){}
  StringList( const StringList& otherList );

  //String  operator[]( int i ) const{ return NULL; }
  bool    operator!=( iterator /* otherIterator */ ) const{ return true; }
  bool    operator!=( const_iterator /* otherIterator */ ) const{ return true; }
  StringList&       operator+=( const StringList& otherList );
  StringList&       operator=( const StringList& otherList );

  String  at( int i ) const { return m_StringList.at( i ); }
  String  last() const { return m_StringList.at( size() - 1 ); }
  bool    removeAt( uint i);
  void    erase( iterator it ){ m_StringList.erase(it); }
  void    push_back( String string ){ m_StringList.push_back(string); }
  void    insert( iterator it, String string ){ m_StringList.insert(it,string); }
  void    sort();
  void    clear(){ m_StringList.clear(); }
  bool    empty() const{ return m_StringList.empty(); }
  size_t  size() const { return m_StringList.size(); }
  iterator begin() { return m_StringList.begin(); }
  iterator end() { return m_StringList.end(); }
  const_iterator begin() const { return m_StringList.begin(); }
  const_iterator end() const { return m_StringList.end(); }

  /// Joins a StringList into one string with the seperator
  String Join(String spacer) const;
  /// Joins a StringList into one string with the seperator
  static String Join(const StringList& strList, String spacer );
  /// Breaks a string seperated with splitChar up in multiple parts 
  static StringList Split( const String& originalString, char splitChar );
  /// Breaks a string seperated with a splitString up in multiple parts
  static StringList Split( const String& originalString, const String& splitString );
  /// Splits the sentance based on max characters pr line.
  static StringList WordWrap( const String& originalString, uint32 maxCharacters );

  static StringList zero;

private:
  std::vector<String> m_StringList;
};

#endif /// CUSTOM_STRINGLIST_H
