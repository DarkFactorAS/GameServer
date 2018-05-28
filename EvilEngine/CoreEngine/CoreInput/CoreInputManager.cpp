/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : CoreInputManager
*
* Description : Handles all input messages, processes them and forwards them to the game thread
*               Currently this supports keyboard, touch and mouse.
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "CoreInputManager.h"
#include "EvilEngine/CoreLib/BasicTypes/Mutex/ScopedMutex.h"
#include "EvilEngine/CoreIO/Logger/Logger.h"
#include "Touch.h"
#include "Keyboard.h"

#include <stdlib.h>

CoreInputManager::CoreInputManager() :
  m_SupportMultiTouch( true ),
  m_GestureMode( GESTURE_NONE )
{
  m_MouseButtons = new TouchState[ NUM_BUTTONS ];
  for ( int i = 0; i < NUM_BUTTONS; i++ )
  {
    m_MouseButtons[ i ].m_IsDown  = false;
  }
}

/************************************************************************************************
* GetInstance:
* Return the static singleton pointer to this class
*
* @return (CoreInputManager*)               - Returns the singleton instance
*
* @author Thor Richard Hansen
*************************************************************************************************/
CoreInputManager* CoreInputManager::GetInstance()
{
  static CoreInputManager* s_Instance = NULL;
  if ( s_Instance == NULL )
  {
    s_Instance = new CoreInputManager();
  }
  return s_Instance;
}

void CoreInputManager::HandleMouseMove( int pointerIndex, float mouseX, float mouseY )
{
  // Only support two finger touch
  if ( pointerIndex == Touch::LEFT_MOUSE_BUTTON /*|| pointerIndex == Touch::RIGHT_MOUSE_BUTTON*/ )
  {
    InputEvent event;
    event.m_InputType     = ( pointerIndex == Touch::LEFT_MOUSE_BUTTON ) ? INPUT_MOUSEMOVE : INPUT_MOUSEMOVE2;
    event.m_Position.Set( mouseX, mouseY );
    {
      ScopedMutex eventMutex( &m_EventMutex );
      m_EventList.push_back( event );
    }
  }
  // Only support two finger touch
  else if ( pointerIndex == Touch::RIGHT_MOUSE_BUTTON )
  {
    InputEvent event;
    event.m_InputType     = ( pointerIndex == Touch::LEFT_MOUSE_BUTTON ) ? INPUT_MOUSEMOVE : INPUT_MOUSEMOVE2;
    event.m_Position.Set( mouseX, mouseY );
    {
      ScopedMutex eventMutex( &m_EventMutex );
      m_EventList.push_back( event );
    }
  }
}

void CoreInputManager::HandleMouseButtonDown( int buttonId )
{
  switch( buttonId )
  {
    // Treat LMB as normal touch
    case Touch::LEFT_MOUSE_BUTTON:
      {
        InputEvent event;
        event.m_InputType     = INPUT_MOUSEDOWN;
        event.m_EventData.SetUInt32( buttonId );
        //event.m_Position.Set( m_TouchPosition[ Touch::LEFT_MOUSE_BUTTON ].x, m_TouchPosition[ Touch::LEFT_MOUSE_BUTTON ].y );
        ScopedMutex eventMutex( &m_EventMutex );
        m_EventList.push_back( event );
      }
      break;

      // Treat LMB as normal touch
    case Touch::RIGHT_MOUSE_BUTTON:
      {
        InputEvent event;
        event.m_InputType     = INPUT_MOUSE2DOWN;
        event.m_EventData.SetUInt32( buttonId );
        //int moveButtonId = ( m_SupportMultiTouch ? Touch::RIGHT_MOUSE_BUTTON : Touch::LEFT_MOUSE_BUTTON );
        //event.m_Position.Set( m_TouchPosition[ moveButtonId ].x, m_TouchPosition[ moveButtonId ].y );
        //event.m_Position.Set( m_TouchPosition[ Touch::LEFT_MOUSE_BUTTON ].x, m_TouchPosition[ Touch::LEFT_MOUSE_BUTTON ].y );
        ScopedMutex eventMutex( &m_EventMutex );
        m_EventList.push_back( event );
      }
      break;

    // Ignore other buttons for now
    default:
      break;
  }
}

void CoreInputManager::HandleMouseButtonUp( int buttonId )
{
  switch( buttonId )
  {
    // Treat LMB as normal touch
    case Touch::LEFT_MOUSE_BUTTON:
      {
        InputEvent event;
        event.m_InputType     = INPUT_MOUSEUP;
        event.m_EventData.SetUInt32( buttonId );
        //event.m_Position.Set( m_TouchPosition[ Touch::LEFT_MOUSE_BUTTON ].x, m_TouchPosition[ Touch::LEFT_MOUSE_BUTTON ].y );
        {
          ScopedMutex eventMutex( &m_EventMutex );
          m_EventList.push_back( event );
        }
      }
      break;

    // Treat LMB as normal touch
    case Touch::RIGHT_MOUSE_BUTTON:
      {
        //int moveButtonId = ( m_SupportMultiTouch ? buttonId : Touch::LEFT_MOUSE_BUTTON );

        InputEvent event;
        event.m_InputType     = INPUT_MOUSE2UP;
        event.m_EventData.SetUInt32( buttonId );
        //event.m_Position.Set( m_TouchPosition[ moveButtonId ].x, m_TouchPosition[ moveButtonId ].y );
        {
          ScopedMutex eventMutex( &m_EventMutex );
          m_EventList.push_back( event );
        }
      }
      break;

    // Ignore other buttons
    default:
      break;
  }
}

void CoreInputManager::HandleMouseWheel( float scrollDelta )
{
  InputEvent event;
  event.m_InputType     = INPUT_MOUSEWHEEL;
  event.m_EventData.SetFloat( scrollDelta );
  //event.m_Position.Set( m_TouchPosition[ Touch::LEFT_MOUSE_BUTTON ].x, m_TouchPosition[ Touch::LEFT_MOUSE_BUTTON ].y );
  {
    ScopedMutex eventMutex( &m_EventMutex );
    m_EventList.push_back( event );
  }
}

void CoreInputManager::ProcessEvents()
{
  {
    ScopedMutex eventMutex(&m_EventMutex );
    m_RunningEventList.swap( m_EventList );
  }

  for ( size_t index = 0; index < m_RunningEventList.size(); index++ )
  {
    const InputEvent& event = m_RunningEventList[ index ];
    switch( event.m_InputType )
    {
      case INPUT_MOUSEDOWN:
        m_MouseButtons[ INPUT_MOUSEBUTTON_1 ].m_IsDown  = true;
        m_MouseButtons[ INPUT_MOUSEBUTTON_1 ].m_StartPosition = m_MouseButtons[ INPUT_MOUSEBUTTON_1 ].m_CurrentPosition;
        SignalTouchEvent( INPUT_TOUCH_DOWN, m_MouseButtons[ INPUT_MOUSEBUTTON_1 ].m_CurrentPosition );
        //LogInfoFMT("CoreInputManager", "Mouse1Down : pos[%0.2f x %0.2f]", m_MouseButtons[ INPUT_MOUSEBUTTON_1 ].m_CurrentPosition.x, m_MouseButtons[ INPUT_MOUSEBUTTON_1 ].m_CurrentPosition.y );
        break;
        
      case INPUT_MOUSE2DOWN:
        m_MouseButtons[ INPUT_MOUSEBUTTON_2 ].m_IsDown  = true;
        m_MouseButtons[ INPUT_MOUSEBUTTON_2 ].m_StartPosition = m_MouseButtons[ INPUT_MOUSEBUTTON_2 ].m_CurrentPosition;
        //LogInfoFMT("CoreInputManager", "Mouse2Down : pos[%0.2f x %0.2f]", m_MouseButtons[ INPUT_MOUSEBUTTON_2 ].m_CurrentPosition.x, m_MouseButtons[ INPUT_MOUSEBUTTON_2 ].m_CurrentPosition.y );
        SignalTouchEvent( INPUT_TOUCH2_DOWN, m_MouseButtons[ INPUT_MOUSEBUTTON_2 ].m_CurrentPosition );
        break;

      case INPUT_MOUSEUP:
        m_GestureMode = GESTURE_NONE;
        m_MouseButtons[ INPUT_MOUSEBUTTON_1 ].m_IsDown  = false;
        SignalTouchEvent( INPUT_TOUCH_UP, m_MouseButtons[ INPUT_MOUSEBUTTON_1 ].m_CurrentPosition );
        break;

      case INPUT_MOUSE2UP:
        m_GestureMode = GESTURE_NONE;
        m_MouseButtons[ INPUT_MOUSEBUTTON_2 ].m_IsDown  = false;
        SignalTouchEvent( INPUT_TOUCH2_UP, m_MouseButtons[ INPUT_MOUSEBUTTON_2 ].m_CurrentPosition );
        break;

      case INPUT_MOUSEMOVE:
        {
          bool useGesture = CheckForGesture( INPUT_MOUSEBUTTON_1, m_MouseButtons[ INPUT_MOUSEBUTTON_1 ].m_CurrentPosition, event.m_Position );
          m_MouseButtons[ INPUT_MOUSEBUTTON_1 ].m_CurrentPosition = event.m_Position;
          if ( !useGesture )
          {
            SignalTouchEvent( INPUT_MOUSEMOVE, event.m_Position );
          }
        }
        break;

      case INPUT_MOUSEMOVE2:
        {
          bool useGesture = CheckForGesture( INPUT_MOUSEBUTTON_2, m_MouseButtons[ INPUT_MOUSEBUTTON_2 ].m_CurrentPosition, event.m_Position );
          m_MouseButtons[ INPUT_MOUSEBUTTON_2 ].m_CurrentPosition = event.m_Position;
          if ( !useGesture && !m_SupportMultiTouch )
          {
            SignalTouchEvent( INPUT_MOUSEMOVE, event.m_Position );
          }
        }
        break;

      case INPUT_MOUSEWHEEL:
        SignalMouseWheel( event.m_EventData.GetFloat(), m_MouseButtons[ INPUT_MOUSEBUTTON_1 ].m_CurrentPosition );
        break;

      default:
        break;
    }
  }
  m_RunningEventList.clear();
}

bool CoreInputManager::CheckForGesture( int moveEnum, const Vector2& fromPosition, const Vector2& toPosition )
{
  // If Im in normal mode, check if I should switch
  if ( m_GestureMode == GESTURE_NONE )
  {
    if ( !m_MouseButtons[ INPUT_MOUSEBUTTON_1 ].m_IsDown || !m_MouseButtons[ INPUT_MOUSEBUTTON_2 ].m_IsDown )
    {
      return false;
    }

    //float deltaX = fabs( toPosition.x - m_MouseButtons[ moveEnum ].m_StartPosition.x );
    //float deltaY = fabs( toPosition.y - m_MouseButtons[ moveEnum ].m_StartPosition.y );
    //if ( deltaX < 0.05f && deltaY < 0.05f )
    //{
    //  return false;
    //}
  }

  float deltaX = fabs( toPosition.x - fromPosition.x );
  float deltaY = fabs( toPosition.y - fromPosition.y );

  bool moveHorizontal = ( deltaX > deltaY );
  bool moveVertical   = ( deltaX < deltaY );

  // Zoom
  if ( moveHorizontal )
  {
    float distX1 = m_MouseButtons[ INPUT_MOUSEBUTTON_2 ].m_CurrentPosition.x - m_MouseButtons[ INPUT_MOUSEBUTTON_1 ].m_CurrentPosition.x;
    float distX2 = m_MouseButtons[ ( moveEnum == INPUT_MOUSEBUTTON_1 ? INPUT_MOUSEBUTTON_2 : INPUT_MOUSEBUTTON_1 ) ].m_CurrentPosition.x - toPosition.x;
    float zoom   = ( fabs( distX1 ) - fabs( distX2 ) );
    SignalMouseWheel( zoom, toPosition );
  }
  // Rotate
  else if ( moveVertical )
  {
    if ( IsLeftPoint( INPUT_MOUSEBUTTON_2 - moveEnum, toPosition.x ) )
    {
      SignalGestureRotate( 0.0f, toPosition.y - fromPosition.y );
    }
    else
    {
      SignalGestureRotate( 0.0f, fromPosition.y - toPosition.y );
    }
  }

  return true;
}

bool CoreInputManager::IsMouseButtonDown( int buttonId )
{
  if ( buttonId >=0 && buttonId < NUM_BUTTONS )
  {
    return m_MouseButtons[ buttonId ].m_IsDown;
  }
  return false;
}

bool CoreInputManager::IsLeftPoint( int pointEnum, float x )
{
  return ( x < m_MouseButtons[ pointEnum ].m_CurrentPosition.x );
}

int CoreInputManager::GetHotkey( const String& namedHotkey )
{
  String lowerKey = namedHotkey.GetLower();
  if ( lowerKey.Equals( "escape" ) )
  {
    return Keyboard::KEY_ESCAPE;
  }
  if ( lowerKey.Equals( "enter" ) )
  {
    return Keyboard::KEY_ENTER;
  }
  return atoi( lowerKey.c_str() );
}