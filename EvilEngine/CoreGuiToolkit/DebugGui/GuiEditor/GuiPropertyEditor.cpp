
#include "Precompile.h"
#include "GuiPropertyEditor.h"
#include "GuiEditor.h"
#include "EvilEngine/CoreGuiToolkit/CoreGuiToolkitInclude.h"
#include "EvilEngine/CoreGuiToolkit/GuiCreator/GuiResource.h"
#include "EvilEngine/CoreRendering/RenderObjects/TextStringRenderObject.h"
#include "EvilEngine/CoreEngine/CoreResourceManager/CoreResourceManager.h"

GuiPropertyEditor::GuiPropertyEditor( CoreGuiBase* guiObject, bool isMinimized, const Vector2& position ) :
  CoreGuiXMLView( "DebugGuiPropertyWindow.xml" ),
  m_GuiObject( guiObject ),
  m_HasChanges(false),
  m_IsMinimized(isMinimized),
  m_ParentPosition( position )
{
}

void GuiPropertyEditor::OnDataLoaded( Resource* /*resource*/ )
{
  SetDebugGui(true);
  SetPosition(m_ParentPosition);


  ConnectButton( "BtnAdd", &GuiPropertyEditor::SlotAddField );
  ConnectButton( "BtnMinimize", &GuiPropertyEditor::SlotToggleMinimize );
  ConnectButton( "BtnClose", &GuiPropertyEditor::SlotCloseWindow );

  CoreGuiBase* gridWindow = GetChildWithName("maxwindow");
  if ( gridWindow != NULL )
  {
    gridWindow->SetVisible( !m_IsMinimized );
    //gridWindow->SetAlpha( (m_IsMinimized ? 0.1f : 0.8f ) );
  }
  
  if ( !m_IsMinimized )
  {
    ConnectButton( "BtnSelf", &GuiPropertyEditor::SlotSelf );
    ConnectButton( "BtnUndo", &GuiPropertyEditor::SlotUndo );
    
    // Add members
    if ( m_GuiObject != NULL )
    {
      m_MemberList = m_GuiObject->GetMemberList();
      ShowGridList(m_MemberList);
    }
  }  
}

void GuiPropertyEditor::ShowGridList(std::vector< KeyValue > memberList)
{
  uint32 uniqueId = 0;
  CoreGuiGridView* gridView = (CoreGuiGridView*) GetChildWithName("ObjectGrid");
  if ( gridView != NULL )
  {
    gridView->Clear();
    gridView->AddHeader( 30, "Name" );
    gridView->AddHeader( 70, "Value");
  
    for (std::vector< KeyValue >::iterator itMember = memberList.begin(); itMember != memberList.end(); ++itMember)
    {
      KeyValue& keyValue = *itMember;
      GridRowData* rowData = new GridRowData(++uniqueId, NULL, true);
      
      CoreGuiInputfield* nameInputField = new CoreGuiInputfield(keyValue.m_Key, Vector2::s_Vec2One, Vector2::s_Vec2One, TextStringRenderObject::ALIGNMENT_CENTERLEFT);
      nameInputField->m_ActionEvent.Connect( this, &GuiPropertyEditor::SlotChangedNameInputfield );
      nameInputField->SetName( uniqueId );
      
      CoreGuiInputfield* valueInputField = new CoreGuiInputfield(keyValue.m_Value, Vector2::s_Vec2One, Vector2::s_Vec2One, TextStringRenderObject::ALIGNMENT_CENTERLEFT);
      valueInputField->m_ActionEvent.Connect( this, &GuiPropertyEditor::SlotChangedValueInputfield );
      valueInputField->SetName( keyValue.m_Key );
      
      rowData->AddRowData(nameInputField);
      rowData->AddRowData(valueInputField);
      gridView->AddData(rowData);
    }
  }
}

void GuiPropertyEditor::CloseWindow()
{
  if ( m_GuiObject != NULL )
  {
    m_GuiObject->SetMemberList( m_MemberList );
  }
  
  CoreGuiBase* parent = GetParent();
  if ( parent != NULL && !m_GuiObject->IsDebugGui() )
  {
    CoreGuiBase* guiEditor = parent->GetChildWithName("GuiEditor");
    if ( guiEditor != NULL )
    {
      guiEditor->SetVisible( true );
    }
  }

  Destroy();
}

void GuiPropertyEditor::SlotToggleMinimize( int /*btnId*/ )
{
  CoreGuiBase* parent = GetParent();
  if ( parent != NULL )
  {
    m_IsMinimized = !m_IsMinimized;
    
    CoreGuiBase* gridWindow = GetChildWithName("maxwindow");
    if ( gridWindow != NULL )
    {
      gridWindow->SetVisible( !m_IsMinimized );
    }
  }
}

void GuiPropertyEditor::SlotAddField( int /*btnId*/ )
{
  m_MemberList.push_back( KeyValue("","") );
  ShowGridList( m_MemberList );
}

void GuiPropertyEditor::SlotCloseWindow( int /*btnId*/ )
{
  CloseWindow();
}

void GuiPropertyEditor::SlotSelf( int /*btnId*/ )
{
  SetDebugGui(false);

  //CoreGuiBase* parent = GetParent();
  //if ( parent != NULL )
  //{
  //  parent->AddChild( new GuiEditor( this ) );
  //}
}

void GuiPropertyEditor::SlotUndo( int /*btnId*/ )
{
  m_MemberList = m_GuiObject->GetMemberList();
  for (std::vector< KeyValue >::iterator itMember = m_MemberList.begin(); itMember != m_MemberList.end(); ++itMember)
  {
    KeyValue& keyValue = *itMember;
    m_GuiObject->SetMember( keyValue.m_Key, keyValue.m_Value );
  }
  CloseWindow();
}

void GuiPropertyEditor::SlotChangedNameInputfield( int /*btnId*/, CoreGuiBase* guiBase )
{
  CoreGuiInputfield* inputField = safe_cast< CoreGuiInputfield* > ( guiBase );
  if ( inputField == NULL )
  {
    return;
  }

  CoreGuiGridView* gridView = (CoreGuiGridView*) GetChildWithName("ObjectGrid");
  if ( gridView != NULL )
  {
    uint32 uniqueKey = inputField->GetName().ToUInt32();
    const GridRowData* rowData = gridView->GetRowData( uniqueKey );
    if ( rowData != NULL )
    {
      CoreGuiBase* valueInputField = rowData->GetColumnData( 1 );
      if ( valueInputField != NULL )
      {
        valueInputField->SetName( inputField->GetValue() );
      }
    }
  }
}

void GuiPropertyEditor::SlotChangedValueInputfield( int /*btnId*/, CoreGuiBase* guiBase )
{
  CoreGuiInputfield* inputField = safe_cast< CoreGuiInputfield* > ( guiBase );
  if ( inputField == NULL )
  {
    return;
  }

  // Update memberlist
  for (std::vector< KeyValue >::iterator itMember = m_MemberList.begin(); itMember != m_MemberList.end(); ++itMember)
  {
    KeyValue& keyValue = *itMember;
    if ( keyValue.m_Key.Equals( inputField->GetName() ) ) 
    {
      keyValue.m_Value = inputField->GetValue();
    }
  }

  // Apply to object
  if ( !inputField->GetName().empty() )
  {
    m_GuiObject->SetMember( inputField->GetName(), inputField->GetValue() );
  }
  
  m_HasChanges = true;
}
