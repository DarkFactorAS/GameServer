#ifndef EVILENGINE_COREENGINE_COREINPUT_COREINPUTMANAGER
#define EVILENGINE_COREENGINE_COREINPUT_COREINPUTMANAGER 1

#include "EvilEngine/CoreLib/Signal/Signal.h"
#include "EvilEngine/CoreLib/CoreMath/Vector2.h"
#include "EvilEngine/CoreLib/BasicTypes/Mutex/Mutex.h"
#include "EvilEngine/CoreLib/BasicTypes/Variant/Variant.h"

#include <map>

#define MAX_EVENT_SIZE 200
#define NUM_BUTTONS 2

class CoreInputManager
{
public:

  enum INPUT_TYPE
  {
    INPUT_NONE,
    INPUT_MOUSEDOWN,      // Android/iOS Touch uses same event when user touches the screen
    INPUT_MOUSEUP,        // Android/iOS Touch uses same event when user is not touching the screen anymore
    INPUT_MOUSEMOVE,      // move touch point #1 / Mouse
    INPUT_MOUSEMOVE2,     // move touch point #2
    INPUT_MOUSEWHEEL,     // Touch devices uses this even to notify pinch/zoom gesture
    INPUT_MOUSE2DOWN,     // RMB in Windows, 2nd finger on touch devices
    INPUT_MOUSE2UP,       // RMB in Windows, 2nd finger on touch devices

    INPUT_TOUCH_UP,       // Can only be sent from inputmanager
    INPUT_TOUCH_DOWN,     // Can only be sent from inputmanager
    INPUT_TOUCH2_DOWN,    // Can only be sent from inputmanager
    INPUT_TOUCH2_UP,      // Can only be sent from inputmanager

    INPUT_TOUCH_1     = 1,     // Touch point 1
    INPUT_TOUCH_2     = 2,     // Touch point 2

    INPUT_MOUSEBUTTON_1 = 0,     // Touch point 1
    INPUT_MOUSEBUTTON_2 = 1,     // Touch point 1
  };

  struct InputEvent
  {
    INPUT_TYPE  m_InputType;
    //int         m_MouseButton;
    Vector2     m_Position;
    Variant     m_EventData;

    InputEvent() :
      m_InputType( INPUT_NONE )
    {
      m_Position = Vector2( 0.0f, 0.0f );
    }
  };

  CoreInputManager();
  static CoreInputManager*              GetInstance();

  void                                  HandleMouseButtonDown( int buttonId );
  void                                  HandleMouseButtonUp( int buttonId );
  void                                  HandleMouseMove( int pointerIndex, float mouseX, float mouseY );
  void                                  HandleMouseWheel( float scrollDelta );

  void                                  ProcessEvents();
  bool                                  IsMouseButtonDown( int buttonId );
  void                                  SupportMultitouch( bool supportMultitouch ){ m_SupportMultiTouch = supportMultitouch; }
  bool                                  DoesSupportMultitouch() const{ return m_SupportMultiTouch; }
  int                                   GetHotkey( const String& namedHotkey );

  Signal2< void, int, const Vector2& >  SignalTouchEvent;
  Signal1< void, const Vector2& >       SignalMouseMove;
  Signal2< void, float, const Vector2& > SignalMouseWheel;
  Signal2< void, float, float >         SignalGestureRotate;

private:

  bool                                  IsLeftPoint( int pointEnum, float x );
  bool                                  CheckForGesture( int moveEnum, const Vector2& fromPosition, const Vector2& toPosition );

  Mutex                                 m_EventMutex;

  std::vector< InputEvent >             m_EventList;
  std::vector< InputEvent >             m_RunningEventList;

  struct TouchState
  {
    bool    m_IsDown;
    Vector2 m_StartPosition;
    Vector2 m_CurrentPosition;
  };

  enum GESTURE_NIDE
  {
    GESTURE_NONE,
    GESTURE_ZOOM,
    GESTURE_ROTATE,
  };

  TouchState*                           m_MouseButtons;
  bool                                  m_SupportMultiTouch;
  int                                   m_GestureMode;
};

#endif /// EVILENGINE_COREENGINE_COREINPUT_COREINPUTMANAGER
