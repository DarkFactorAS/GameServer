/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : GuiLoader
*
* Description : Resource that loads an xml file, parses it and generates a gui object out of it.
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "GuiResource.h"
#include "EvilEngine/CoreLib/XMLParser/XMLParser.h"
#include "EvilEngine/CoreGuiToolkit/BaseGuiElements/CoreGuiBase.h"
#include "EvilEngine/CoreGuiToolkit/CoreGuiToolkit.h"
#include "EvilEngine/CoreIO/Logger/Logger.h"

GuiResource::GuiResource( CoreGuiBase* parent, const char * filename ) : 
  Resource( ResourceType_Gui, filename ),
  m_ParsedData( NULL )
{
  if ( parent != NULL )
  {
    m_Parents.push_back( parent );
    parent->m_SignalOnDeath.Connect( this, &GuiResource::OnParentDeath );
  }
}

GuiResource::GuiResource( CoreGuiBase* parent, uint32 resourceInstance, const char * filename ) : 
  Resource( ResourceType_Gui, resourceInstance, filename ),
  m_ParsedData( NULL )
{
  if ( parent != NULL )
  {
    m_Parents.push_back( parent );
    parent->m_SignalOnDeath.Connect( this, &GuiResource::OnParentDeath );
  }
}

GuiResource::GuiResource( CoreGuiBase* parent, const char * filename, ResourceRef* memory ) :
  Resource( ResourceType_Gui, filename ),
  m_ParsedData( NULL )
{
  m_RawMemory       = memory;

  if ( parent != NULL )
  {
    m_Parents.push_back( parent );
    parent->m_SignalOnDeath.Connect( this, &GuiResource::OnParentDeath );
  }
}

GuiResource::~GuiResource()
{
}

void GuiResource::Copy( const Resource* other )
{
  //const GuiResource* guiResource = dynamic_cast< const GuiResource* > ( other );
  const GuiResource* guiResource = (const GuiResource*) ( other );
  if ( guiResource == NULL )
  {
    return;
  }

  // Copy the parents from the other object
  for ( std::vector< CoreGuiBase* >::const_iterator itParent = guiResource->m_Parents.begin(); itParent != guiResource->m_Parents.end(); ++itParent )
  {
    CoreGuiBase* parent = *itParent;
    m_Parents.push_back( parent );
    parent->m_SignalOnDeath.Connect( this, &GuiResource::OnParentDeath );

    m_SignalResourceReady.Copy( guiResource->m_SignalResourceReady );
  }
}

bool GuiResource::LoadData()
{
  if ( m_ParsedData != NULL )
  {
    delete m_ParsedData;
    m_ParsedData = NULL;
  }

  if ( m_RawMemory == NULL )
  {
#if !defined PLATFORM_IOS && !defined PLATFORM_OSX

    size_t filenameLength = strlen( m_Filename );
    char* guiFileName = (char*) malloc( filenameLength + 5 );
    if ( guiFileName != NULL )
    {
      memcpy( guiFileName, "Gui/", 4 );
      memcpy( guiFileName + 4, m_Filename, filenameLength );
      memset( guiFileName + 4 + filenameLength, 0, 1 );
    }
    m_RawMemory = LoadFileToMemory( guiFileName );
#else
    const char* guiFileName = m_Filename;
    m_RawMemory = LoadFileToMemory( m_Filename );
#endif
      
    if ( m_RawMemory == NULL )
    {
      dfBugreportFMT( m_RawMemory != NULL, "Failed to load gui xml file %s", guiFileName );
      return false;
    }

    m_ParsedData = new XMLParser( (const char*) m_RawMemory->m_Data, (uint32) m_RawMemory->m_Filesize );
    delete m_RawMemory;
    m_RawMemory = NULL;
  }
  return true;
}

bool GuiResource::ReloadData()
{
  //for ( std::vector< CoreGuiBase* >::iterator itParent = m_Parents.begin(); itParent != m_Parents.end(); ++itParent )
  //{
  //  CoreGuiBase* parent = *itParent;
  //  parent->RemoveAllChildren();
  //}

  delete m_ParsedData;
  m_ParsedData = NULL;

  return LoadData();
}

void GuiResource::OnDoneLoading()
{
  // Spawn gui for all the parents
  for ( std::vector< CoreGuiBase* >::iterator itParent = m_Parents.begin(); itParent != m_Parents.end(); ++itParent )
  {
    SpawnGui( *itParent );
  }

  Resource::OnDoneLoading();
}

void GuiResource::OnParentDeath( const CoreBaseObject* parentObject )
{
  // TODO Thorh 05.05.2015 - Remove this when reloading of the XML resources works properly
  //ClearFlag( RESOURCEFLAG_CACHE );
  //ClearFlag( RESOURCEFLAG_PARENTCACHE );
  //AddFlag( RESOURCEFLAG_DELETE );

  // Remove parent
  for ( std::vector< CoreGuiBase* >::iterator itParent = m_Parents.begin(); itParent != m_Parents.end(); ++itParent )
  {
    CoreGuiBase* parent = *itParent;
    if ( parent == parentObject )
    {
      m_Parents.erase( itParent );
      return;
    }
  }
}

bool GuiResource::SpawnGui(CoreGuiBase* parent)
{
  // TODO Thor 05.05.2015 : Clone the spawned gui since the code is altering the gui
  //if (parent->IsOrphan())
  {
    CoreGuiBase* spawnedGui = SpawnGui(parent, m_ParsedData->GetRootNode());
    if (spawnedGui != NULL)
    {
      spawnedGui->SetResourceName(m_Filename);
      m_SignalResourceReady(this, parent);
      return true;
    }
    else
    {
      dfBugreportFMT(spawnedGui != NULL, "Failed to parse gui xml file %s", m_Filename);
    }
  }
  return false;
}

CoreGuiBase* GuiResource::SpawnGui( CoreGuiBase* parent, XMLNode* guiNode )
{
  if ( guiNode != NULL )
  {
    CoreGuiBase* gui = NULL;
    if ( guiNode->GetName() != NULL )
    {
      gui = CoreGuiToolkit::InstantiateGui( guiNode->GetName() );

      if ( gui == NULL )
      {
        LogErrorFMT("GuiResource", "[%s] Failed to spawn guinode [%s]", m_Filename, guiNode->GetName() );
        return NULL;
      }
    }
    else if ( parent != NULL )
    {
      gui = new CoreGuiBase("ROOT", parent->GetPosition(), parent->GetSize() );  // Root node
    }
    else
    {
      gui = new CoreGuiBase( "ROOT", Vector2::s_Vec2Zero, Vector2::s_Vec2Zero ); // Root node
    }

    if (parent != NULL)
    {
      parent->AddChild( gui );
    }

    gui->SetXMLResource();

    // Set all the members on this gui
    const std::vector< KeyValue >& memberList = guiNode->GetMembers();
    for ( std::vector< KeyValue >::const_iterator itMember = memberList.begin(); itMember != memberList.end(); ++itMember )
    {
      const KeyValue& pMember = *itMember;
      gui->SetMember( pMember.m_Key, pMember.m_Value );
    }

#ifdef DEBUG
    gui->SetMemberList(memberList);
#endif
    
    // Spawn children
    const std::vector< XMLNode* >& childList = guiNode->GetChildList();
    for ( size_t index = 0; index < childList.size(); index++ )
    {
      XMLNode* childNode    = childList[ index ];
      SpawnGui(gui, childNode );
    }
    return gui;
  }

  return NULL;
}
