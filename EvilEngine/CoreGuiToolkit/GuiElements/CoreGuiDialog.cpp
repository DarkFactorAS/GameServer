/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : CoreGuiDialog
* Description : Creates a window with some buttons on it.
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "PreCompile.h"
#include "CoreGuiDialog.h"
#include "EvilEngine/CoreGuiToolkit/BaseGuiElements/CoreGuiTextfield.h"
#include "EvilEngine/CoreRendering/RenderObjects/TextStringRenderObject.h"
#include "EvilEngine/CoreGuiToolkit/BaseGuiElements/CoreGuiImageView.h"

CoreGuiDialog::CoreGuiDialog() :
  CoreGuiWindow(),
  m_BodyText(NULL)
{
  SetLayer(LAYER_DIALOG);
  SetFlag(FLAGS_ISMODAL);

  m_WindowBackground->SetColor(Color::DarkGrey);
  m_WindowBackground->SetAlpha(0.9f);
}

CoreGuiDialog::CoreGuiDialog( const String& title, const String& message, const Vector2& position, const Vector2& size ) : 
  CoreGuiWindow( title, position, size, GuiResource_WindowBackground, true ),
  m_BodyText( NULL )
{
  SetLayer( LAYER_DIALOG );
  SetFlag( FLAGS_ISMODAL );

  //m_WindowBackground->SetLayer( ++m_Layer );
  m_WindowBackground->SetColor( Color::DarkGrey );
  m_WindowBackground->SetAlpha( 0.9f );

  SetMessage(message);
}

CoreGuiDialog::CoreGuiDialog( const String& title, const String& message, const Vector2& position, const Vector2& size, int buttonGroup ) : 
  CoreGuiWindow( title, position, size, GuiResource_WindowBackground, true ),
  m_BodyText( NULL )
{
  SetLayer( LAYER_DIALOG );
  SetFlag( FLAGS_ISMODAL );

  //m_WindowBackground->SetLayer( ++m_Layer );
  m_WindowBackground->SetColor( Color::DarkGrey );
  m_WindowBackground->SetAlpha( 0.9f );

  SetMessage( message );
  CreateButtonGroup( buttonGroup );
}

void CoreGuiDialog::SetMessage( const String& message )
{
  RemoveChild( m_BodyText );
  if (!message.empty())
  {
    Vector2 bodyPos( m_Position.x, m_Position.y - s_TitleSize * 1.25f);
    Vector2 bodySize( m_Size.x - CoreGuiWindow::BORDER - CoreGuiWindow::BORDER, s_TitleSize * 1.25f);
    m_BodyText = new CoreGuiTextfield(message, bodyPos, bodySize, TextStringRenderObject::ALIGNMENT_CENTER, Color::White);
    m_BodyText->SetFlag(FLAGS_NOTSCALECHILD);
    AddChild(m_BodyText);
  }
}

void CoreGuiDialog::CreateButtonGroup( int buttonGroup )
{
  switch( buttonGroup )
  {
    case CoreGuiButton::BUTTONGROUP_YESNO:
      {
        CoreGuiButton* yesBtn = AddYesButton();
        CoreGuiButton* noBtn  = AddNoButton();
        yesBtn->m_SignalButtonClicked.Connect( this, &CoreGuiWindow::CloseWindow );
        noBtn->m_SignalButtonClicked.Connect( this, &CoreGuiWindow::CloseWindow );
      }
      break;

    case CoreGuiButton::BUTTONGROUP_RETRYCANCEL:
      {
        CoreGuiButton* yesBtn = AddYesButton("RETRY");
        CoreGuiButton* noBtn  = AddNoButton("CANCEL");
        yesBtn->m_SignalButtonClicked.Connect( this, &CoreGuiWindow::CloseWindow );
        noBtn->m_SignalButtonClicked.Connect( this, &CoreGuiWindow::CloseWindow );
      }
      break;

    case CoreGuiButton::BUTTONGROUP_CANCEL:
      {
        CoreGuiButton* noBtn = AddNoButton("CANCEL");
        noBtn->m_SignalButtonClicked.Connect( this, &CoreGuiWindow::CloseWindow );
      }
      break;

    case CoreGuiButton::BUTTONGROUP_ABORT:
      {
        CoreGuiButton* noBtn = AddNeutralButton("CANCEL");
        noBtn->SetColor( noBtn->GetDefaultButtonColor( CoreGuiButton::BUTTONTYPE_NO ) );
        noBtn->m_SignalButtonClicked.Connect( this, &CoreGuiWindow::CloseWindow );
      }
      break;

    case CoreGuiButton::BUTTONGROUP_OK:
      {
        CoreGuiButton* noBtn = AddNeutralButton("OK");
        noBtn->m_SignalButtonClicked.Connect( this, &CoreGuiWindow::CloseWindow );
      }
      break;

    case CoreGuiButton::BUTTONGROUP_CLOSE:
    default:
      {
        CoreGuiButton* noBtn = AddNeutralButton("CLOSE");
        noBtn->m_SignalButtonClicked.Connect( this, &CoreGuiWindow::CloseWindow );
      }
      break;
  }
}

/************************************************************************************************
* OnMouseButtonDown:
* Dialog windows will steal all mouse clicks regardless if they are inside the window or not
*
* @param  (int)           buttonId     - ID of the mouse button
* @param  (int)           xPos         - X Position of the mouse when clicking
* @param  (int)           yPos         - Y Position of the mouse when clicking
* @return (CoreGuiBase*)               - Returns the focus object for the mouse when clicking here
*
* @author Thor Richard Hansen
*************************************************************************************************/
CoreGuiBase* CoreGuiDialog::OnMouseButtonDown( int buttonId, float xPos, float yPos )
{
  CoreGuiBase* focusObject = CoreGuiWindow::OnMouseButtonDown( buttonId, xPos, yPos );
  if ( focusObject == NULL )
  {
    return this;
  }
  return focusObject;
}
