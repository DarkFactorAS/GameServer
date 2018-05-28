/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : CoreGuiButton
* Description : Creates the render and handling of a button on the openGLscreen.
*               This button will render with a button texture, take mouse input and has callback
*               for when it is pressed/released.
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "CoreGuiButton.h"
#include "CoreGuiFrame.h"
#include "../../CoreRendering/RenderObjects/TextStringRenderObject.h"
#include "CoreGuiTextfield.h"
#include "CoreGuiImageView.h"
#include "CoreGuiRoundedImageView.h"
#include "../CoreGuiToolkit.h"

#include <stdlib.h>

float CoreGuiButton::s_DefaultWidth  = 0.15f;
float CoreGuiButton::s_DefaultHeight = 0.0675f;

bool CoreGuiButton::s_UseRoundedButtons = true;
Vector2 CoreGuiButton::s_DefaultSize = Vector2( 0.15f, 0.0675f );

Color CoreGuiButton::ButtonColorNo   = Color( 1, 0, 0 );
Color CoreGuiButton::ButtonColorYes  = Color( 0, 1, 0 );

CoreGuiButton::CoreGuiButton( const String& text, const Vector2& position, const Vector2& size, uint32 resourceId ) : 
  CoreGuiBase( "CoreGuiButton", position, size )
{
  Init( BUTTONTYPE_NONE, text, position, size, resourceId );
}

CoreGuiButton::CoreGuiButton( const String& text, const Vector2& position, uint32 resourceId ) : 
  CoreGuiBase( "CoreGuiButton", position, Vector2::s_Vec2One )
{
  Init( BUTTONTYPE_NONE, text, position, Vector2( s_DefaultWidth, s_DefaultHeight ), resourceId );
}

CoreGuiButton::CoreGuiButton( int buttonType, const String& text, const Vector2& position, uint32 resourceId ) : 
  CoreGuiBase( "CoreGuiButton", position, Vector2::s_Vec2One )
{
  Init( buttonType, text, position, Vector2( s_DefaultWidth, s_DefaultHeight ), resourceId );
}

CoreGuiButton::CoreGuiButton( const String& name, int buttonType, const String& text, const Vector2& position, const Vector2& size, uint32 resourceId ) : 
  CoreGuiBase( name, position, size )
{
  Init( buttonType, text, position, size, resourceId );
}

void CoreGuiButton::Init( int buttonType, const String& text, const Vector2& position, const Vector2& size, uint32 resourceId )
{
  m_Image             = NULL;
  m_ResourceId        = resourceId;
  m_ButtonType        = buttonType;
  m_Position          = position;
  m_Size              = size;
  m_IsDown            = false;
  m_Textscale         = 0.6f;

  m_GuiFrame1         = NULL;
  m_GuiFrame2         = NULL;
  m_ButtonFrame       = NULL;

  m_ButtonRoundBg     = NULL;
  m_ButtonNormalBg    = NULL;

  m_UseRoundedIcons   = s_UseRoundedButtons;
  m_IsEnabled         = true;

  String upperText    = text;//text.GetUpper();

  Vector3 tSize       = Vector3( size.x * 0.5f, size.y, 0 );
  Vector3 tPos        = Vector3( position.x, position.y, 0 );

  StringList strList  = StringList::Split( upperText, "\n");
  if ( strList.size() > 1 )
  {
    float multiTextScale = 1.0f / strList.size();
    if ( multiTextScale < m_Textscale )
    {
      m_Textscale = multiTextScale;
    }
  }

  tSize.y = tSize.y * m_Textscale;

  CreateButtons( m_UseRoundedIcons );

  m_Title             = new CoreGuiTextfield( upperText, tPos, tSize, TextStringRenderObject::ALIGNMENT_CENTER, Color::White );

  AddChildWithLayer( m_Title, LAYER_TEXT );
  //AddChild( m_GuiFrame1, 3 );
  //AddChild( m_GuiFrame2, 3 );

  //ShowBorders( true );
}

CoreGuiButton::~CoreGuiButton()
{
}

void CoreGuiButton::SetEnabled( bool isEnabled )
{
  if ( m_IsEnabled != isEnabled )
  {
    m_IsEnabled = isEnabled;
    if ( isEnabled )
    {
      SetColor( Color::LightBlue );
      SetTextColor( Color::White );
    }
    else
    {
      SetColor( Color::Grey );
      SetTextColor( Color::DarkGrey );
    }
  }
}

void CoreGuiButton::CreateButtons( bool useRoundedIcons )
{
  RemoveChild( m_ButtonNormalBg );
  RemoveChild( m_ButtonRoundBg );

  m_ButtonNormalBg = NULL;
  m_ButtonRoundBg  = NULL;

  if ( useRoundedIcons )
  {
    m_ButtonRoundBg  = new CoreGuiRoundedImageView( m_ObjectName, m_Position, m_Size, m_ResourceId );
    AddChildWithLayer( m_ButtonRoundBg, LAYER_BACKGROUND );
  }
  else
  {
    m_ButtonNormalBg = new CoreGuiImageView( m_ObjectName, m_Position, m_Size, m_ResourceId );
    AddChildWithLayer( m_ButtonNormalBg, LAYER_BACKGROUND );
  }
}

void CoreGuiButton::UseRoundedIcons( bool useRoundedIcons )
{
  if ( m_UseRoundedIcons != useRoundedIcons )
  {
    m_UseRoundedIcons = useRoundedIcons; 
    CreateButtons( useRoundedIcons );
  }
}


void CoreGuiButton::SetResourceId( uint32 resourceId )
{
  SetBackground( resourceId, m_Color.GetAlpha(), m_Color );
  CoreGuiBase::SetResourceId( resourceId );
}

void CoreGuiButton::SetBackground( uint32 resourceId, float alpha, const Color& color )
{
  if ( resourceId != 0 && m_UseRoundedIcons )
  {
    if ( m_ButtonRoundBg == NULL )
    {
      m_ButtonRoundBg  = new CoreGuiRoundedImageView( m_Position, m_Size, resourceId );
      AddChildWithLayer( m_ButtonRoundBg, LAYER_BACKGROUND );
    }
    else
    {
      m_ButtonRoundBg->SetResourceId( resourceId );
    }
    m_ButtonRoundBg->SetAlpha( alpha );
    m_ButtonRoundBg->SetColor( color );
  }
  if ( resourceId != 0 && !m_UseRoundedIcons )
  {
    if ( m_ButtonNormalBg == NULL )
    {
      m_ButtonNormalBg  = new CoreGuiImageView( m_Position, m_Size, resourceId );
      AddChildWithLayer( m_ButtonNormalBg, LAYER_BACKGROUND );
    }
    else
    {
      m_ButtonNormalBg->SetResourceId( resourceId );
    }
    m_ButtonNormalBg->SetAlpha( alpha );
    m_ButtonNormalBg->SetColor( color );
  }
  else if ( resourceId == 0 && m_ButtonRoundBg != NULL )
  {
    RemoveChild( m_ButtonRoundBg );
    m_ButtonRoundBg = NULL;
  }
  else if ( resourceId == 0 && m_ButtonNormalBg != NULL )
  {
    RemoveChild( m_ButtonNormalBg );
    m_ButtonNormalBg = NULL;
  }
}

void CoreGuiButton::SetImage( CoreGuiImageView* image )
{
  //int layer = ( m_ButtonRoundBg != NULL ) ? m_ButtonRoundBg->GetLayer() : m_ButtonNormalBg->GetLayer();
  if ( m_Image == NULL && image != NULL )
  {
    m_Image  = image;
    AddChildWithLayer( m_Image, LAYER_IMAGE );
  }
  else if ( m_Image != NULL && image == NULL )
  {
    RemoveChild( m_Image );
    m_Image = NULL;
  }
}

void CoreGuiButton::SetImage( uint32 resourceId )
{
  if ( resourceId != 0 )
  {
    if ( m_Image == NULL )
    {
      m_Image  = new CoreGuiImageView( m_Position, m_Size, resourceId );
      AddChildWithLayer( m_Image, LAYER_IMAGE );
    }
    else
    {
      m_Image->SetResourceId( resourceId );
    }
  }
  else
  {
    RemoveChild( m_Image );
    m_Image = NULL;
  }
}

void CoreGuiButton::SetImage( uint32 resourceId, float alpha, const Color& color )
{
  if ( resourceId != 0 )
  {
    if ( m_Image == NULL )
    {
      m_Image  = new CoreGuiImageView( m_Position, m_Size, resourceId );
      AddChildWithLayer( m_Image, LAYER_IMAGE );
    }
    else
    {
      m_Image->SetResourceId( resourceId );
    }
    m_Image->SetAlpha( alpha );
    m_Image->SetColor( color );
  }
  else
  {
    RemoveChild( m_Image );
    m_Image = NULL;
  }
}

void CoreGuiButton::SetBackgroundColor( const Color& color, float alpha )
{
  if ( m_ButtonRoundBg != NULL )
  {
    m_ButtonRoundBg->SetColor( color );
    m_ButtonRoundBg->SetAlpha( alpha );
  }
  else if ( m_ButtonNormalBg != NULL )
  {
    m_ButtonNormalBg->SetColor( color );
    m_ButtonNormalBg->SetAlpha( alpha );
  }
}
  
void CoreGuiButton::SetMember( const String& key, const String& value )
{
  if ( key.EqualsWithCase("title" ) )
  {
    SetText( value );
  }
  else if ( key.EqualsWithCase( "textscale" ) )
  {
    float scale = (float)atof( value.c_str() );
    if ( scale >= 0.0f )
    {
      m_Textscale = scale;
      m_Title->SetTextScale( scale );
    }
  }
  else if ( key.EqualsWithCase("textcolor" ) )
  {
    if ( value.IsNumeric() )
    {
      uint32 color = (uint32)strtol( value.c_str(), NULL, 16 );
      SetTextColor( color );
    }
    else
    {
      Color color = Color::GetNamedColor( value );
      SetTextColor( color );
    }
  }
  else if ( key.EqualsWithCase("image") )
  {
    uint32 resourceId = atoi( value.c_str() );
    SetImage( resourceId, 1.0f, Color::White );
  }
  else if ( key.EqualsWithCase("imagecolor") )
  {
    if ( m_Image != NULL )
    {
      if ( value.IsNumeric() )
      {
        uint32 color = (uint32)strtol( value.c_str(), NULL, 16 );
        m_Image->SetColor( color );
      }
      else
      {
        Color color = Color::GetNamedColor( value );
        m_Image->SetColor( color );
      }
    }
  }
  else if ( key.EqualsWithCase("resourceid" ) )
  {
    uint32 resourceId = atoi( value.c_str() );
    SetResourceId( resourceId );
  }
  else if ( key.EqualsWithCase("type" ) )
  {
    m_ButtonType = atoi( value.c_str() );
  }
  else if ( key.EqualsWithCase("color") )
  {
    if ( value.IsNumeric() )
    {
      uint32 color = (uint32)strtol( value.c_str(), NULL, 16 );
      SetColor( color );
    }
    else
    {
      Color color = Color::GetNamedColor( value );
      SetColor( color );
    }
  }
  else
  {
    CoreGuiBase::SetMember( key, value );
  }
}

void CoreGuiButton::SetImageColor( const Color& color )
{
  if ( m_Image != NULL )
  {
    m_Image->SetColor( color );
  }
}

void CoreGuiButton::ShowBorders( bool showBorder )
{
  if ( m_ButtonFrame == NULL && showBorder && !m_UseRoundedIcons )
  {
    m_ButtonFrame = new CoreGuiImageView( m_ObjectName, m_Position, m_Size, GuiResource_ButtonFrame );
    m_ButtonFrame->SetColor( Color::Black );
    AddChildWithLayer( m_ButtonFrame, LAYER_BORDER );
  }
  else if ( m_ButtonFrame != NULL && !showBorder )
  {
    RemoveChild( m_ButtonFrame );
    m_ButtonFrame = NULL;
  }
}

void CoreGuiButton::SetSize( float x, float y )
{
  CoreGuiBase::SetSize( x, y );
  if ( m_ButtonRoundBg != NULL )
  {
    m_ButtonRoundBg->SetSize( x, y );
  }
  if ( m_ButtonNormalBg != NULL )
  {
    m_ButtonNormalBg->SetSize( x, y );
  }
  m_Title->SetSize( x * 0.5f, y * m_Textscale );
  if ( m_GuiFrame1 != NULL )
  {
    m_GuiFrame1->SetSize( x - CoreGuiFrame::BORDER, y - CoreGuiFrame::BORDER );
    //m_GuiFrame2->SetSize( x, y );
  }
  if ( m_ButtonFrame != NULL )
  {
    m_ButtonFrame->SetSize( x, y );
  }
}

CoreGuiBase* CoreGuiButton::OnMouseButtonDown( int /*buttonId*/, float xPos, float yPos )
{
  if ( IsIntersecting( xPos, yPos ) )
  {
    m_IsDown = true;
    m_SignalButtonClicked( m_ButtonType, this );
//    m_SignalButtonClickedEx( this, m_ButtonType );
    return this;
  }
  return NULL;
}

CoreGuiBase* CoreGuiButton::OnMouseButtonUp( int /*buttonId*/, float /*xPos*/, float /*yPos*/ )
{
  if ( m_IsDown )
  {
    m_IsDown = false;
    m_SignalButtonReleased();
    return this;
  }
  return NULL;
}

bool CoreGuiButton::OnKeyDown( int keyCode  )
{
  if ( keyCode == m_Hotkey )
  {
    m_IsDown = true;
    m_SignalButtonClicked( m_ButtonType, this );
    return true;
  }
  return false;
}

void CoreGuiButton::SetHotkey( int hotkey )
{
  dfAssert( m_Hotkey == 0, "SetHotke: Hotkey already set!" );
  m_Hotkey = hotkey;
}

void CoreGuiButton::SetWireframe( bool isWireframe )
{
  CoreGuiBase::SetWireframe( isWireframe );
  if ( m_ButtonRoundBg != NULL )
  {
    m_ButtonRoundBg->SetWireframe( isWireframe );
  }
  else if ( m_ButtonNormalBg != NULL )
  {
    m_ButtonNormalBg->SetWireframe( isWireframe );
  }
}

void CoreGuiButton::SetAlpha( float alpha )
{
  CoreBaseObject::SetAlpha( alpha );
  if ( m_ButtonRoundBg != NULL )
  {
    m_ButtonRoundBg->SetAlpha( alpha );
  }
  else if ( m_ButtonNormalBg != NULL )
  {
    m_ButtonNormalBg->SetAlpha( alpha );
  }
}


void CoreGuiButton::SetColor( float red, float green, float blue )
{
  if ( m_ButtonRoundBg != NULL && m_Color == m_ButtonRoundBg->GetColor() )
  {
    m_ButtonRoundBg->SetColor( red, green, blue );
  }
  else if ( m_ButtonNormalBg != NULL && m_Color == m_ButtonNormalBg->GetColor() )
  {
    m_ButtonNormalBg->SetColor( red, green, blue );
  }

  CoreGuiBase::SetColor( red, green, blue );
  if ( m_GuiFrame1 != NULL )
  {
    m_GuiFrame1->SetColor( red * 0.5f, green* 0.5f, blue* 0.5f );
  }
  if ( m_GuiFrame2 != NULL )
  {
    m_GuiFrame2->SetColor( red* 1.0f, green* 1.0f, blue * 1.0f);
  }

  if ( m_ButtonFrame != NULL )
  {
    m_ButtonFrame->SetColor( red * 0.5f, green* 0.5f, blue* 0.5f );
  }
}

void CoreGuiButton::SetText( const String& text )
{
  m_Title->SetText( text );
}

void CoreGuiButton::SetTextScale( float scale )
{
  m_Title->SetTextScale( scale );
}

void CoreGuiButton::SetTextColor( const Color& color )
{
  m_Title->SetColor( color );
}

void CoreGuiButton::SetTextColor( float red, float green, float blue )
{
  m_Title->SetColor( red, green, blue );
}

Color CoreGuiButton::GetDefaultButtonColor( int buttonType )
{
  switch( buttonType )
  {
    case BUTTONTYPE_YES:
      return Color::GetNamedColor("positive");
    case BUTTONTYPE_NO:
      return Color::GetNamedColor("negative");
    case BUTTONTYPE_CLOSE:
      return Color::GetNamedColor("neutral");
  }
  return Color::Grey;
}

CoreGuiBase* CoreGuiButton::GetSolidGuiUnderMouse( float xPos, float yPos )
{
  if ( !m_Wireframe  && IsIntersecting( xPos, yPos ) )
  {
    return this;
  }
  return NULL;
}

void CoreGuiButton::SetFrameColor( const Color& color )
{
  if ( m_ButtonFrame != NULL )
  {
    m_ButtonFrame->SetColor( color );
  }
  m_FrameColor = color;
}
