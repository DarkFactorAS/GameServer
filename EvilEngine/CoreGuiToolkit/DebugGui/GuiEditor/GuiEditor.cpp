
#include "Precompile.h"
#include "GuiEditor.h"
#include "GuiPropertyEditor.h"
#include "EvilEngine/CoreGuiToolkit/GuiElements/GridRowData.h"
#include "EvilEngine/CoreGuiToolkit/CoreGuiToolkitInclude.h"
#include "EvilEngine/CoreGuiToolkit/GuiCreator/GuiResource.h"
#include "EvilEngine/CoreRendering/RenderObjects/TextStringRenderObject.h"
#include "EvilEngine/CoreEngine/CoreResourceManager/CoreResourceManager.h"

#include "EvilEngine/CoreEngine/CoreEffectSystem/EffectPackage.h"
#include "EvilEngine/CoreEngine/CoreEffectSystem/Tasks/LoopEffectList.h"
#include "EvilEngine/CoreEngine/CoreEffectSystem/Effects/FadeEffect.h"
#include "EvilEngine/CoreGuiToolkit/CoreGuiToolkit.h"

GuiEditor::GuiEditor() :
  CoreGuiXMLView( "DebugGuiEditorWindow.xml" ),
  m_GuiObject( NULL )
{
  SetName("GuiEditor");
}

GuiEditor::~GuiEditor()
{
  StopEffect();
}

void GuiEditor::OnDataLoaded( Resource* /*resource*/ )
{
  ConnectButton( "BtnSave", &GuiEditor::SlotSaveGui);
  ConnectButton( "BtnAll", &GuiEditor::SlotTopGui);
  ConnectButton( "BtnReload", &GuiEditor::SlotReloadGui);
  ConnectButton( "BtnClear", &GuiEditor::SlotClear);
  ConnectButton( "BtnReloadMe", &GuiEditor::SlotReloadThisGui);
  ConnectButton( "BtnClose", &GuiEditor::SlotCloseWindow);

  CoreGuiGridView* gridView = (CoreGuiGridView*) GetChildWithName("ObjectGrid");
  if ( gridView != NULL )
  {
    gridView->AddHeader( 5, " ");
    gridView->AddHeader( 10, "ID" );
    gridView->AddHeader( 65, "Name");
    gridView->AddHeader( 10, "E");
    gridView->AddHeader( 10, "V" );
  }

  SetDebugGui( true );
  
  if ( m_GuiObject != NULL )
  {
    SetActiveParent(m_GuiObject);
  }
}

void GuiEditor::StopEffect()
{
  if (m_GuiObject != NULL && m_EffectPackage != NULL)
  {
    m_GuiObject->RemoveEffectPackage(m_EffectPackage);

    EffectPackage* effectPackage = m_GuiObject->CreateEffectPackage();
    effectPackage->AddEffect(new FadeEffect(1.0f, m_GuiObject->GetAlpha(), m_EffectStartAlpha));
    m_EffectPackage = NULL;
  }
}

void GuiEditor::SlotTopGui(int /*btnId*/)
{
  StopEffect();

  CoreGuiGridView* gridView = (CoreGuiGridView*)GetChildWithName("ObjectGrid");
  if (gridView == NULL)
  {
    return;
  }
  gridView->Clear();

  uint32 uniqueId = 0;
  std::vector< CoreGuiBase* > guiList = CoreGuiToolkit::GetInstance()->GetGuiObjects();
  for ( uint32 index = 0; index < guiList.size(); index++ )
  {
    CoreGuiBase* guiBase = guiList[ index ];
    uniqueId = AddChildren(gridView, guiBase, NULL, uniqueId);
  }
}

void GuiEditor::SetActiveParent( CoreGuiBase* debugObject )
{
  CoreGuiGridView* gridView = (CoreGuiGridView*) GetChildWithName("ObjectGrid");
  if ( gridView != NULL )
  {
    gridView->SetSelectedItem(debugObject);
  }

  StopEffect();

  bool addEffect = ( m_GuiObject == debugObject && debugObject != NULL );

  StopEffect();
  m_GuiObject = debugObject;
  SlotTopGui(0);

  if ( addEffect )
  {
    m_EffectStartAlpha = debugObject->GetAlpha();
    m_EffectPackage = debugObject->CreateEffectPackage();
    LoopEffectList* loopEffect = safe_cast< LoopEffectList* > ( m_EffectPackage->AddEffect( new LoopEffectList() ) );
    loopEffect->AddEffect( new FadeEffect( 1.0f, 0.0f, 1.0f ) );
    loopEffect->AddEffect( new FadeEffect( 1.0f, 1.0f, 0.0f ) );
  }

/*  
  StopEffect();


  CoreGuiGridView* gridView = (CoreGuiGridView*) GetChildWithName("ObjectGrid");
  if ( gridView == NULL )
  {
    return;
  }

  gridView->Clear();

  if ( debugObject != NULL )
  {
    CoreGuiBase* xmlParent = debugObject->GetXMLParent();
    if (xmlParent == NULL)
    {
      return;
    }
    gridView->SetSelectedItem(debugObject);

    AddChildren(gridView, xmlParent, NULL, 0);

    m_EffectStartAlpha = debugObject->GetAlpha();
    m_EffectPackage = debugObject->CreateEffectPackage();
    LoopEffectList* loopEffect = safe_cast< LoopEffectList* > ( m_EffectPackage->AddEffect( new LoopEffectList() ) );
    loopEffect->AddEffect( new FadeEffect( 1.0f, 0.0f, 1.0f ) );
    loopEffect->AddEffect( new FadeEffect( 1.0f, 1.0f, 0.0f ) );
  }
 
 */
}

uint32 GuiEditor::AddChildren( CoreGuiGridView* gridView, CoreGuiBase* xmlParent, GridRowData* parent, uint32 uniqueId )
{
  //if ( xmlParent->IsXMLResource() )
  {
    std::vector< CoreGuiBase* > data;

    uint32 levels = 0;
    String spacer = "";
    GridRowData* tParent = parent;
    while ( tParent != NULL )
    {
      spacer += " ";
      tParent = tParent->GetParent();
      levels++;
    }
    if (levels > 5) { levels = 5; }

    if ( !xmlParent->GetResourceName().empty() )
    {
      spacer += xmlParent->GetResourceName();
    }
    else
    {
      spacer += xmlParent->GetName();
    }
    uniqueId++;
    
    bool hasChildren = !xmlParent->IsOrphan();

    //CoreGuiButton* deleteButton = new CoreGuiButton("X", Vector2::s_Vec2One, Vector2::s_Vec2One, GuiResource_ButtonBackground);
    //deleteButton->SetButtonType( uniqueId );
    //deleteButton->m_SignalButtonClicked.Connect(this, &GuiEditor::DeleteElement );
    
    CoreGuiButton* expandCollapseBtn = new CoreGuiButton( ( hasChildren ? "+" : " " ), Vector2::s_Vec2One, Vector2::s_Vec2One, GuiResource_ButtonBackground);
    expandCollapseBtn->SetButtonType(uniqueId);
    expandCollapseBtn->SetColor(Color::DarkBlue);
    expandCollapseBtn->m_SignalButtonClicked.Connect(this, &GuiEditor::ExpandCollapseElement);

    CoreGuiButton* editButton = new CoreGuiButton("Edit", Vector2::s_Vec2One, Vector2::s_Vec2One, GuiResource_ButtonBackground);
    editButton->SetButtonType(uniqueId);
    editButton->SetColor( Color::DarkBlue );
    if ( !xmlParent->GetMemberList().empty() )
    {
      editButton->m_SignalButtonClicked.Connect(this, &GuiEditor::EditElement);
    }
    else
    {
      editButton->SetTextColor( Color::DarkGrey );
    }

    CoreGuiBase* nameField = NULL;
    if (levels > 0)
    {
      CoreGuiView* parentView = new CoreGuiView(Vector2::s_Vec2One, Vector2::s_Vec2One);

      Vector2 size = Vector2::s_Vec2One;
      float delta = (( 1.0f * levels ) / 20.0f);
      size.x = size.x - ( size.x * delta );

      Vector2 pos = Vector2::s_Vec2One;
      pos.x = 0.0f + delta + delta + size.x;

      CoreGuiImageView* imgView = new CoreGuiImageView( Vector2(delta,1.0f), Vector2(delta,1.0f), Color::DarkBlue);
      imgView->SetColor( Color::LightBlue );
      parentView->AddChild(imgView);

      CoreGuiButton* chooseButton = new CoreGuiButton(spacer, pos, size , GuiResource_ButtonBackground);
      chooseButton->SetButtonType(uniqueId);
      chooseButton->SetColor(Color::DarkBlue);
      chooseButton->m_SignalButtonClicked.Connect(this, &GuiEditor::ChooseElement);
      parentView->AddChild( chooseButton );
      nameField = parentView;
    }
    else
    {
      CoreGuiButton* chooseButton = new CoreGuiButton(spacer, Vector2::s_Vec2One, Vector2::s_Vec2One, GuiResource_ButtonBackground);
      chooseButton->SetButtonType(uniqueId);
      chooseButton->SetColor(Color::DarkBlue);
      chooseButton->m_SignalButtonClicked.Connect(this, &GuiEditor::ChooseElement);
      nameField = chooseButton;
    }

    CoreGuiButton* visibilityButton = new CoreGuiButton( (xmlParent->IsVisible() ? "Hide" : "Show" ), Vector2::s_Vec2One, Vector2::s_Vec2One, GuiResource_ButtonBackground);
    visibilityButton->SetButtonType(uniqueId);
    visibilityButton->SetColor(Color::DarkBlue);
    visibilityButton->m_SignalButtonClicked.Connect(this, &GuiEditor::ToggleVisibility);

    CoreGuiImageView* imgView = new CoreGuiImageView( Vector2::s_Vec2One, Vector2::s_Vec2One, Color::DarkBlue);
    imgView->AddChild( new CoreGuiTextfield( String::FormatString("%d ", uniqueId ), Vector2::s_Vec2One, Vector2::s_Vec2One, TextStringRenderObject::ALIGNMENT_CENTERRIGHT ) );

    GridRowData* rowData = new GridRowData( uniqueId, parent, false );
    rowData->AddRowData( expandCollapseBtn );
    rowData->AddRowData( imgView );
    rowData->AddRowData( nameField );
    rowData->AddRowData( editButton );
    rowData->AddRowData( visibilityButton );
    rowData->AddRowData( xmlParent );

    gridView->AddData( rowData );

    const std::vector<CoreGuiBase*>& childList = xmlParent->GetChildren();
    for (std::vector<CoreGuiBase*>::const_iterator itObj = childList.begin(); itObj != childList.end(); itObj++)
    {
      CoreGuiBase* guiObject = *itObj;
      uniqueId = AddChildren(gridView, guiObject, rowData, uniqueId);
    }
  }

  return uniqueId;
}

CoreGuiBase* GuiEditor::GetObjectFromRow( uint32 rowId )
{
  CoreGuiGridView* gridView = (CoreGuiGridView*)GetChildWithName("ObjectGrid");
  if (gridView == NULL)
  {
    return NULL;
  }

  const GridRowData* rowData = gridView->GetRowData(rowId);
  if (rowData != NULL)
  {
    return rowData->GetColumnData( GameObjectIndex );
  }

  return NULL;
}

void GuiEditor::ExpandCollapseElement(int rowId, CoreGuiButton* button)
{
  CoreGuiGridView* gridView = (CoreGuiGridView*)GetChildWithName("ObjectGrid");
  if (gridView == NULL)
  {
    return;
  }

  CoreGuiBase* object = GetObjectFromRow( rowId );
  if ( object != NULL && object->IsOrphan() )
  {
    button->SetText(" ");
  }
  else
  {
    bool isExpanded = gridView->ToggleExpandRow(rowId);
    button->SetText((isExpanded ? "-" : "+"));
  }
}

void GuiEditor::DeleteElement( int rowId, CoreGuiButton* /*button*/ )
{
  CoreGuiBase* rowObject = GetObjectFromRow( rowId );
  if ( rowObject != NULL )
  {
    rowObject->Destroy();
  }
}

void GuiEditor::ChooseElement(int rowId, CoreGuiButton* /*button*/)
{
  CoreGuiBase* guiObject = GetObjectFromRow(rowId);
  SetActiveParent( guiObject );
}

void GuiEditor::ToggleVisibility(int rowId, CoreGuiButton* button)
{
  CoreGuiBase* guiObject = GetObjectFromRow(rowId);
  if ( guiObject != NULL )
  {
    guiObject->SetVisible( !guiObject->IsVisible() );
    button->SetText( guiObject->IsVisible() ? "Hide" : "Show" );
  }
}

void GuiEditor::EditElement(int rowId, CoreGuiButton* /*button*/)
{
  CoreGuiBase* guiObject = GetObjectFromRow(rowId);
  CoreGuiBase* parent    = GetParent();
  CoreGuiBase* window    = GetChildWithName("editwindow");
  if (guiObject != NULL && parent != NULL && window != NULL )
  {
    parent->AddChild( new GuiPropertyEditor( guiObject, false, window->GetPosition() ) );
    this->SetVisible( false );
    //Destroy();
  }
}

void GuiEditor::SlotClear( int /*btnId*/ )
{
  SetActiveParent( NULL );
}

void GuiEditor::SlotCloseWindow(int /*btnId*/)
{
  SignalWindowClosed();
  Destroy();
}

void GuiEditor::SlotSaveGui( int /*btnId*/ )
{
}


void GuiEditor::SlotReloadThisGui(int /*btnId*/)
{
  CoreGuiBase* xmlParent = GetXMLParent();
  if (xmlParent == NULL)
  {
    return;
  }

  CoreGuiBase* attachNode = xmlParent->GetParent();
  SetActiveParent(NULL);

  // If we can remove it from the cache
  if (CoreResourceManager::GetInstance()->ReloadResource(xmlParent->GetResourceName()))
  {
    if (attachNode != NULL)
    {
      attachNode->RemoveChild(xmlParent);
    }
  }
}

void GuiEditor::SlotReloadGui(int /*btnId*/ )
{
  if ( m_GuiObject == NULL )
  {
    return;
  }

  CoreGuiBase* xmlParent = m_GuiObject->GetXMLParent();
  if (xmlParent == NULL)
  {
    return;
  }

  CoreGuiBase* attachNode = xmlParent->GetParent();

  // If we can remove it from the cache
  if ( CoreResourceManager::GetInstance()->ReloadResource( xmlParent->GetResourceName() ) )
  {
    if ( attachNode != NULL )
    {
      SetActiveParent(NULL);
      attachNode->RemoveChild( xmlParent );
    }
  }
}

void GuiEditor::SlotApplySettings( int /*btnId*/ )
{
  if ( m_GuiObject == NULL )
  {
    return;
  }

  CoreGuiInputfield* posX = (CoreGuiInputfield*) GetChildWithName("position_x");
  CoreGuiInputfield* posY = (CoreGuiInputfield*) GetChildWithName("position_y");
  if ( posX != NULL && posY != NULL )
  {
    m_GuiObject->SetPosition( posX->GetFloatValue(), posY->GetFloatValue() );
  }

  CoreGuiInputfield* sizeX = (CoreGuiInputfield*) GetChildWithName("size_x");
  CoreGuiInputfield* sizeY = (CoreGuiInputfield*) GetChildWithName("size_y");
  if ( sizeX != NULL && sizeY != NULL )
  {
    m_GuiObject->SetSize( sizeX->GetFloatValue(), sizeY->GetFloatValue() );
  }
}
