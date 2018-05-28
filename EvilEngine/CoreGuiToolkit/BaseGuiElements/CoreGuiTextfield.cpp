/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : CoreGuiTextfield
* Description : Creates the render and handling of a text view/field on the screen.
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "PreCompile.h"
#include "CoreGuiTextfield.h"
#include "../../CoreRendering/RenderObjects/TextStringRenderObject.h"
#include "../CoreGuiToolkit.h"
#include "CoreGuiFrame.h"

float CoreGuiTextfield::HEIGHT = 0.05f;
float CoreGuiTextfield::BORDER = 0.01f;

CoreGuiTextfield::CoreGuiTextfield( const String& text, const Vector2& position, const Vector2& size ) : 
  CoreGuiBase( "CoreGuiTextfield", position, size )
{
  Color textColor = Color::GetNamedColor( StaticStr("textcolor") );
  Init( text, position, size, TextStringRenderObject::ALIGNMENT_CENTERLEFT, textColor );
}

CoreGuiTextfield::CoreGuiTextfield( const String& text, const Vector2& position, const Vector2& size, int textAlignment ) : 
  CoreGuiBase( "CoreGuiTextfield", position, size )
{
  Color textColor = Color::GetNamedColor(StaticStr("textcolor"));
  Init( text, position, size, textAlignment, textColor );
}

CoreGuiTextfield::CoreGuiTextfield ( const String& text, const Vector2& position, const Vector2& size, int textAlignment, const Color& color ) :
  CoreGuiBase( "CoreGuiTextfield", position, size )
{
  Init( text, position, size, textAlignment, color );
}

void CoreGuiTextfield::Init( const String& text, const Vector2& position, const Vector2& size, int textAlignment, const Color& textColor )
{
  m_Text              = text;
  m_Text.Replace("|", "\n");

  //Vector3 vec3Position = Vector3( position.x, position.y, m_Position.z );
  Vector2 innerSize    = Vector2( size.x - CoreGuiTextfield::BORDER, size.y - CoreGuiTextfield::BORDER );

  m_Textfield          = new TextStringRenderObject( m_Text, position, innerSize, GuiResource_FontDynamic );
  m_Textfield->SetColor( textColor );
  m_Textfield->SetAlignment( textAlignment );

  AddRenderObject( m_Textfield );
}

CoreGuiTextfield::~CoreGuiTextfield()
{
  //delete m_Textfield;
}

void CoreGuiTextfield::SetWordWrap(int numCharacters)
{
  if ( m_Textfield != NULL )
  {
    m_Textfield->SetWordWrap( numCharacters );
  }
}

void CoreGuiTextfield::SetMember( const String& key, const String& value )
{
  if ( key.EqualsWithCase("value" ) )
  {
    SetText( value );
  }
  else if ( key.EqualsWithCase("text" ) )
  {
    SetText( value );
  }
  else if ( key.EqualsWithCase("wordwrap") )
  {
    int numCharacters = atoi( value.c_str() );
    m_Textfield->SetWordWrap( numCharacters );
  }
  else if ( key.EqualsWithCase("align" ) )
  {
    if ( value.EqualsWithCase("center-right") )
    {
      SetAlignment( TextStringRenderObject::ALIGNMENT_CENTERRIGHT );
    }
    else if ( value.EqualsWithCase("center") )
    {
      SetAlignment( TextStringRenderObject::ALIGNMENT_CENTER );
    }
    else if ( value.EqualsWithCase("center-left") )
    {
      SetAlignment( TextStringRenderObject::ALIGNMENT_CENTERLEFT );
    }
    else if ( value.EqualsWithCase("top-left"))
    {
      SetAlignment( TextStringRenderObject::ALIGNMENT_TOPLEFT );
    }
    else if ( value.EqualsWithCase("top-center"))
    {
      SetAlignment( TextStringRenderObject::ALIGNMENT_TOPCENTER );
    }
    else if ( value.EqualsWithCase("top-right"))
    {
      SetAlignment( TextStringRenderObject::ALIGNMENT_TOPRIGHT );
    }
  }
  else if ( key.EqualsWithCase( "textscale" ) )
  {
    float scale = (float)atof( value.c_str() );
    if ( scale >= 0.0f )
    {
      SetTextScale( scale );
    }
  }
  else
  {
    CoreGuiBase::SetMember( key, value );
  }
}

void CoreGuiTextfield::SetAlignment( int textAlignment )
{
  m_Textfield->SetAlignment( textAlignment );
}

void CoreGuiTextfield::SetText( const String& text )
{
  if ( !m_Text.EqualsWithCase(text) )
  {
    m_Text = text;
    m_Text.Replace("|", "\n");
    m_Textfield->SetText( m_Text );
  }
}

float CoreGuiTextfield::GetTextWidth( size_t lineIndex )
{
  return m_Textfield->GetTextWidth( lineIndex );
}

void CoreGuiTextfield::SetTextScale( float scale )
{
  m_Textfield->SetTextScale( scale );
}

int CoreGuiTextfield::GetTextAlignment()
{
  if ( m_Textfield != NULL )
  {
    return m_Textfield->GetTextAlignment();
  }
  return TextStringRenderObject::ALIGNMENT_CENTER;
}

void CoreGuiTextfield::SetPosition( float x, float y )
{
  CoreGuiBase::SetPosition( x, y );
  m_Textfield->SetPosition(  x, y, m_Textfield->GetPosition().z );
}

void CoreGuiTextfield::SetSize( float x, float y )
{
  CoreGuiBase::SetSize( x, y );
  m_Textfield->SetSize( x - CoreGuiTextfield::BORDER, y - CoreGuiTextfield::BORDER, 0 );
}

void CoreGuiTextfield::SetColor( const Color& color )
{
  SetColor( color.x, color.y, color.z ); 
  SetAlpha( color.w );
}

void CoreGuiTextfield::SetColor( float red, float green, float blue )
{
  CoreGuiBase::SetColor( red, green, blue );
  m_Textfield->SetColor( red, green, blue );
}

void CoreGuiTextfield::SetAlpha( float alpha )
{
  CoreGuiBase::SetAlpha( alpha );
  m_Textfield->SetAlpha( alpha );
}
