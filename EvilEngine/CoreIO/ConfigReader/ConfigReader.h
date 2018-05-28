#ifndef EVILENGINE_COREIO_CONFIGREADER_H
#define EVILENGINE_COREIO_CONFIGREADER_H

#include <map>

class IOFile;

class ConfigReader
{
public:
  ConfigReader( const String& fileName );

  /// Singleton accessors
  static ConfigReader*    CreateInstance( const String& fileName );
  static ConfigReader*    GetInstanceIfAny(){ return m_Instance; }

  /// Return sht ename of the config file
  const String&           GetFilename() const { return m_FileName; }

  /// Returns true if this config is valid
  bool                    IsValid(){ return !m_HasErrors; }
  /// Return the error message
  const String&           GetErrorMessage();
  /// Returns the int value ( if possible ) for the given key
  uint32                  GetIntKey( const String& key, uint32 defaultValue = 0 );
  /// Returns te String value for the given key
  String                  GetStringKey( const String& key, const String& defaultValue = "" );
  /// Returns the bool value ( if possible ) for the given key
  bool                    GetBoolKey( const String& key, bool defaultValue = false );

private:

  /// Reads the config file from disk and parses it
  int                     ReadConfig( String& line, int startPos, String keyPath, String tag );
  /// Returns the full path to a key from the shortname
  const String&           GetFullKey( const String& key );
  /// Add reason the config failed
  void                    AddError( const String& reason );
  /// Pointer to the file we are reading the config from
  IOFile*                 m_File;
  /// filename
  String                  m_FileName;
  /// True if the config file is not valid XML
  bool                    m_HasErrors;
  /// Reason for failure
  String                  m_ErrorReason;
  /// True if we are in a comment block
  int                     m_Comment;
  /// Singleton instance
  static ConfigReader*    m_Instance;
  /// List of all the Key/value pairs
  std::map<String,String> m_Keys;
  /// List of all the shortnames to full name keys
  std::map<String,String> m_KeyMap;

};

#endif /// EVILENGINE_COREIO_CONFIGREADER_H