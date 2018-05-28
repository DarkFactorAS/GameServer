/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : CoreGuiWindow
* Description : Creates a gui window with some buttons on it.
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "PreCompile.h"
#include "CoreGuiWindow.h"
#include "../CoreGuiToolkit.h"

#include "../BaseGuiElements/CoreGuiImageView.h"
#include "../BaseGuiElements/CoreGuiTextfield.h"
#include "../BaseGuiElements/CoreGuiFrame.h"
#include "EvilEngine/CoreRendering/RenderObjects/TextStringRenderObject.h"

#include <stdlib.h>

CoreGuiWindow::CoreGuiWindow() :
  CoreGuiView("CoreGuiWindow", Vector2::s_Vec2Zero, Vector2::s_Vec2One),
  m_Drag(CoreGuiToolkit::s_MoveGui),
  m_TitleHeight(0.04f),
  m_WindowTitleBg(NULL)
{
  InitWindow("", Vector2::s_Vec2Zero, Vector2::s_Vec2One, GuiResource_WindowBackground, false);
}

CoreGuiWindow::CoreGuiWindow( const String& title, const Vector2& position, const Vector2& size ) : 
  CoreGuiView( "CoreGuiWindow", position, size ),
  m_Drag( CoreGuiToolkit::s_MoveGui ),
  m_TitleHeight( 0.04f ),
  m_WindowTitleBg( NULL )
{
  InitWindow( title, position, size, GuiResource_WindowBackground, false );
}

CoreGuiWindow::CoreGuiWindow( const String& title, const Vector2& position, const Vector2& size, uint32 bgResourceId, bool showBorders ) : 
  CoreGuiView( "CoreGuiWindow", position, size ),
  m_Drag( CoreGuiToolkit::s_MoveGui ),
  m_TitleHeight( 0.04f ),
  m_WindowTitleBg( NULL )
{
  InitWindow( title, position, size, bgResourceId, showBorders );
}

void CoreGuiWindow::SetTitle( const String& title )
{
  if ( m_WindowTitleText != NULL )
  {
    m_WindowTitleText->SetText( title );
  }
}

void CoreGuiWindow::SetMember( const String& key, const String& value )
{
  if ( key.EqualsWithCase("title" ) )
  {
    SetTitle( value );
  }
  else if ( key.EqualsWithCase("titleheight" ) )
  {
    m_TitleHeight = (float)atof( value.c_str() );
    Vector2 titlePos( m_Position.x, m_Position.y - m_Size.y + m_TitleHeight );
    Vector2 titleSize( m_Size.x , m_TitleHeight );

    String title = m_WindowTitleText->GetText();

    RemoveChild( m_WindowTitleBg );
    RemoveChild( m_WindowTitleText );
    m_WindowTitleBg     = new CoreGuiImageView( titlePos, titleSize, m_WindowTitleBgColor );
    m_WindowTitleText   = new CoreGuiTextfield( title, titlePos, titleSize, TextStringRenderObject::ALIGNMENT_CENTER, Color::White );

    AddChild( m_WindowTitleBg );
    AddChild( m_WindowTitleText );
  }
  else if ( key.EqualsWithCase("titlebgcolor" ) )
  {
    SetValueColor( m_WindowTitleBg, m_WindowTitleBgColor, value );
  }
  else if ( key.EqualsWithCase("bgcolor" ) )
  {
    SetValueColor( m_WindowBackground, m_WindowBgColor, value );
  }
  else if ( key.EqualsWithCase("resourceid" ) )
  {
    if ( m_WindowBackground != NULL )
    {
      RemoveChild( m_WindowBackground );
      m_WindowBackground = NULL;
    }

    uint32 resourceId = atoi( value.c_str() );
    if ( resourceId != 0 )
    {
      m_WindowBackground  = new CoreGuiImageView( m_Position, m_Size, resourceId );
      m_WindowBackground->SetAlpha( m_BackgroundAlpha );
      m_WindowBackground->SetColor( m_WindowBgColor );
      AddChild( m_WindowBackground );
    }
  }
  else if ( key.EqualsWithCase("alpha" ) )
  {
    m_BackgroundAlpha = (float)atof( value.c_str() );
    if ( m_WindowBackground != NULL )
    {
      m_WindowBackground->SetAlpha( m_BackgroundAlpha );
    }
  }
  else
  {
    CoreGuiView::SetMember( key, value );
  }
}

void CoreGuiWindow::InitWindow( const String& title, const Vector2& position, const Vector2& size, uint32 bgResourceId, bool showBorders )
{
  SetLayer( LAYER_WINDOW );

  m_TitleHeight         = s_TitleSize;
  m_BackgroundAlpha     = 0.5f;
  m_BorderColor         = Color::GetNamedColor( StaticStr("window_bordercolor") );
  m_WindowTitleBgColor  = Color::GetNamedColor( StaticStr("window_titlebgcolor") );
  m_WindowBgColor       = Color::GetNamedColor( StaticStr("window_bgcolor") );
  Color titleTextColor  = Color::GetNamedColor( StaticStr("window_titletextcolor") );

  Vector2 titlePos( position.x, position.y - size.y + s_TitleSize + BORDER );
  Vector2 titleSize( size.x , s_TitleSize );
  Vector2 titlePos2( position.x, position.y - size.y + ( s_TitleSize * 1.2f ) );

  if ( bgResourceId != 0 )
  {
    m_WindowBackground  = new CoreGuiImageView( position, size, bgResourceId );
    m_WindowBackground->SetAlpha( m_BackgroundAlpha );
    AddChild( m_WindowBackground );
  }
  else
  {
    m_WindowBackground = NULL;
  }

  if ( showBorders )
  {
    m_WindowTitleBg     = new CoreGuiImageView( titlePos, titleSize, m_BorderColor );
    AddChild( m_WindowTitleBg );
  }

  m_WindowTitleText   = new CoreGuiTextfield( title, titlePos2, titleSize, TextStringRenderObject::ALIGNMENT_CENTER, titleTextColor );
  AddChild( m_WindowTitleText );

  m_ContentView       = new CoreGuiView( Vector2( position.x, position.y - titleSize.y ), Vector2( size.x, size.y - titleSize.y ) );
  AddChild( m_ContentView );

  ShowBorders( showBorders );
}

//void CoreGuiWindow::ShowBorders( bool showBorders )
//{
//  RemoveChild( m_GuiFrame );
//  RemoveChild( m_WindowTitleBg );
//
//  if ( showBorders )
//  {
//    Vector2 titlePos( m_Position.x, m_Position.y - m_Size.y + m_TitleHeight );
//    Vector2 titleSize( m_Size.x , m_TitleHeight );
//
//    String title = m_WindowTitleText->GetText();
//    RemoveChild( m_WindowTitleText );
//
//    m_WindowTitleBg     = new CoreGuiImageView( titlePos, titleSize, m_WindowTitleBgColor );
//    m_WindowTitleText   = new CoreGuiTextfield( title, titlePos, titleSize, TextStringRenderObject::ALIGNMENT_CENTER, Color::White );
//    m_GuiFrame          = new CoreGuiFrame( m_Position, m_Size, m_BorderColor );
//
//    AddChild( m_WindowTitleBg );
//    AddChild( m_WindowTitleText );
//    AddChild( m_GuiFrame );
//  }
//  else
//  {
//    m_GuiFrame = NULL;
//    m_WindowTitleBg = NULL;
//  }
//}

void CoreGuiWindow::CloseWindow( int /*btnId*/ )
{
  CoreGuiView::Destroy();
}

float CoreGuiWindow::GetContentTop()
{
  return m_ContentView->GetTop();
}

const Vector3& CoreGuiWindow::GetContentSize()
{
  return m_ContentView->GetSize();
}

const Vector3& CoreGuiWindow::GetContentPosition()
{
  return m_ContentView->GetPosition();
}

CoreGuiBase* CoreGuiWindow::OnMouseButtonUp( int /*buttonId*/, float /*xPos*/, float /*yPos*/ )
{
  m_Drag = false;
  return NULL;
}

/************************************************************************************************
* OnMouseButtonDown:
* Normal windows will catch mouse clicks for the window title bar ( to move the window if that
* is enabled ) and steal focus if you click inside the window ( unless the window is click-through )
*
* @param  (int)           buttonId     - ID of the mouse button
* @param  (int)           xPos         - X Position of the mouse when clicking
* @param  (int)           yPos         - Y Position of the mouse when clicking
* @return (CoreGuiBase*)               - Returns the focus object for the mouse when clicking here
*
* @author Thor Richard Hansen
*************************************************************************************************/
CoreGuiBase* CoreGuiWindow::OnMouseButtonDown( int buttonId, float xPos, float yPos )
{
  CoreGuiBase* focusItem = CoreGuiView::OnMouseButtonDown( buttonId, xPos, yPos );

  // If we click the titlebar, we can drag this window around
  if ( focusItem == NULL && buttonId == 1 && m_WindowTitleBg != NULL && m_WindowTitleBg->IsIntersecting( xPos, yPos ) )
  {
    m_Drag = CoreGuiToolkit::s_MoveGui;
    return this;
  }

  // If this window is modal, it will override everything else
  if ( focusItem == NULL && IsModal() )
  {
    return this;
  }

  return focusItem;
}

bool CoreGuiWindow::OnMouseDrag( float  xPos , float  yPos  )
{
  if ( m_Drag )
  {
    Move( xPos, yPos, false );
    return true;
  }
  return false;
}

void CoreGuiWindow::SetTitleSize( float x, float y )
{
  if ( m_WindowTitleBg != NULL )
  {
    m_WindowTitleBg->SetSize( x, y );
  }
  if ( m_WindowTitleText != NULL )
  {
    m_WindowTitleText->SetSize( x, y );
  }
}

void CoreGuiWindow::SetBgColor( const Color& color )
{
  if ( m_WindowBackground != NULL )
  {
    m_WindowBackground->SetColor( color );
  }
}

void CoreGuiWindow::SetTitleBgColor( const Color& color )
{
  m_WindowTitleBgColor = color;
  if ( m_WindowTitleBg != NULL )
  {
    m_WindowTitleBg->SetColor( m_WindowTitleBgColor );
  }
}
