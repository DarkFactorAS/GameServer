/*************************************************************************************************
 * Project     : EvilEngine Multiplatform 2014
 * File        : UserConfig_iOS
 *
 * Description : Save/Load code for the iOS implementation of the user config system. On the
 *               iOS platform all user config entries are stored in files that are on internal
 *               storage space.
 *
 * Author      : Thor Richard Hansen
 *************************************************************************************************/

#include "Precompile.h"

#if defined(PLATFORM_LINUX) || defined(PLATFORM_OSX)

#include "UserConfig.h"
#include "EvilEngine/CoreIO/File/IOFile.h"
#include "EvilEngine/CoreLib/BasicTypes/String/StringList.h"
#include "EvilEngine/CoreIO/Logger/Logger.h"
#include <stdlib.h>

bool UserConfig::Load()
{
  m_File = new IOFile();
  String fileName = s_ConfigPath + "/" + m_RootNode->GetName();
  if ( !m_File->OpenRead( fileName ) )
  {
    LogErrorFMT("UserConfigLoad:","Could not open file %s", fileName.c_str() );
    return false;
  }
  bool loadChildren = LoadChildren( m_RootNode, String::zero );
  m_File->Close();

  LogInfoFMT("UserConfig","Loaded tree : \n%s", m_RootNode->DumpTreeToString().c_str() );
  return loadChildren;
}

bool UserConfig::LoadChildren( VariantTree* parentNode, String parentPath )
{
  bool isLoaded = true;
  while( !m_File->IsEOF() )
  {
    String newLine = m_File->ReadLine();
    if ( newLine.empty() )
    {
      break;
    }
    if ( newLine.CharacterAt(0) == '>' )
    {
      VariantTree* child = new VariantTree();
      child->SetName( newLine.SubString( 1, newLine.Length() ) );
      LogInfoFMT("UserConfigLoad:[C]",">%s", child->GetName().c_str());
      parentNode->AddChild( child );
      isLoaded = LoadChildren(child, String::zero) & isLoaded;
    }
    else if ( newLine.CharacterAt(0) == '<')
    {
      //LogInfo("UserConfigLoad:[C]","<");
      return isLoaded;
    }
    else
    {
      //LogInfoFMT("UserConfigLoad:[K]","%s", newLine.c_str());
      StringList strList = StringList::Split( newLine, "|");
      if ( strList.size() != 3 )
      {
        strList = StringList::Split(newLine, ":");
        if (strList.size() != 3)
        {
          LogInfoFMT("UserConfigLoad:[K]", "Failed to parse '%s' ( num words : %d )", newLine.c_str(), strList.size());
          return false;
        }
      }

      int variantType = atoi( strList.at( 0 ).c_str() );
      String key      = strList.at( 1 );
      String value    = strList.at( 2 );
      switch( variantType )
      {
      case Variant::STRING:
        {
          parentNode->SetString( key, value );
        }
        break;

      case Variant::INT:
        {
          uint32 num = atoi( value.c_str() );
          parentNode->SetUInt32( key, num );
        }
        break;

      case Variant::BOOLVALUE:
        {
          //Variant* var = new Variant();
          uint32 num = atoi( value.c_str() );
          parentNode->SetBool( key, num != 0 );
        }
        break;
      }
    }
  }
  return isLoaded;
}

bool UserConfig::Save()
{
  m_File = new IOFile();
  String fileName = s_ConfigPath + "/" + m_RootNode->GetName();
  if ( !m_File->OpenWrite( fileName ) )
  {
    LogErrorFMT("UserConfigSave:","Could not open file %s", fileName.c_str() );
    return false;
  }
  bool ret = SaveChildren( m_RootNode, String::zero );
  m_File->Close();
  return ret;
}

bool UserConfig::SaveChildren( VariantTree* parentNode, String parentPath )
{
  std::map< String, Variant* >& keyValueList = parentNode->GetKeyValueList(); 
  for ( std::map< String, Variant* >::const_iterator itEntry = keyValueList.begin(); itEntry != keyValueList.end(); ++itEntry )
  {
    String key = itEntry->first;
    //key = key.Replace(":","");

    Variant* fieldValue = itEntry->second;
    switch( fieldValue->GetType() )
    {
    case Variant::STRING:
      {
        String varStr = fieldValue->GetString();
        //varStr = varStr.Replace(":", "");
        String strValue = String::FormatString("%d|%s|%s\n", Variant::STRING, key.c_str(), varStr.c_str() );
        m_File->WriteString( strValue );
        //LogInfo( "UserConfigSave:[K]", strValue );
      }
      break;

    case Variant::INT:
    case Variant::SHORT:
      {
        String strValue = String::FormatString("%d|%s|%d\n", fieldValue->GetType(), key.c_str(), fieldValue->GetUInt32() );
        m_File->WriteString( strValue );
        //LogInfo( "UserConfigSave:[K]", strValue );
      }
      break;

    case Variant::FLOAT:
      {
        String strValue = String::FormatString("%d|%s|%0.2f\n", Variant::FLOAT, key.c_str(), fieldValue->GetFloat() );
        m_File->WriteString( strValue );
        //LogInfo( "UserConfigSave:[K]", strValue );
      }
      break;

    case Variant::BOOLVALUE:
      {
        String strValue = String::FormatString("%d|%s|%d\n", Variant::BOOLVALUE, key.c_str(), fieldValue->GetUInt32() );
        m_File->WriteString( strValue );
        //LogInfo( "UserConfigSave:[K]", strValue );
      }
      break;

    case Variant::NONE:
      break;
    }
  }

  // Save all children
  bool saveChildren = true;
  std::vector< VariantTree* >& childList = parentNode->GetChildList();
  for (uint32 index = 0; index < childList.size(); index++)
  {
    VariantTree* child = childList[ index ];

    String objName = ">" + child->GetName() + "\n";
    m_File->WriteString( objName );
    //LogInfo( "UserConfigSave:[C]", objName );

    if ( !SaveChildren( child, String::zero ) )
    {
      saveChildren = false;
    }
    m_File->WriteString( "<\n" );
    //LogInfo( "UserConfigSave:[C]", "<" );
  }

  return saveChildren;
}

bool UserConfig::Delete()
{
  return DeleteChildren( m_CurrentNode );
}

bool UserConfig::DeleteChildren( VariantTree* parentNode )
{
  // Delete all keys
  std::map< String, Variant* >& keyList = parentNode->GetKeyValueList();
  for ( std::map< String, Variant* >::const_iterator itEntry = keyList.begin(); itEntry != keyList.end(); ++itEntry )
  {
    delete itEntry->second;
  }
  keyList.clear();

  // Delete all children
  std::vector< VariantTree* >& childList = parentNode->GetChildList();
  for (uint32 index = 0; index < childList.size(); index++)
  {
    delete childList[ index ];
  }
  childList.clear();
  return true;
}

bool UserConfig::DeleteChildren( const String& /*parentKey*/ )
{
  return false;
}

bool UserConfig::PlatformDeleteChild(VariantTree* /*node*/)
{
  return false;
}

bool UserConfig::Remove( const String& configName )
{
  return remove( configName.c_str() );
}

bool UserConfig::DeleteGroup()
{
  return false;
}

#endif

