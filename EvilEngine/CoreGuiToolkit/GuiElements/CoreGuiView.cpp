/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : CoreGuiView
* Description : Creates a Gui view that can have multiple guiobject children. This class will 
*               handle all drawing, moving, sizing etc for all children. This class also takes
*               ownership of the GuiObjects that are added to it.
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "PreCompile.h"
#include "CoreGuiView.h"
#include "../BaseGuiElements/CoreGuiImageView.h"
#include "../CoreGuiToolkit.h"

float CoreGuiView::s_FrameSize = 0.04f;
float CoreGuiView::s_TitleSize = 0.04f;
float CoreGuiView::BORDER      = 0.006f;

CoreGuiView::CoreGuiView( const Vector3& position, const Vector3& size ) : 
  CoreGuiBase( "CoreGuiView", position, size )
{
}

CoreGuiView::CoreGuiView( const String& name, const Vector3& position, const Vector3& size ) : 
  CoreGuiBase( name, position, size )
{
}

CoreGuiView::~CoreGuiView()
{
  m_ButtonList.clear();
}

CoreGuiBase* CoreGuiView::OnMouseButtonDown( int buttonId, float xPos, float yPos )
{
  for ( size_t index = m_GuiObjects.size(); index > 0; index-- )
  {
    CoreGuiBase* guiObject   = ( CoreGuiBase* ) m_GuiObjects[ index - 1 ];
    if ( guiObject != NULL && guiObject->IsVisible() )
    {
      CoreGuiBase* focusObject = guiObject->OnMouseButtonDown( buttonId, xPos, yPos );
      if ( focusObject != NULL )
      {
        return focusObject;
      }
    }
  }
  return NULL;
}

/************************************************************************************************
* AddYesButton/AddNoButton/AddNeutral:
* Add a generic yes/no/neutral button that is green/red/yellow
*
* @author Thor Richard Hansen
*************************************************************************************************/
CoreGuiButton* CoreGuiView::AddYesButton()
{
  return AddYesButton( "Yes" );
}
CoreGuiButton* CoreGuiView::AddYesButton( const String& buttonTitle )
{
  return AddButton( buttonTitle, CoreGuiButton::BUTTONTYPE_YES, Vector2( GetRight() - CoreGuiButton::s_DefaultWidth - s_FrameSize, GetBottom() - CoreGuiButton::s_DefaultHeight - s_FrameSize ) );
}
CoreGuiButton* CoreGuiView::AddNoButton()
{
  return AddNoButton( "No" );
}
CoreGuiButton* CoreGuiView::AddNoButton( const String& buttonTitle )
{
  return AddButton( buttonTitle, CoreGuiButton::BUTTONTYPE_NO, Vector2( GetLeft() + CoreGuiButton::s_DefaultSize.x + s_FrameSize, GetBottom() - CoreGuiButton::s_DefaultSize.y - s_FrameSize ) );
}
CoreGuiButton* CoreGuiView::AddNeutralButton( const String& buttonTitle )
{
  return AddButton( buttonTitle, CoreGuiButton::BUTTONTYPE_CLOSE, Vector2( GetPosition().x, GetBottom() - CoreGuiButton::s_DefaultSize.y - s_FrameSize ) );
}

/************************************************************************************************
* AddYesButton:
* Add a generic button
*
* @param  (CoreGuiButton*)  guiButton - The button to add
*
* @author Thor Richard Hansen
*************************************************************************************************/
CoreGuiButton* CoreGuiView::AddButton( const String& buttonTitle, int buttonType, const Vector3& position )
{
  CoreGuiButton* guiButton = (CoreGuiButton*) CoreGuiToolkit::InstantiateGui("Button");
  guiButton->SetButtonType( buttonType );
  guiButton->SetText( buttonTitle );
  guiButton->SetPosition( position );
  guiButton->SetSize( CoreGuiButton::s_DefaultWidth, CoreGuiButton::s_DefaultHeight );
  guiButton->SetResourceId( GuiResource_ButtonBackground );
  guiButton->SetColor( guiButton->GetDefaultButtonColor( buttonType ) );

  AddButton( guiButton );
  return guiButton;
}

CoreGuiButton* CoreGuiView::AddButton( CoreGuiButton* guiButton )
{
  guiButton->m_SignalButtonClicked.Connect( this, &CoreGuiView::BtnPressed );

  AddChild( guiButton );
  m_ButtonList.push_back( guiButton );
  return guiButton;
}

void CoreGuiView::BtnPressed( int btn )
{
  m_SignalButtonDown( btn );
}

void CoreGuiView::SetHotkey( int buttonType, int hotkey )
{
  for ( size_t index = 0; index < m_ButtonList.size(); index++ )
  {
    CoreGuiButton* guiButton   = ( CoreGuiButton* ) m_ButtonList[ index ];
    if ( guiButton->GetButtonType() == buttonType )
    {
      guiButton->SetHotkey( hotkey );
      break;
    }
  }
}
