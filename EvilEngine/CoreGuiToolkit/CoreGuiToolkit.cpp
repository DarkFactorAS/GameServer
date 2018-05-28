/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014-2015
* File        : CoreGuiToolkit
* Description : Handles all the openGL gui elements. This class forwards the rendering of the 
*               gui objects as well as handling the mouse/keyboard for them.
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "CoreGuiToolkit.h"
#include "BaseGuiElements/CoreGuiBase.h"
#include "EvilEngine/CoreLib/BasicTypes/Mutex/ScopedMutex.h"
#include "EvilEngine/CoreLib/Assert/DFAssert.h"
#include "EvilEngine/CoreGuiToolkit/CoreGuiToolkitInclude.h"
#include "EvilEngine/CoreIO/Logger/Logger.h"

#include <algorithm>

float CoreGuiToolkit::s_Left     = -1.0f;
float CoreGuiToolkit::s_Right    = 1.0f;
float CoreGuiToolkit::s_Top      = -1.0f;
float CoreGuiToolkit::s_Bottom   = 1.0f;

float CoreGuiToolkit::s_Width    = 2.0f;
float CoreGuiToolkit::s_Height   = 2.0f;

bool  CoreGuiToolkit::s_MoveGui  = false;

CoreGuiToolkit* CoreGuiToolkit::s_Instance    = NULL;
CoreBaseObject* CoreGuiToolkit::s_DragObject  = NULL;

std::map< const String, CoreGuiToolkit::InstantiateGui_fp* > CoreGuiToolkit::s_GuiSpawnList;

CoreGuiToolkit* CoreGuiToolkit::CreateInstance()
{
  dfAssert(s_Instance == NULL, "Tried to create a gui toolkit twice!");
  s_Instance = new CoreGuiToolkit();
  return GetInstance();
}

CoreGuiToolkit::CoreGuiToolkit() :
  EngineModule( EngineModule::COREMODULETYPE_GUITOOLKIT ),
  RenderModule( RenderModule_GuiToolkit ),
  m_MouseHoldTimer( 0.0f )
{
  m_FocusObject = NULL;

  AddGuiSpawner( "Window",        CoreGuiWindow::Initialize );
  AddGuiSpawner( "View",          CoreGuiView::Initialize );
  AddGuiSpawner( "Button",        CoreGuiButton::Initialize );
  AddGuiSpawner( "InputField",    CoreGuiInputfield::Initialize );
  AddGuiSpawner( "TextField",     CoreGuiTextfield::Initialize );
  AddGuiSpawner( "Image",         CoreGuiImageView::Initialize );
  AddGuiSpawner( "Frame",         CoreGuiFrame::Initialize );
  AddGuiSpawner( "GridView",      CoreGuiGridView::Initialize );
  AddGuiSpawner( "TileView",      CoreGuiTileView::Initialize );

  // Button colors
  Color::AddNamedColor( StaticStr("positive"), Color::Green);
  Color::AddNamedColor( StaticStr("negative"), Color::Red);
  Color::AddNamedColor( StaticStr("neutral"), Color::Orange);

  // Window color
  Color::AddNamedColor( StaticStr("window_bordercolor"), Color::LightGrey);
  Color::AddNamedColor( StaticStr("window_titlebgcolor"), Color::DarkGrey);
  Color::AddNamedColor( StaticStr("window_bgcolor"), Color::Grey);
  Color::AddNamedColor( StaticStr("window_titletextcolor"), Color::White);

  Color::AddNamedColor(StaticStr("window_bgcolor"), Color::Grey);

  // Inputfield color
  Color::AddNamedColor( StaticStr("input_backgroundcolor"), Color::White);
  Color::AddNamedColor( StaticStr("input_textcolor"), Color::Black);
  Color::AddNamedColor( StaticStr("input_bordercolor"), Color::Black);
  Color::AddNamedColor( StaticStr("input_activebordercolor"), Color::Red);
  Color::AddNamedColor( StaticStr("input_activetextcolor"), Color::Red);
  Color::AddNamedColor( StaticStr("input_tooltipcolor"), Color::LightGrey);

  // Textfield color
  Color::AddNamedColor(StaticStr("textcolor"), Color::White);
}

CoreGuiToolkit::~CoreGuiToolkit()
{
  // Refactor this
  FrameProcess( 0.0f );
}

CoreGuiToolkit* CoreGuiToolkit::GetInstance()
{
  dfAssert(s_Instance != NULL, "Tried to reference a NULL pointer gui toolkit!");
  return s_Instance;
}

void CoreGuiToolkit::Destroy()
{
  delete s_Instance;
  s_Instance = NULL;
}

void CoreGuiToolkit::AddGuiSpawner( const char* name, InstantiateGui_fp* fp )
{
  s_GuiSpawnList[ name ] = fp;
}

CoreGuiBase* CoreGuiToolkit::CreateGuiObject(GuiObjectType objectType)
{
  switch( objectType )
  {
    case GuiObject_Dialog:
      return new CoreGuiDialog();
      break;
    default:
      return new CoreGuiBase();
  }
}

CoreGuiBase* CoreGuiToolkit::InstantiateGui( const String& name )
{
  dfAssert( !s_GuiSpawnList.empty(), "Gui spawner is not initialized!" ); 

  std::map< const String, InstantiateGui_fp* >::iterator itGui = s_GuiSpawnList.find( name );
  if ( itGui != s_GuiSpawnList.end() )
  {
    return ( ( *itGui ).second() );
  }
  else
  {
    LogWarningFMT("CoreGuiToolkit", "Cannot spawn gui : Missing gui element '%s'", name.c_str() );
    return NULL;
  }
}

void CoreGuiToolkit::OnGuiObjectDeath( const CoreBaseObject* guiObject )
{
  dfAssert( guiObject != NULL, "A gui object was deleted without beeing removed first!" );
}

void CoreGuiToolkit::OnFocusObjectDeath( const CoreBaseObject* guiObject )
{
  if ( m_FocusObject == guiObject )
  {
    m_FocusObject = NULL;
  }
}

/************************************************************************************************
* OnDraw:
* Calls the draw method on all gui elements to be drawn on the openGL screen. The gui toolkit
* will use its own projection to draw a seperate 2D layer of objects
*
* @param  (const Matrix&)  projection   - The screen projection for the gui elements
*
* @author Thor Richard Hansen
*************************************************************************************************/
#ifdef CLIENT
void CoreGuiToolkit::OnDraw( const Matrix& /*projection*/ )
{
  for ( size_t index = 0; index < m_GuiObjects.size(); index++ )
  {
    CoreGuiBase* guiObject = m_GuiObjects[ index ];
    guiObject->OnDraw( m_Projection );
  }
}
#endif

/************************************************************************************************
* ResizeScreen:
* Called when the screen is resized. This is used to reorganize the gui elements
*
* @param  (int)  width   - new Width of the render surface
* @param  (int)  height  - new height of the render surface
*
* @author Thor Richard Hansen
*************************************************************************************************/
#ifdef CLIENT
void CoreGuiToolkit::ResizeScreen( int /*width*/, int /*height*/ )
{
  m_Projection.Ortographic( s_Left, s_Right, s_Top, s_Bottom, 0.1f, 100.0f );
}
#endif

/************************************************************************************************
* AddGuiObjects:
* Adds a gui object to the list
*
* @param  (CoreGuiBase*)  guiObject - The gui object to add to the gui toolkit
*
* @author Thor Richard Hansen
*************************************************************************************************/
void CoreGuiToolkit::AddGuiObject( CoreGuiBase* guiObject )
{
  if ( guiObject != NULL )
  {
    ScopedMutex mutex( &m_GuiObjectMutex );
    m_NewGuiObjects.push_back( guiObject );

    dfAssert( guiObject->GetParent() == NULL, "Cannot add non top level gui objects to the toolkit!");
    dfAssert( !guiObject->IsAddedToToolkit(), "This gui object is already added to the gui toolkit!");
    guiObject->SetInToolkit( true );

#if defined DEBUG && defined PLATFORM_WINDOWS
    // Check for direct delete
    guiObject->m_SignalOnDeath.Connect( this, &CoreGuiToolkit::OnGuiObjectDeath );
#endif
  }
}

CoreGuiBase* CoreGuiToolkit::GetChildWithName( const String& guiName )
{
  ScopedMutex mutex( &m_GuiObjectMutex );

  for ( std::vector<CoreGuiBase*>::iterator itObj = m_NewGuiObjects.begin(); itObj != m_NewGuiObjects.end(); itObj++ )
  {
    CoreGuiBase* guiObject = *itObj;
    if ( guiObject->HasDestroy() )
    {
      continue;
    }
    if ( guiObject->GetName().CompareNoCase( guiName ) == 0 )
    {
      return guiObject;
    }
    CoreGuiBase* child = guiObject->GetChildWithName( guiName );
    if ( child != NULL )
    {
      return child;
    }
  }

  for ( std::vector<CoreGuiBase*>::iterator itObj = m_GuiObjects.begin(); itObj != m_GuiObjects.end(); itObj++ )
  {
    CoreGuiBase* guiObject = *itObj;
    if ( guiObject->HasDestroy() )
    {
      continue;
    }
    if ( guiObject->GetName().CompareNoCase( guiName ) == 0 )
    {
      return guiObject;
    }
    CoreGuiBase* child = guiObject->GetChildWithName( guiName );
    if ( child != NULL )
    {
      return child;
    }
  }
  return NULL;
}

/************************************************************************************************
* RemoveGuiObject:
* Remove a gui object from the gui toolkit. The gui toolkit will handle behavior and rendering
*
* @param  (CoreGuiBase*)   guiObject - The gui object to remove
*
* @author Thor Richard Hansen
*************************************************************************************************/
void CoreGuiToolkit::RemoveGuiObject( CoreGuiBase* removeGuiObject )
{
  if ( removeGuiObject == NULL )
  {
    return;
  }

  // Check if the object is in the new list
  {
    ScopedMutex mutex( &m_GuiObjectMutex );
    for ( std::vector< CoreGuiBase* >::iterator itObj = m_NewGuiObjects.begin(); itObj != m_NewGuiObjects.end(); ++itObj )
    {
      CoreGuiBase* guiObject = *itObj;
      if ( guiObject == removeGuiObject )
      {
#ifdef DEBUG
        guiObject->m_SignalOnDeath.Disconnect( this, &CoreGuiToolkit::OnGuiObjectDeath );
#endif
        m_NewGuiObjects.erase( itObj );

        if ( guiObject == m_FocusObject )
        {
          m_FocusObject = NULL;
        }

        delete guiObject;
        return;
      }
    }
  }

  for ( std::vector< CoreGuiBase* >::iterator itObj = m_GuiObjects.begin(); itObj != m_GuiObjects.end(); ++itObj )
  {
    if ( removeGuiObject == *itObj )
    {
      CoreGuiBase* guiObject = *itObj;
      guiObject->Destroy();
      return;
    }
  }
}

/************************************************************************************************
* RemoveAllGuiObjects:
* Removes all gui objects from the list
*
* @author Thor Richard Hansen
*************************************************************************************************/
void CoreGuiToolkit::RemoveAllGuiObjects()
{
  for ( std::vector< CoreGuiBase* >::iterator itObj = m_GuiObjects.begin(); itObj != m_GuiObjects.end(); ++itObj )
  {
    CoreGuiBase* guiObject = *itObj;
    guiObject->Destroy();
  }
}

/************************************************************************************************
 * FrameProcess:
 * Overloaded function that runs every frame. This will check if the gui object has set itself
 * to terminate. If it has, just remove it from the gui list.
 *
 * @param  (float)   deltaTime         - Amount of time that has passed since last call
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
void CoreGuiToolkit::FrameProcess( float deltaTime )
{
  {
    ScopedMutex mutex( &m_GuiObjectMutex );
    if ( !m_NewGuiObjects.empty() )
    {
      m_GuiObjects.insert( m_GuiObjects.end(), m_NewGuiObjects.begin(), m_NewGuiObjects.end() );
      m_NewGuiObjects.clear();
      CoreGuiBaseSorter sorter;
      std::sort( m_GuiObjects.begin(), m_GuiObjects.end(), sorter );
    }
  }

  for ( std::vector< CoreGuiBase* >::iterator itObj = m_GuiObjects.begin(); itObj != m_GuiObjects.end(); )
  {
    CoreGuiBase* guiObject = *itObj;
    if ( guiObject->RunDelete() )
    {
      itObj = m_GuiObjects.erase( itObj );
      if ( guiObject == m_FocusObject )
      {
        m_FocusObject = NULL;
      }
      delete guiObject;
    }
    else
    {
      guiObject->FrameProcess( deltaTime );
      itObj++;
    }
  }

  if ( m_MouseHoldTimer > 0.0f )
  {
    m_MouseHoldTimer -= deltaTime;
    if ( m_MouseHoldTimer <= 0.0f )
    {
      SignalMouseLongTouch( m_MouseHoldPos );
    }
  }
}

CoreGuiBase* CoreGuiToolkit::GetWireGuiUnderMouse( float xPos, float yPos )
{
  for ( size_t index = 0; index < m_GuiObjects.size(); index++ )
  {
    CoreGuiBase* guiObject      = m_GuiObjects[ index ];
    if ( !guiObject->HasDestroy() )
    {
      CoreGuiBase* foundGuiObject = guiObject->GetWireGuiUnderMouse( xPos, yPos );
      if ( foundGuiObject != NULL )
      {
        return foundGuiObject;
      }
    }
  }
  return NULL;
}

CoreGuiBase* CoreGuiToolkit::GetSolidGuiUnderMouse( float xPos, float yPos )
{
  for ( size_t index = m_GuiObjects.size(); index > 0; index-- )
  {
    CoreGuiBase* guiObject      = m_GuiObjects[ index - 1 ];
    if ( !guiObject->HasDestroy() )
    {
      CoreGuiBase* foundGuiObject = guiObject->GetSolidGuiUnderMouse( xPos, yPos );
      if ( foundGuiObject != NULL )
      {
        return foundGuiObject;
      }
    }
  }
  return NULL;
}

/************************************************************************************************
* OnMouseButtonDown:
* Forwards the mousebutton / touch to the first gui object it intersects with
*
* @param  (int)   xPos - The x position in the windwo, where the mouse button was pressed
* @param  (int)   yPos - The y position in the windwo, where the mouse button was pressed
* @return (bool)       - Return true if any of the gui object captured the mouse
*
* @author Thor Richard Hansen
*************************************************************************************************/
#ifdef CLIENT
bool CoreGuiToolkit::OnMouseButtonDown( int buttonId, float xPos, float yPos )
{
  //if ( m_FocusObject != NULL && m_FocusObject->OnMouseButtonDown( buttonId, xPos, yPos ) == m_FocusObject )
  //{
  //  return true;
  //}
  //LooseFocus();

  for ( size_t index = m_GuiObjects.size(); index > 0; index-- )
  {
    CoreGuiBase* guiObject      = m_GuiObjects[ index - 1 ];
    if ( !guiObject->HasDestroy() )
    {
      CoreGuiBase* guiFocusObject = guiObject->OnMouseButtonDown( buttonId, xPos, yPos );
      if ( guiFocusObject != NULL )
      {
        if ( guiFocusObject != m_FocusObject )
        {
          LooseFocus();
        }
        GainFocus( guiFocusObject );
        return true;
      }
      else if ( guiObject->IsModal() )
      {
        return true;
      }
    }
  }
  LooseFocus();
  m_MouseHoldTimer = 2.0f;
  m_MouseHoldPos.x = xPos;
  m_MouseHoldPos.y = yPos;
  return false;
}
#endif

#ifdef CLIENT
bool CoreGuiToolkit::OnMouseButtonUp( int buttonId, float xPos, float yPos )
{
  if ( m_FocusObject != NULL )
  {
    m_FocusObject->OnMouseButtonUp( buttonId, xPos, yPos );
  }

  // Send mouse button up to all guis
  for ( size_t index = m_GuiObjects.size(); index > 0; index-- )
  {
    CoreGuiBase* guiObject      = m_GuiObjects[ index - 1 ];
    if ( !guiObject->HasDestroy() && guiObject != m_FocusObject )
    {
      guiObject->OnMouseButtonUp( buttonId, xPos, yPos );
    }
  }
  m_MouseHoldTimer = 0.0f;
  return true;
}
#endif

#ifdef CLIENT
void CoreGuiToolkit::OnMouseMove( float xPos, float yPos )
{
  if ( m_CoreEngine == NULL || !m_CoreEngine->IsMouseDown( 1 ) )
  {
    return;
  }

  if ( m_FocusObject != NULL )
  {
    m_FocusObject->OnMouseDrag( xPos, yPos );
  }

  //for ( size_t index = 0; index < m_GuiObjects.size(); index++ )
  //{
  //  CoreGuiBase* guiObject = m_GuiObjects[ index ];
  //  if ( !guiObject->HasDestroy()  )
  //  {
  //    guiObject->OnMouseDrag( xPos, yPos );
  //  }
  //}
}
#endif

#ifdef CLIENT
bool CoreGuiToolkit::OnMouseButtonWheel( float delta, const Vector2& mousePos )
{
  for ( size_t index = m_GuiObjects.size(); index > 0; index-- )
  {
    CoreGuiBase* guiObject      = m_GuiObjects[ index - 1 ];
    if ( !guiObject->HasDestroy() )
    {
      if ( guiObject->OnMouseButtonWheel( delta, mousePos ) )
      {
        return true;
      }
    }
  }
  return false;
}
#endif

#ifdef CLIENT
bool CoreGuiToolkit::OnKeyDown( int keyCode )
{
  if ( m_FocusObject != NULL )
  {
    if ( keyCode == 0x09 )
    {
      return SetNextFocus();
    }
    else
    {
      CoreGuiBase* focus = m_FocusObject;
      return focus->OnKeyDown( keyCode );
    }
  }
  for ( size_t index = m_GuiObjects.size(); index > 0; index-- )
  {
    CoreGuiBase* guiObject      = m_GuiObjects[ index - 1 ];
    if ( !guiObject->HasDestroy() )
    {
      if ( guiObject->OnKeyDown( keyCode ) )
      {
        return true;
      }
    }
  }
  return false;
}
#endif

#ifdef CLIENT
bool CoreGuiToolkit::OnKeyUp( int keyCode )
{
  if ( m_FocusObject != NULL )
  {
    return m_FocusObject->OnKeyUp( keyCode );
  }
  return false;
}
#endif

bool CoreGuiToolkit::SetNextFocus()
{
  // The gui object itself will have to check if it can change focus
  if ( m_FocusObject != NULL )
  {
    CoreGuiBase* guiFocusObject = m_FocusObject->SetNextFocus();
    if ( guiFocusObject )
    {
      GainFocus( guiFocusObject );
      return true;
    }
    LooseFocus();
  }
  return false;
}

void CoreGuiToolkit::GainFocus( CoreGuiBase* focusObject )
{
  if ( m_FocusObject != focusObject && m_FocusObject != NULL )
  {
    LooseFocus();
  }

  if ( focusObject != NULL && m_CoreEngine != NULL )
  {
    if ( focusObject->GainFocus() )
    {
      m_FocusObject = focusObject;
      m_FocusObject->m_SignalOnDeath.Connect( this, &CoreGuiToolkit::OnFocusObjectDeath );
      m_FocusObject->m_LooseFocus.Connect( this, &CoreGuiToolkit::SlotLooseFocus );

      if ( m_FocusObject->RequiresKeyboard() )
      {
        m_CoreEngine->AddMessage( CoreEngine::MESSAGE_OPEN_KEYBOARD );
      }
    }
  }
}

void CoreGuiToolkit::LooseFocus()
{
  if ( m_FocusObject != NULL && m_CoreEngine != NULL )
  {
    LogInfoFMT( "CoreGuiToolkit", "Loosing focus '%s'", m_FocusObject->GetName().c_str() );
    if ( m_FocusObject->RequiresKeyboard() )
    {
      m_CoreEngine->AddMessage( CoreEngine::MESSAGE_CLOSE_KEYBOARD );
    }

    m_FocusObject->m_SignalOnDeath.Disconnect( this, &CoreGuiToolkit::OnFocusObjectDeath );
    m_FocusObject->m_LooseFocus.Disconnect( this, &CoreGuiToolkit::SlotLooseFocus );
    m_FocusObject->LooseFocus();
    m_FocusObject = NULL;
  }
}

void CoreGuiToolkit::SlotLooseFocus( CoreGuiBase* focusObject )
{
  if ( focusObject == m_FocusObject )
  {
    LooseFocus();
  }
}

CoreGuiDialog* CoreGuiToolkit::CreateDefaultDialog( const String& title, const String& message, int buttonGroupId )
{
  CoreGuiDialog* window = new CoreGuiDialog(title, message, Vector2::s_Vec2Zero, Vector2( 0.6f, 0.4f ), buttonGroupId );
  return window;
}

CoreGuiDialog* CoreGuiToolkit::GenericErrorDialog( const String& title, const String& message )
{
  CoreGuiDialog* window = new CoreGuiDialog(title, message, Vector2::s_Vec2Zero, Vector2( 0.6f, 0.4f ), CoreGuiButton::BUTTONGROUP_CLOSE );
  return window;
}
