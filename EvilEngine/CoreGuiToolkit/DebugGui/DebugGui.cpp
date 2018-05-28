
#include "PreCompile.h"
#include "DebugGui.h"
#include "GuiEditor/GuiEditor.h"
#include "../CoreGuiToolkitInclude.h"
#include "../CoreGuiToolkit.h"

#include "EvilEngine/CoreEngine/CoreInput/CoreInputManager.h"
#include "EvilEngine/CoreEngine/CoreInput/Touch.h"
#include "EvilEngine/CoreRendering/OpenGLRender/CoreOpenGLRender.h"
#include "EvilEngine/CoreRendering/RenderObjects/RenderObject.h"
#include "EvilEngine/CoreRendering/CoreTextureManager/CoreTextureManager.h"
#include "EvilEngine/CoreEngine/CoreResourceManager/CoreResourceManager.h"
#include "EvilEngine/CoreRendering/Camera/RenderCamera.h"


bool DebugGui::s_Console = false;

DebugGui::DebugGui( CoreEngine& parent, bool autoOpen ) :
  CoreGuiBase( "DebugGUI", Vector2::s_Vec2Zero, Vector2::s_Vec2Zero ),
  m_CoreEngine( parent ),
  m_DebugMode( DEBUGMODE_HIDDEN ),
  m_ActiveObject( NULL ),
  m_RevertObject( NULL ),
  m_Menu( NULL )
{
  SetLayer( CoreGuiBase::LAYER_DEBUGGUI );

  if ( autoOpen )
  {
    ToggleDebug();
  }
  parent.SignalGlobalHotkey.Connect( this, &DebugGui::SlotHotkeys );
}

void DebugGui::SlotHotkeys( int hotkey )
{
  switch( hotkey )
  {
    case 'd':
      ToggleDebug();
      break;
    case 'c':
      CenterCamera();
      break;
  }
}

void DebugGui::SlotOnMouseLongTouch( const Vector2& position )
{
  float rightBorder   = 1.0f;
  float topBorder = -1.0f;
  if ( position.x > rightBorder - 0.2f && position.y < topBorder + 0.2f )
  {
    ToggleDebug();
  }
}

void DebugGui::SlotCloseMenu( int /*btnId*/ )
{
  if (m_DebugMode != DEBUGMODE_HIDDEN)
  {
    ToggleDebug();
  }
}

void DebugGui::CenterCamera()
{
  RenderCamera::GetInstance()->Center();
}

void DebugGui::ToggleDebug()
{
#if !defined DEBUG && defined WINDOWS
  if ( !s_Console )
  {
    return;
  }
#endif

  if ( m_DebugMode == DEBUGMODE_EDITGUI )
  {
    if ( m_MainButton != NULL )
    {
      RemoveButtons();
      RemoveMainButton();
    }
    else
    {
      m_MainButton = CreateMenuButton(FLAG_NONE, "*", 0, 0, GuiResource_ButtonBackground, &DebugGui::ShowButtons, false);
      AddChild(m_MainButton);
    }
  }
  else if ( m_DebugMode == DEBUGMODE_HIDDEN )
  {
    RemoveAllChildren();
    m_MainButton = CreateMenuButton(FLAG_NONE, "*", 0, 0, GuiResource_ButtonBackground, &DebugGui::ShowButtons, false );
    AddChild( m_MainButton );
    m_DebugMode = DEBUGMODE_NONE;

    if ( m_Menu != NULL )
    {
      RemoveButtons();
      ShowButtons(0);
    }
  }
  else 
  {
    RemoveAllChildren();
    m_DebugMode = DEBUGMODE_HIDDEN;
    m_MainButton = NULL;
  }
}

void DebugGui::ShowButtons( int /*btnId*/ )
{
  if ( m_Menu == NULL )
  {
    m_Menu = new CoreGuiBase( "DebugMenu" );
    m_Menu->AddChild( CreateMenuButton(FLAG_NONE,"X", 1, 0, GuiResource_ButtonBackground, &DebugGui::SlotCloseMenu, false ) );

    m_Menu->AddChild( CreateMenuButton(FLAG_DEBUG, "Wire|Single", 0, 1, GuiResource_ButtonBackground, &DebugGui::SlotToggleSingleWireframe, m_DebugMode == DEBUGMODE_SINGLEWIREFRAME ) );
    m_Menu->AddChild( CreateMenuButton(FLAG_DEBUG, "Wire|All", 1, 1, GuiResource_ButtonBackground, &DebugGui::SlotToggleAllWireframe, RenderObject::s_DrawWireframe ) );
    m_Menu->AddChild( CreateMenuButton(FLAG_DEBUG, "Invisible|Alpha", 2, 1, GuiResource_ButtonBackground, &DebugGui::SlotToggleDrawInvisibleAlpha, RenderObject::s_DrawInvisibleAlpha ) );
    m_Menu->AddChild( CreateMenuButton(FLAG_DEBUG, "Edit", 3, 1, GuiResource_ButtonBackground, &DebugGui::SlotEditGui, m_DebugMode == DEBUGMODE_EDITGUI ) );

    m_Menu->AddChild( CreateMenuButton(FLAG_DEBUG, "Release|Texures", 0, 2, GuiResource_ButtonBackground, &DebugGui::SlotReleaseTextures, false ) );
    m_Menu->AddChild( CreateMenuButton(FLAG_DEBUG, "Reinit|Screen", 1, 2, GuiResource_ButtonBackground, &DebugGui::SlotReinitScreen, false ) );
    m_Menu->AddChild( CreateMenuButton(FLAG_DEBUG, "Reload|Resources", 2, 2, GuiResource_ButtonBackground, &DebugGui::SlotReloadResources, false ) );

    AddChild( m_Menu );
  }
  else
  {
    RemoveButtons();
  }
}

void DebugGui::RemoveButtons()
{
  RemoveChild( m_Menu );
  m_Menu = NULL;
}

void DebugGui::RemoveMainButton()
{
  RemoveChild(m_MainButton);
  m_MainButton = NULL;
}

//CoreGuiButton* DebugGui::CreateMenuButton( const String& title, int x, int y, uint32 resourceId, dfDebugGuiButtonCallback callback, bool showFrame )
//{
//  float width  = 0.06f;
//  float height = 0.05f;//CoreOpenGLRender::GetRatio() * 0.035f;
//  CoreGuiButton* btn = new CoreGuiButton( title, Vector2( -0.98f + width + ( x * width * 2.2f ), -0.9f - height + ( y * height * 2.2f ) ), Vector2( width, height ), resourceId );
//  btn->SetAlpha( 0.5f );
//  btn->SetDebugGui( true );
//  btn->m_SignalButtonClicked.Connect( this, callback );
//  if ( showFrame )
//  {
//    btn->AddDebugFrame( Color::Red );
//  }
//  return btn;
//}

void DebugGui::StartSeperateWindow( CoreGuiBase* object )
{
  //CoreGuiToolkit::GetInstance()->AddGuiObject(object);
  AddChild( object );
  RemoveButtons();
  RemoveMainButton();
}

void DebugGui::CloseEditGui()
{
  if ( m_DebugMode == DEBUGMODE_EDITGUI )
  {
    SetMode( DEBUGMODE_NONE );
    RemoveButtons();
  }
}

void DebugGui::SlotEditGui( int /* btnId */ )
{
  if ( m_DebugMode == DEBUGMODE_EDITGUI )
  {
    SetMode( DEBUGMODE_NONE );
  }
  else
  {
    SetMode( DEBUGMODE_EDITGUI );

    if ( GetChildWithName("GuiEditor") == NULL )
    {
      GuiEditor* editor = new GuiEditor();
      editor->SetName("GuiEditor");
      editor->SignalWindowClosed.Connect( this, &DebugGui::CloseEditGui );
      SignalActiveGui.Connect( editor, &GuiEditor::SetActiveParent );

      StartSeperateWindow( editor );
    }
  }
}

void DebugGui::SlotToggleSingleWireframe( int /* btnId */ )
{
  if ( m_DebugMode == DEBUGMODE_SINGLEWIREFRAME )
  {
    SetMode( DEBUGMODE_NONE );
  }
  else
  {
    SetMode( DEBUGMODE_SINGLEWIREFRAME );
  }
}

void DebugGui::SlotToggleAllWireframe( int /* btnId */ )
{
  RenderObject::s_DrawWireframe = !RenderObject::s_DrawWireframe;
  RemoveButtons();
  ShowButtons( 0 );
}

void DebugGui::SlotToggleDrawInvisibleAlpha( int /* btnId */ )
{
  RenderObject::s_DrawInvisibleAlpha = !RenderObject::s_DrawInvisibleAlpha;
  RemoveButtons();
  ShowButtons( 0 );
}

void DebugGui::SlotToggleMoveGui( int /* btnId */ )
{
  if ( m_DebugMode == DEBUGMODE_MOVEGUI )
  {
    SetMode( DEBUGMODE_NONE );
  }
  else
  {
    SetMode( DEBUGMODE_MOVEGUI );
  }
}

void DebugGui::SetMode( int mode )
{
  if ( m_DebugMode == mode )
  {
    return;
  }

  m_DebugMode = mode;

  RemoveButtons();
  ShowButtons( 0 );
}

void DebugGui::SlotReloadResources( int /* btnId */ )
{
  RemoveButtons();
  CoreResourceManager::GetInstance()->ReloadResources();
}

void DebugGui::SlotReleaseTextures( int /*btnId*/ )
{
  //CoreOpenGLRender::s_SignalScreenResetStart();
  CoreTextureManager::GetInstance()->ReleaseTextures();
}

void DebugGui::SlotReloadTextures( int /*btnId*/ )
{
  //CoreTextureManager::GetInstance()->ReleaseTextures();
  CoreTextureManager::GetInstance()->ReloadTextures();
}

void DebugGui::SlotReinitScreen( int /*btnId*/ )
{
  CoreOpenGLRender::s_SignalScreenResetStart();
  CoreOpenGLRender::s_SignalScreenResetDone();
  //CoreTextureManager::GetInstance()->ReleaseTextures();
}

CoreGuiBase* DebugGui::OnMouseButtonDown( int  buttonId , float  xPos , float  yPos  )
{
  CoreGuiBase* guiObject  = NULL;

  switch ( m_DebugMode )
  {
    case DEBUGMODE_SINGLEWIREFRAME:
      {
        CoreGuiToolkit* guiToolkit = safe_cast<CoreGuiToolkit*> ( m_CoreEngine.GetEngineModule( EngineModule::COREMODULETYPE_GUITOOLKIT ) );
        if ( guiToolkit )
        {
          bool wireframe          = true;

          if ( buttonId == CoreInputManager::INPUT_TOUCH_2 )
          {
            guiObject = guiToolkit->GetWireGuiUnderMouse( xPos, yPos );
            wireframe = false;
          }
          else
          {
            guiObject = guiToolkit->GetSolidGuiUnderMouse( xPos, yPos );
            wireframe = true;
          }

          if ( guiObject != NULL )
          {
            if ( guiObject->GetTopParent() != this )
            {
              guiObject->SetWireframe( wireframe );
              return this;
            }
          }
        }

        // If we are clicking on the debug buttons, we have to treat those like normal buttons
        guiObject = CoreGuiBase::OnMouseButtonDown( buttonId, xPos, yPos );
        if ( guiObject != NULL && guiObject->GetTopParent() == this )
        {
          return guiObject;
        }
      }
      return this;

    case DEBUGMODE_MOVEGUI:
      {
        if ( buttonId == CoreInputManager::INPUT_TOUCH_2 && m_RevertObject != NULL )
        {
          m_RevertObject->SetPosition( m_StartPos );
          m_RevertObject = NULL;
          return this;
        }

        CoreGuiToolkit* guiToolkit = (CoreGuiToolkit*) m_CoreEngine.GetEngineModule( EngineModule::COREMODULETYPE_GUITOOLKIT );
        if ( guiToolkit )
        {
          guiObject = guiToolkit->GetSolidGuiUnderMouse( xPos, yPos );
          if ( guiObject != NULL && !guiObject->IsDebugGui() )
          {
            m_ActiveObject = guiObject;
            m_RevertObject = m_ActiveObject;
            m_StartPos     = m_ActiveObject->GetPosition();
            m_ActiveObject->AddChild( new CoreGuiFrame( "MouseMoveFrame", m_ActiveObject->GetPosition(), m_ActiveObject->GetSize(), Color::Orange ) );
            return this;
          }
        }

        // If we are clicking on the debug buttons, we have to treat those like normal buttons
        guiObject = CoreGuiBase::OnMouseButtonDown( buttonId, xPos, yPos );
        if ( guiObject != NULL && guiObject->GetTopParent() == this )
        {
          return guiObject;
        }
        return this;
      }
      break;

    case DEBUGMODE_EDITGUI:
      {
        if ( buttonId == CoreInputManager::INPUT_TOUCH_2 && m_RevertObject != NULL )
        {
          m_RevertObject->SetPosition( m_StartPos );
          m_RevertObject = NULL;
          return this;
        }

        CoreGuiToolkit* guiToolkit = (CoreGuiToolkit*) m_CoreEngine.GetEngineModule( EngineModule::COREMODULETYPE_GUITOOLKIT );
        if ( guiToolkit )
        {
          guiObject = guiToolkit->GetSolidGuiUnderMouse( xPos, yPos );
          if ( guiObject != NULL && !guiObject->IsDebugGui() )
          {
            SignalActiveGui( guiObject );
            return this;
          }
        }

        // If we are clicking on the debug buttons, we have to treat those like normal buttons
        guiObject = CoreGuiBase::OnMouseButtonDown( buttonId, xPos, yPos );
        if ( guiObject != NULL && guiObject->GetTopParent() == this )
        {
          return guiObject;
        }
        return this;
      }
      break;

    case DEBUGMODE_HIDDEN:
      return NULL;

    case DEBUGMODE_NONE:
    default:
      {
        guiObject = CoreGuiBase::OnMouseButtonDown( buttonId, xPos, yPos );
        if ( guiObject != NULL )
        {
          return guiObject;
        }
        if (m_Menu != NULL)
        {
          return this;
        }
      }
      break;
  }

  return NULL;
}

CoreGuiBase* DebugGui::OnMouseButtonUp( int  buttonId , float  xPos , float  yPos  )
{
  switch( m_DebugMode )
  {
    case DEBUGMODE_MOVEGUI:
      {
        if ( m_ActiveObject != NULL )
        {
          m_ActiveObject->RemoveChild( m_ActiveObject->GetChildWithName("MouseMoveFrame") );
          m_ActiveObject = NULL;
        }
      }
      return this;

    default:
      {
        // If we are clicking on the debug buttons, we have to treat those like normal buttons
        CoreGuiBase* guiObject = CoreGuiBase::OnMouseButtonUp( buttonId, xPos, yPos );
        if ( guiObject != NULL && guiObject->GetTopParent() == this )
        {
          return guiObject;
        }
      }
      break;
  }

  return NULL;
}


bool DebugGui::OnMouseDrag( float  xPos , float  yPos  )
{
  switch( m_DebugMode )
  {
    case DEBUGMODE_MOVEGUI:
      {
        if ( m_ActiveObject != NULL )
        {
          m_ActiveObject->Move( xPos, yPos, false );
          return true;
        }
      }
      break;

    default:
      break;
  }
  return false;
}

void DebugGui::RefreshButtons()
{
  RemoveButtons();
  ShowButtons( 0 );
}
