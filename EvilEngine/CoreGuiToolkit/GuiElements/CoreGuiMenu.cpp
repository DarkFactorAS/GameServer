
#include "PreCompile.h"
#include "CoreGuiMenu.h"

CoreGuiMenu::CoreGuiMenu( const String& title, const Vector2& position, const Vector2& size, uint32 resourceId, int direction ) :
  CoreGuiBase( "CoreGuiMenu", position, size ),
  m_Direction( direction ),
  m_Show( false ),
  m_MenuId( 0 ),
  m_Offset( 0 )
{
  m_ResourceId = resourceId;
  CoreGuiButton* btn = new CoreGuiButton( "MAIN", CoreGuiButton::BUTTONTYPE_NONE,  title, Vector2( m_Position.x, m_Position.y ), m_Size, resourceId );
  btn->m_SignalButtonClicked.Connect( this, &CoreGuiMenu::SlotToggleMenu );
  btn->SetColor( Color::Green );
  AddChild( btn );

  m_Offset += ( m_Size.y * 2.0f ) + CoreGuiBase::BORDER;
}

void CoreGuiMenu::AddSpacer()
{
  m_Offset += m_Size.y;
}

CoreGuiButton* CoreGuiMenu::AddSubMenu( const String& title, const Color& color )
{
  //m_Index++;
  CoreGuiButton* btn = new CoreGuiButton( title, Vector2( m_Position.x, m_Position.y - 0.01f - m_Offset ), m_Size, GetResourceId() );
  btn->SetColor( color );
  m_SubMenuList.push_back( btn );

  m_Offset += ( m_Size.y * 2.0f ) + CoreGuiBase::BORDER;

  return btn;
}

void CoreGuiMenu::SlotToggleMenu( int /*btnId*/ )
{
  Show( !m_Show );
}

void CoreGuiMenu::ShowMainMenu( bool showMainMenu )
{
  CoreGuiBase* main = GetChildWithName("MAIN");
  if ( main != NULL )
  {
    if ( showMainMenu )
    {
      main->ClearFlag( CoreGuiBase::FLAGS_ISINVISIBLE );
    }
    else
    {
      main->SetFlag( CoreGuiBase::FLAGS_ISINVISIBLE );
    }
  }
}


void CoreGuiMenu::Clear()
{
  Show( false );
  for ( size_t index = 0; index < m_SubMenuList.size(); index++ )
  {
    CoreGuiButton* guiObject = m_SubMenuList[ index ];
    guiObject->Destroy();
  }
  m_SubMenuList.clear();
  m_Offset = ( m_Size.y * 2.0f ) + CoreGuiBase::BORDER;
}

void CoreGuiMenu::Show( bool showSubMenu )
{
  if ( m_Show == showSubMenu )
  {
    return;
  }
  m_Show = showSubMenu;

  if ( m_Show )
  {
    for ( size_t index = 0; index < m_SubMenuList.size(); index++ )
    {
      CoreGuiButton* guiObject = m_SubMenuList[ index ];
      AddChild( guiObject );
    }
  }
  else
  {
    for ( size_t index = 0; index < m_SubMenuList.size(); index++ )
    {
      CoreGuiButton* guiObject = m_SubMenuList[ index ];
      RemoveChild( guiObject );
    }
  }
}
