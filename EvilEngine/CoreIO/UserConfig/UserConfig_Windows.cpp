/*************************************************************************************************
 * Project     : EvilEngine Multiplatform 2014
 * File        : UserConfig_Windows
 *
 * Description : Save/Load code for the windows implementation of the user config system. On the
 *               Windows platform all user config entries are stored in the registry under the
 *               HKEY_CURRENT_USER\Software\Darkfactor\<ConfigName>. The configname is usually the
 *               name of the game or application.
 *
 * Author      : Thor Richard Hansen
 *************************************************************************************************/

#include "PreCompile.h"

#ifdef WINDOWS
#define MAX_VALUE_LENGHT 4096
#define MAX_KEY_LENGHT 64

#include "UserConfig.h"
#include "windows.h"
#include <winreg.h>
#include "EvilEngine/CoreIO/Logger/Logger.h"
#include "EvilEngine/CoreLib/BasicTypes/String/StringList.h"
#include "EvilEngine/CoreLib/Assert/DFAssert.h"

bool UserConfig::CreatePath( const String& path )
{
  HKEY hKey;
  long error = RegOpenKeyEx(HKEY_CURRENT_USER, path.c_str(), 0, KEY_ALL_ACCESS, &hKey);
  switch( error )
  {
    case ERROR_SUCCESS:
      return true;
    case ERROR_FILE_NOT_FOUND:
    case ERROR_NO_MATCH:
      {
        error = RegCreateKeyEx(HKEY_CURRENT_USER, path.c_str(), 0L, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL );
        return ( error == ERROR_SUCCESS );
      }
    default:
      return false;
  }
}

bool UserConfig::Load()
{
  m_CurrentNode->Clear();
  String rootKey = String::FormatString("Software\\%s\\%s", COMPANY_NAME, s_ApplicationName.c_str() );
  return LoadChildren( m_RootNode, rootKey );
}

bool UserConfig::LoadChildren( VariantTree* parentNode, String parentPath )
{
  HKEY hKey;

  // Create the path
  String thisPath = parentPath;
  if ( parentNode->GetName().IsEmpty() )
  {
    return false;
  }
  thisPath += "\\";
  thisPath += parentNode->GetName();

  // Check if root key exist
  long error = RegOpenKeyEx(HKEY_CURRENT_USER, thisPath.c_str(), 0, KEY_ALL_ACCESS, &hKey);
  if ( error != ERROR_SUCCESS )
  {
    return false;
  }

  // Fetch all subkeys
  DWORD numSubKeys;
  DWORD maxSubkeyLen; 
  DWORD numValues;      
  DWORD maxValueLen;  
  DWORD retCode;      

  RegQueryInfoKey(hKey,
    NULL,            // buffer for class name
    NULL,            // size of class string
    NULL,            // reserved
    &numSubKeys,     
    &maxSubkeyLen,   
    NULL,            // longest class string 
    &numValues,       
    &maxValueLen,    
    NULL,            // longest value data 
    NULL,            // security descriptor 
    NULL);            // last write time

  if( numValues > 0 )
  {
    LPSTR keyName   = new CHAR[ MAX_KEY_LENGHT ];
    char* dataValue = new char[ MAX_VALUE_LENGHT ];
    DWORD keyType   = 0;

    for( uint32 i=0; i < numValues;i++)
    {
      DWORD keyNameLength = MAX_KEY_LENGHT;
      DWORD dataSize      = MAX_VALUE_LENGHT;

      retCode = RegEnumValue(hKey, i, keyName, &keyNameLength, NULL, &keyType, (LPBYTE) dataValue, &dataSize);
      if( retCode == ERROR_SUCCESS )
      {
        switch( keyType )
        {
        case REG_SZ:
          {
            char* b = new char[ dataSize ];
            memset( b, 0, dataSize + 1 );
            memcpy( b, dataValue, dataSize );
            String str( b );
            //Variant* var = new Variant();
            //var->SetString( str );
            //m_KeyValueList[ lowKeyName.GetLower() ] = var;
            parentNode->SetString( keyName, str );
          }
          break;

        case REG_DWORD:
          {
            DWORD* val = (DWORD*) dataValue;

            //Variant* var = new Variant();
            //var->SetUInt32( *val );
            //m_KeyValueList[ lowKeyName.GetLower() ] = var;

            parentNode->SetUInt32( keyName, *val);
          }
          break;

        default:
          LogErrorFMT( "UserConfig_Windows", "Unknown registry key type %d for key '%s'", keyType, keyName );
          break;
        }
      }
      else if ( error == ERROR_MORE_DATA )
      {
        LogFatalFMT( "UserConfig", "Failed to load registry key [index:%d] from %s. Buffer too small ( wanted:%d, max is:%d ) ", i, thisPath.c_str(), dataSize, MAX_VALUE_LENGHT );
      }
    }
  }

  bool loadSuccessful = true;
  if ( numSubKeys > 0 )
  {
    LPSTR keyName = new CHAR[ MAX_KEY_LENGHT ];
    FILETIME ftLastWriteTime;

    for( uint32 i=0; i < numSubKeys;i++)
    {
      DWORD keyNameLen = MAX_KEY_LENGHT;

      retCode = RegEnumKeyEx (hKey, i, keyName, &keyNameLen, NULL,NULL, NULL, &ftLastWriteTime);
      if( retCode == ERROR_SUCCESS )
      {
        char* b = new char[ keyNameLen + 1 ];
        memset( b, 0, keyNameLen + 1 );
        memcpy( b, keyName, keyNameLen );

        VariantTree* child = new VariantTree();
        child->SetName( b );
        parentNode->AddChild( child );
        bool loadedChild = LoadChildren( child, thisPath );
        loadSuccessful = loadSuccessful & loadedChild;
      }
      else if ( error == ERROR_MORE_DATA )
      {
        LogFatalFMT("UserConfig", "Failed to load registry key [index:%d] from %s. Buffer too small ( wanted:%d, max is:%d )", i, thisPath.c_str(), keyNameLen, MAX_KEY_LENGHT );
      }
    }
  }

  RegCloseKey(hKey); 
  return loadSuccessful;
}

bool UserConfig::SaveChildren( VariantTree* parentNode, String parentPath )
{
  // Create the path
  String thisPath = parentPath;
  if ( parentNode->GetName().IsEmpty() )
  {
    return false;
  }
  thisPath += "\\";
  thisPath += parentNode->GetName();
  if ( !CreatePath( thisPath ) )
  {
    return false;
  }

  // Sanity check that the path exists
  HKEY hKey;
  long error = RegOpenKeyEx(HKEY_CURRENT_USER, thisPath.c_str(), 0, KEY_ALL_ACCESS, &hKey);
  if ( error != ERROR_SUCCESS )
  {
    LogFatalFMT("UserConfig", "Could not read newly created config path (%s)!", thisPath.c_str() );
    return false;
  }

  // Save all keys
  std::map< String, Variant* >& keyList = parentNode->GetKeyValueList();
  for ( std::map< String, Variant* >::const_iterator itEntry = keyList.begin(); itEntry != keyList.end(); ++itEntry )
  {
    Variant* fieldValue = itEntry->second;

    switch( fieldValue->GetType() )
    {
    case Variant::STRING:
      {
        if ( fieldValue->GetString().length() > MAX_VALUE_LENGHT )
        {
          RegSetValueEx(hKey, itEntry->first.c_str(),0,REG_SZ,(LPBYTE) fieldValue->GetString().SubString(0, MAX_VALUE_LENGHT).c_str(), fieldValue->GetString().length() );
        }
        else
        {
          RegSetValueEx(hKey, itEntry->first.c_str(),0,REG_SZ,(LPBYTE) fieldValue->GetString().c_str(), fieldValue->GetString().length() );
        }
      }
      break;
    case Variant::INT:
    case Variant::BOOLVALUE:
      {
        DWORD value = fieldValue->GetUInt32();
        RegSetValueEx(hKey, itEntry->first.c_str(),0,REG_DWORD,(LPBYTE) &value , sizeof( value ) );
      }
      break;
    default:
      continue;
    }
  }
  RegCloseKey(hKey);

  // Save all children
  bool saveChildren = true;
  std::vector< VariantTree* >& childList = parentNode->GetChildList();
  for (uint32 index = 0; index < childList.size(); index++)
  {
    VariantTree* child = childList[ index ];
    if ( !SaveChildren( child, thisPath ) )
    {
      saveChildren = false;
    }
  }
  return saveChildren;
}

bool UserConfig::Save()
{
  String nodePath = ResolveNodePath();

  HKEY hKey;
  String rootPath = String::FormatString("Software\\%s\\%s%s", COMPANY_NAME, s_ApplicationName.c_str(), nodePath.c_str() );

  String checkPath = "";
  StringList strList = StringList::Split( rootPath, "\\" );
  for ( size_t index = 0; index < strList.size(); index++ )
  {
    const String& key = strList.at( index );
    if ( index > 0 )
    {
      checkPath += "\\";
    }
    checkPath += key;

    if ( !CreatePath( checkPath ) )
    {
      return false;
    }
  }

  long error = RegOpenKeyEx(HKEY_CURRENT_USER, rootPath.c_str(), 0, KEY_ALL_ACCESS, &hKey);
  if ( error != ERROR_SUCCESS )
  //if ( error == ERROR_NO_MATCH )
  {
    //error = RegCreateKeyEx(HKEY_LOCAL_MACHINE, registryPath.c_str(), 0L, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL );
    return false;
  }

  return SaveChildren( m_CurrentNode, rootPath);
}

bool UserConfig::PlatformDeleteChild(VariantTree* childNode)
{
  if (!DeleteChildren(childNode))
  {
    return false;
  }

  HKEY hKey;
  String nodePath = ResolveNodePath(childNode->GetParent());
  String rootPath = String::FormatString("Software\\%s\\%s%s", COMPANY_NAME, s_ApplicationName.c_str(), nodePath.c_str());
  long error = RegOpenKeyEx(HKEY_CURRENT_USER, rootPath.c_str(), 0, KEY_ALL_ACCESS, &hKey);
  if (error != ERROR_SUCCESS)
  {
    return true;
  }

  bool deletedChildren = true;
  long delResult = RegDeleteKey(hKey, childNode->GetName().c_str());
  if (delResult != ERROR_SUCCESS)
  {
    LogErrorFMT("UserConfig_Windows", "Could not delete registry key '%s'", childNode->GetName().c_str());
    deletedChildren = false;
  }

  RegCloseKey(hKey);

  return deletedChildren;

}

bool UserConfig::Delete()
{
  return DeleteChildren(m_CurrentNode);
}

bool UserConfig::DeleteChildren( VariantTree* parentNode )
{
  // Find the registry path
  HKEY hKey;
  String nodePath = ResolveNodePath( parentNode );
  String rootPath = String::FormatString("Software\\%s\\%s%s", COMPANY_NAME, s_ApplicationName.c_str(), nodePath.c_str() );
  long error = RegOpenKeyEx(HKEY_CURRENT_USER, rootPath.c_str(), 0, KEY_ALL_ACCESS, &hKey);
  if ( error != ERROR_SUCCESS )
  {
    return true;
  }

  // Delete all keys
  std::map< String, Variant* >& keyList = parentNode->GetKeyValueList();
  for ( std::map< String, Variant* >::const_iterator itEntry = keyList.begin(); itEntry != keyList.end(); ++itEntry )
  {
    const String keyName = itEntry->first;
    long delResult = RegDeleteValue( hKey, keyName.c_str() );
    if ( delResult != ERROR_SUCCESS )
    {
      LogErrorFMT( "UserConfig_Windows", "Could not delete registry key '%s'", keyName.c_str() );
    }
    delete itEntry->second;
  }
  keyList.clear();

  bool deletedChildren = true;
  std::vector< VariantTree* >& childList = parentNode->GetChildList();
  for (uint32 index = 0; index < childList.size(); index++)
  {
    VariantTree* child = childList[ index ];
    bool deletedChild = DeleteChildren( child );
    deletedChildren = deletedChildren & deletedChild;

    long delResult = RegDeleteKey( hKey, child->GetName().c_str() );
    if ( delResult != ERROR_SUCCESS )
    {
      LogErrorFMT( "UserConfig_Windows", "Could not delete registry key '%s'", child->GetName().c_str() );
      deletedChildren = false;
    }
    delete child;
  }
  childList.clear();

  RegCloseKey(hKey);

  // Delete keys
  return deletedChildren;
}

bool UserConfig::DeleteChildren( const String& parentKey )
{
  HKEY hKey;
  long error = RegOpenKeyEx(HKEY_CURRENT_USER, parentKey.c_str(), 0, KEY_ALL_ACCESS, &hKey);
  if ( error != ERROR_SUCCESS )
  {
    return true;
  }

  // Fetch all subkeys
  DWORD numSubKeys;
  DWORD maxSubkeyLen; 
  DWORD numValues;      
  DWORD maxValueLen;  
  DWORD retCode;      

  RegQueryInfoKey(hKey,
    NULL,            // buffer for class name
    NULL,            // size of class string
    NULL,            // reserved
    &numSubKeys,     
    &maxSubkeyLen,   
    NULL,            // longest class string 
    &numValues,       
    &maxValueLen,    
    NULL,            // longest value data 
    NULL,            // security descriptor 
    NULL);            // last write time

  if( numValues > 0 )
  {
    //char currentSubkey[MAX_PATH];
    LPSTR valueName = new CHAR[ 64 ];
    char* dataValue = new char[ 200 ];
    DWORD dataType  = 0;

    for( uint32 i=0; i < numValues;i++)
    {
      DWORD dataSize   = 200;
      DWORD valNameLen = MAX_PATH;

      retCode = RegEnumValue(hKey, i, valueName, &valNameLen, NULL, &dataType, (LPBYTE) dataValue, &dataSize);
      if( retCode == ERROR_SUCCESS )
      {
        long delResult = RegDeleteKey( hKey, valueName );
        if ( delResult != ERROR_SUCCESS )
        {
          LogErrorFMT( "UserConfig_Windows", "Could not delete registry key type %d for key '%s'", dataType, valueName );
          RegCloseKey(hKey);
          return false;
        }
      }
    }
  }
  RegCloseKey(hKey); 
  return true;
}

bool UserConfig::DeleteGroup()
{
  String nodePath  = ResolveNodePath();
  if ( nodePath.empty() )
  {
    return false;
  }
  String registryPath = String::FormatString("Software\\%s\\%s\\%s", COMPANY_NAME, s_ApplicationName.c_str(), nodePath.c_str() );

  // Check if the key actually exist
  HKEY hKeyRoot;
  long error = RegOpenKeyEx(HKEY_CURRENT_USER, registryPath.c_str(), 0, KEY_ALL_ACCESS, &hKeyRoot);
  if (error != ERROR_SUCCESS)
  {
    return false;
  }

  DeleteChildren(registryPath);

  // Delete this the node for this child
  long lResult = RegDeleteKey(hKeyRoot, registryPath.c_str());
  if (lResult != ERROR_SUCCESS)
  {
    return false;
  }
  return true;
}

bool UserConfig::Remove( const String& configName )
{
  String rootKey = String::FormatString("Software\\%s\\%s", COMPANY_NAME, s_ApplicationName.c_str() );
  String parentKey = String::FormatString("%s\\%s", rootKey.c_str(), configName.c_str() );

  // Call children has to be deleted first
  if ( !DeleteChildren( parentKey ) )
  {
    return false;
  }

  // Check if the key actually exist
  HKEY hKeyRoot;
  long error = RegOpenKeyEx(HKEY_CURRENT_USER, rootKey.c_str(), 0, KEY_ALL_ACCESS, &hKeyRoot);
  if ( error != ERROR_SUCCESS )
  {
    return true;
  }

  long lResult = RegDeleteKey( hKeyRoot, configName.c_str() );
  if ( lResult != ERROR_SUCCESS )
  {
    return false;
  }
  return true;
}

String UserConfig::ResolveNodePath( VariantTree* node )
{
  String str = "";
  if ( node != NULL )
  {
    if ( node->GetParent() != NULL )
    {
      str += ResolveNodePath( node->GetParent() );
    }
    str += "\\";
    str += node->GetName();
  }
  return str;
}

String UserConfig::ResolveNodePath()
{
  return ResolveNodePath( m_CurrentNode->GetParent() );
}

#endif