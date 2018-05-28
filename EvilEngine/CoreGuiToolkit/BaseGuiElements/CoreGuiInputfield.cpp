/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : CoreGuiInputfield
* Description : Creates the render and handling of a inputfield on the screen. This also handles
*               password fields and normal input fields.
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "PreCompile.h"

#include "EvilEngine/CoreRendering/RenderObjects/TextStringRenderObject.h"
#include "EvilEngine/CoreRendering/OpenGLRender/CoreOpenGLRender.h"
#include "EvilEngine/CoreEngine/CoreInput/Keyboard.h"

#include "CoreGuiInputfield.h"
#include "CoreGuiFrame.h"
#include "CoreGuiTextfield.h"
#include "CoreGuiImageView.h"
#include "../CoreGuiToolkit.h"

#include <stdlib.h>

float CoreGuiInputfield::BORDER = 0.3f;

CoreGuiInputfield::CoreGuiInputfield( const String& text, const Vector2& position, const Vector2& size ) : 
  CoreGuiBase( "CoreGuiInputfield", position, size )
{
  Init( text, position, size, TextStringRenderObject::ALIGNMENT_CENTER );  
}

CoreGuiInputfield::CoreGuiInputfield( const String& text, const Vector2& position, const Vector2& size, int textAlignment ) : 
  CoreGuiBase( "CoreGuiInputfield", position, size )
{
  Init( text, position, size, textAlignment );
}

void CoreGuiInputfield::Init(const String& text, const Vector2& position, const Vector2& size, int textAlignment)
{
  m_Text = text;
  m_DisplayText = text;
  m_IsPassword = false;
  m_UpdateCursor = false;
  m_IsEnabled = true;
  m_UseMultiline = false;
  m_UseTextAlpha = false;

  m_FrameColor1         = Color::GetNamedColor(StaticStr("input_bordercolor"));
  m_FrameColor2         = Color::GetNamedColor(StaticStr("input_activebordercolor"));
  m_TextColorInactive   = Color::GetNamedColor(StaticStr("input_textcolor"));
  m_TextColorActive     = Color::GetNamedColor(StaticStr("input_activetextcolor"));
  m_TextColorTip        = Color::GetNamedColor(StaticStr("input_tooltip"));
  m_ReplacePwd = "*";

  Vector2 innerSize = Vector2(size.x - 0.03f, size.y - (size.y * CoreGuiInputfield::BORDER));

  m_Background = new CoreGuiImageView(position, size, GuiResource_White);
  m_Textfield = new CoreGuiTextfield(m_DisplayText, position, innerSize, textAlignment, m_TextColorInactive);
  m_Frame = new CoreGuiFrame(position, size, m_FrameColor1);
  m_Cursor = NULL;

  AddChild(m_Background);
  AddChild(m_Textfield);
  AddChild(m_Frame);
}

CoreGuiInputfield::CoreGuiInputfield( const String& fieldName, const String& fieldValue, const Vector2& position, const Vector2& size, int textAlignment ) : 
  CoreGuiBase( fieldName, position, size )
{
  m_Text              = fieldValue;
  m_DisplayText       = fieldValue;
  m_IsPassword        = false;
  m_UpdateCursor      = false;
  m_IsEnabled         = true;
  m_UseMultiline      = false;

  m_UseTextAlpha      = false;

  m_FrameColor1       = Color::Black;
  m_FrameColor2       = Color::Red;
  m_TextColorInactive = Color::Black;
  m_TextColorActive   = Color::Red;
  m_TextColorTip      = Color::LightGrey;

  Vector2 innerSize   = Vector2( size.x - CoreGuiTextfield::BORDER, size.y - CoreGuiTextfield::BORDER );

  m_Background        = new CoreGuiImageView( position, size, GuiResource_White );
  m_Textfield         = new CoreGuiTextfield( m_DisplayText, position, innerSize, textAlignment, m_TextColorInactive );
  m_Frame             = new CoreGuiFrame( position, size, m_FrameColor1 );
  m_Cursor            = NULL;

  AddChild( m_Background );
  AddChild( m_Textfield );
  AddChild( m_Frame );
}

CoreGuiInputfield::~CoreGuiInputfield()
{
  delete m_Cursor;
}

void CoreGuiInputfield::SetMember( const String& key, const String& value )
{
  if ( key.EqualsWithCase("name" ) )
  {
    SetName( value );
  }
  else if ( key.EqualsWithCase("password" ) )
  {
    if ( !value.EqualsWithCase("false") )
    {
      SetPassword( true );
    }
  }
  else if ( key.Equals("multiline") )
  {
    m_UseMultiline      = value.Equals("true");
  }
  // Forward align to textfield
  else if ( key.EqualsWithCase("align" ) )
  {
    m_Textfield->SetMember( key, value );
  }
  else if ( key.CompareWithCase("textalpha" ) == 0 )
  {
    float alpha = (float)atof( value.c_str() );
    SetTextAlpha( alpha );
  }  
  else if ( key.CompareWithCase("textcolor" ) == 0 )
  {
    SetTextColor( Color::FromString( value ) );
  }
  else if (key.CompareWithCase("backgroundcolor") == 0)
  {
    SetBackgroundColor(Color::FromString(value));
  }
  else if (key.CompareWithCase("bordercolor") == 0)
  {
    SetBorderColor(Color::FromString(value));
  }
  else if (key.CompareWithCase("activebordercolor") == 0)
  {
    SetActiveBorderColor(Color::FromString(value));
  }
  else if (key.CompareWithCase("activetextcolor") == 0)
  {
    SetActiveTextColor(Color::FromString(value));
  }
  else if (key.CompareWithCase("tooltipcolor") == 0)
  {
    SetTooltipColor(Color::FromString(value));
  }
  else if ( key.EqualsWithCase("title" ) )
  {
    SetText( value );
  }
  else if ( key.EqualsWithCase("value" ) )
  {
    SetText( value );
  }
  else if ( key.EqualsWithCase("tooltip" ) )
  {
    SetTooltip( value );
  }
  else if ( key.EqualsWithCase( "textscale" ) )
  {
    m_Textfield->SetMember( key, value );
  }
  else
  {
    CoreGuiBase::SetMember( key, value );
  }
}

/************************************************************************************************
* SetPassword:
* Changes the input field to be of type password. This will display all characters as stars.
* When the user types NEW characters they are displayed in cleartext until the field looses focus
*
* @author Thor Richard Hansen
*************************************************************************************************/
void CoreGuiInputfield::SetPassword( bool isPasswordField )
{
  if ( m_IsPassword != isPasswordField )
  {
    m_IsPassword  = isPasswordField;
    UpdateText();
  }
}

/************************************************************************************************
 * SetText:
 * Sets the value of the text
 *
 * @param  (const String&)   text         - The new text
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
void CoreGuiInputfield::SetText( const String& text )
{
  m_Text = text;
  UpdateText();
}

/************************************************************************************************
 * SetText:
 * Sets the value of the tooltip text
 *
 * @param  (const String&)   text         - The new text for tooltip
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
void CoreGuiInputfield::SetTooltip( const String& text )
{
  m_Tooltip = text;
  UpdateText();
}

/************************************************************************************************
 * SetValue:
 * Sets the value of the text
 *
 * @param  (uint32)   val         - The numeric value of the text
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
void CoreGuiInputfield::SetValue( uint32 val )
{
  m_Text = String::FormatString("%d", val );
  UpdateText();
}
void CoreGuiInputfield::SetValue( float val )
{
  m_Text = String::FormatString("%0.2f", val );
  UpdateText();
}

void CoreGuiInputfield::SetTextAlpha( float alpha )
{
  m_UseTextAlpha = true;
  m_Textfield->SetAlpha( alpha );
}


float CoreGuiInputfield::GetFloatValue()
{
  float f = (float) atof( m_Text.c_str() );
  return f;
}

void CoreGuiInputfield::SetColor( float red, float green, float blue )
{
  CoreGuiBase::SetColor( red, green, blue );
  m_Background->SetColor( red, green, blue );
}

void CoreGuiInputfield::SetAlpha( float alpha )
{
  m_Background->SetAlpha( alpha );
  m_Frame->SetAlpha( alpha );

  if ( m_Cursor != NULL )
  {
    m_Cursor->SetAlpha( alpha );
  }

  if ( !m_UseTextAlpha )
  {
    m_Textfield->SetAlpha( alpha );
  }
}

void CoreGuiInputfield::SetTextColor( const Color& color )
{
  m_TextColorInactive = color;
  if ( m_Cursor == NULL )
  {
    m_Textfield->SetColor( color );
  }
}

void CoreGuiInputfield::SetActiveTextColor( const Color& color )
{
  m_TextColorActive = color;
  if ( m_Cursor != NULL )
  {
    m_Textfield->SetColor( color );
    m_Cursor->SetColor( color );
  }
}


CoreGuiBase* CoreGuiInputfield::OnMouseButtonDown( int /*buttonId*/, float xPos, float yPos )
{
  if ( !m_IsEnabled )
  {
    return NULL;
  }

  if ( IsIntersecting( xPos, yPos ) )
  {
    return this;
  }
  return NULL;
}

CoreGuiBase* CoreGuiInputfield::GetSolidGuiUnderMouse( float xPos, float yPos )
{
  if ( !m_Wireframe  && IsIntersecting( xPos, yPos ) )
  {
    return this;
  }
  return NULL;
}

bool CoreGuiInputfield::OnKeyDown( int keyCode )
{
  if ( !m_IsEnabled || m_Cursor == NULL )
  {
    return false;
  }

  switch( keyCode )
  {
    case Keyboard::KEY_ESCAPE:
      {
        m_LooseFocus( this );
      }
      break;
    case Keyboard::KEY_RETURN:
    case Keyboard::KEY_ENTER:
      {
        // Only loose focus if I actually have focus
        if ( m_UseMultiline )
        {
          char character = (char) keyCode;
          if ( character == 13 ){ character = 10; }
          m_Text += character;
          UpdateText();
        }
        else
        {
          m_LooseFocus( this );
          m_ActionEvent( 1, this );
        }
      }
      break;
    case Keyboard::KEY_BACKSPACE:
    //case 0x08:
      {
        if ( !m_Text.empty() )
        {
          m_Text = m_Text.SubString( 0, m_Text.length() - 1 );
        }
        UpdateText();
      }
      break;

    default:
      {
        char character = (char) keyCode;
        if ( character >= ' ' && character <= 'z' )
        {
          m_Text += character;
          UpdateText();
        }
      }
      break;
  }
  return true;
}

bool CoreGuiInputfield::OnKeyUp( int /*keyCode*/ )
{
  //m_DisplayText += keyCode;
  return true;
}

bool CoreGuiInputfield::GainFocus()
{
  if ( !m_IsEnabled )
  {
    return false;
  }

  m_Frame->SetColor( m_FrameColor2 );
  //m_Textfield->SetColor( m_TextColorActive );

  if ( m_Cursor == NULL )
  {
    float charSize = 0.0f;
    m_Cursor = (CoreGuiTextfield*) m_Textfield->AddChild( new CoreGuiTextfield( "|", Vector2( m_Textfield->GetPosition().x + m_Textfield->GetTextWidth( 0 ) + charSize, m_Textfield->GetPosition().y), m_Textfield->GetSize(), TextStringRenderObject::ALIGNMENT_CENTERLEFT, m_TextColorActive ) );
    m_UpdateCursor = true;
  }
  UpdateText();
  UpdateCursor();
  return true;
}

void CoreGuiInputfield::LooseFocus()
{
//  m_Textfield->SetColor( m_TextColorInactive );
  m_Frame->SetColor( m_FrameColor1 );
  m_Cursor = (CoreGuiTextfield*) m_Textfield->RemoveChild( m_Cursor );  

  UpdateText();
}

void CoreGuiInputfield::SetBackgroundColor( const Color& color )
{
  m_Background->SetColor( color );
}

void CoreGuiInputfield::SetBorderColor( const Color& color )
{
  m_FrameColor1 = color;
  m_Frame->SetColor( color );
}

void CoreGuiInputfield::SetActiveBorderColor(const Color& color)
{
  m_FrameColor2 = color;
  //m_Frame->SetColor(color);
}

void CoreGuiInputfield::SetTooltipColor(const Color& color)
{
  m_TextColorTip = color;
}

void CoreGuiInputfield::SetEnabled( bool isEnabled )
{
  LooseFocus();
  m_IsEnabled = isEnabled;
}

void CoreGuiInputfield::UpdateText()
{
  if ( m_Cursor == NULL && m_Text.empty() )
  {
    m_DisplayText = m_Tooltip;
    m_Textfield->SetText( m_DisplayText );
    m_Textfield->SetColor( m_TextColorTip );
  }
  else if ( m_IsPassword )
  {
    m_DisplayText = "";
    for ( size_t i = 0; i < m_Text.length(); i++ )
    {
      m_DisplayText += m_ReplacePwd;
    }
    m_Textfield->SetText( m_DisplayText );
    m_Textfield->SetColor( m_Cursor != NULL ? m_TextColorActive : m_TextColorInactive );
  }
  else
  {
    m_DisplayText = m_Text;
    m_Textfield->SetText( m_DisplayText );
    m_Textfield->SetColor( m_Cursor != NULL ? m_TextColorActive : m_TextColorInactive );
  }

  if ( m_Cursor != NULL )
  {
    m_UpdateCursor = true; 
  }

  //if ( m_Cursor != NULL )
  //{
  //  float charSize = 0.0f;
  //  m_Cursor->SetPosition( m_Textfield->GetPosition().x + m_Textfield->GetTextWidth( 0 ) + charSize, m_Textfield->GetPosition().y );
  //}
}

void CoreGuiInputfield::UpdateCursor()
{
  if ( m_Cursor == NULL )
  {
    return;
  }

  if ( m_UpdateCursor )
  {
    m_UpdateCursor = false;
    float charSize = 0.0f;
    switch( m_Textfield->GetTextAlignment() )
    {
    case TextStringRenderObject::ALIGNMENT_CENTER:
    case TextStringRenderObject::ALIGNMENT_TOPCENTER:
      m_Cursor->SetPosition( m_Textfield->GetRight() + ( m_Textfield->GetTextWidth( 0 ) * 0.5f) + charSize, m_Textfield->GetPosition().y );
      break;

    case TextStringRenderObject::ALIGNMENT_TOPLEFT:
    case TextStringRenderObject::ALIGNMENT_CENTERLEFT:
      m_Cursor->SetPosition( m_Textfield->GetPosition().x + m_Textfield->GetTextWidth( 0 ) + charSize, m_Textfield->GetPosition().y );
      break;

    case TextStringRenderObject::ALIGNMENT_CENTERRIGHT:
      break;
    }
  }
}

bool CoreGuiInputfield::OnDraw( const Matrix& projection )
{
  // Make the cursor blink
  bool update = CoreGuiBase::OnDraw( projection );
  if ( update )
  {
    UpdateCursor();
  }
  return update;
}
