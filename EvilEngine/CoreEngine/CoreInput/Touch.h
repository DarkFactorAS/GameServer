#ifndef EVILENGINE_COREENGINE_COREINPUT_TOUCH
#define EVILENGINE_COREENGINE_COREINPUT_TOUCH 1

class Vector2;

class Touch
{
public:

  enum MOUSE_BUTTONS
  {
    FIRST_MOUSE_BUTTON  = 1,
    LEFT_MOUSE_BUTTON   = 1,
    RIGHT_MOUSE_BUTTON  = 2,
  };

  enum TOUCH_EVENT
  {
    TOUCHEVENT_NONE,
    TOUCHEVENT_STARTTOUCH1,   // Touched with 1 finger, or Left MB
    TOUCHEVENT_ENDTOUCH1,     // Released first tocuh or left MB
  };

  struct TouchPoint
  {
  public:
    TouchPoint( float x, float y )
    {
      m_X = x;
      m_Y = y;
    }
    float m_X;
    float m_Y;
    float m_Time;
  };

  struct TouchEvent
  {
    int   m_TouchEvent;
    float m_X;
    float m_Y;
  };

  Touch();

  static Touch* GetInstance();

  TouchPoint OnMouseButtonDown( int buttonId );

  const Vector2& GetPosition( int index );

  //TOUCH_EVENT GetTouchEvent( int a, float x, float y );

};

#endif /// EVILENGINE_COREENGINE_COREINPUT_TOUCH
