
#include "../BaseGuiElements/CoreGuiBase.h"
#include "EvilEngine/CoreLib/Signal/SignalTarget.h"
#include "EvilEngine/CoreGuiToolkit/BaseGuiElements/CoreGuiButton.h"
#include "EvilEngine/CoreEngine/CoreEngine/CoreEngine.h"
#include "EvilEngine/CoreLib/BasicTypes/Definitions/VolatileArgs.h"

//#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"
//#include "EvilGameEngine/CoreGameLogin/Account/Account.h"

class CoreGuiBase;
class DebugGui;

class DebugGui : public CoreGuiBase, public SignalTarget
{
public:

  enum BUTTON_FLAG
  {
    FLAG_NONE         = 0,
    FLAG_DEBUG        = 1 << 0,
    //FLAG_CANGM        = 1 << 1,
    FLAG_ISGM         = 1 << 2,
    FLAG_HASCLIENTGM  = 1 << 3,
    FLAG_HASSERVERGM  = 1 << 4
  };

  enum DEBUG_MODE
  {
    DEBUGMODE_HIDDEN,
    DEBUGMODE_NONE,
    DEBUGMODE_RESOLUTION,
    DEBUGMODE_MOVEGUI,
    DEBUGMODE_EDITGUI,
    DEBUGMODE_ALLWIREFRAME,
    DEBUGMODE_SINGLEWIREFRAME,
  };

  DebugGui( CoreEngine& parent, bool autoOpen );

  DebugGui& operator  =   ( const DebugGui& other )
  {
    CoreGuiBase::operator=(other);
    m_CoreEngine    = other.m_CoreEngine;
    m_DebugMode     = other.m_DebugMode;
    m_ActiveObject  = other.m_ActiveObject;
    return *this;
  }

  virtual CoreGuiBase*      OnMouseButtonDown( int  buttonId , float  xPos , float  yPos  ) DF_OVERRIDE;
  virtual CoreGuiBase*      OnMouseButtonUp( int  buttonId , float  xPos , float  yPos  ) DF_OVERRIDE;
  virtual bool              OnMouseDrag( float  xPos , float  yPos  ) DF_OVERRIDE;
  virtual bool              GainFocus() DF_OVERRIDE { return true; };

  void                      SlotReloadResources( int btnId );
  void                      SlotReleaseTextures( int btnId );
  void                      SlotReloadTextures( int btnId );
  void                      SlotReinitScreen( int btnId );
  void                      SlotToggleSingleWireframe( int /* btnId */ );
  void                      SlotToggleAllWireframe( int /* btnId */ );
  void                      SlotToggleDrawInvisibleAlpha( int /* btnId */ );
  void                      SlotToggleMoveGui( int /* btnId */ );
  void                      CloseEditGui();
  void                      SlotEditGui( int /* btnId */ );
  void                      StartSeperateWindow(CoreGuiBase* object);

  Signal1<void,CoreGuiBase*>  SignalActiveGui;

#if defined DEBUG
  virtual String              GetStaticClassName() DF_OVERRIDE { return "DebugGui"; }
#endif

  void                      SlotCloseMenu( int btnId );
  void                      SlotOnMouseLongTouch( const Vector2& position );
  virtual void              ToggleDebug();
  void                      CenterCamera();

  static bool               s_Console;

protected:

  void                      RefreshButtons();
  void                      SlotHotkeys( int hotkey );
  void                      SetMode( int mode );

  virtual void              ShowButtons( int /*btnId*/ );
  void                      RemoveMainButton();
  void                      RemoveButtons();

  virtual bool              MatchViewFlags( int viewFlags )
  {
    // If the button requires debug mode, check if the client is in debug
#if !defined DEBUG
    if ((viewFlags & FLAG_DEBUG) == FLAG_DEBUG)
    {
      return NULL;
    }
#else
    VOLATILE_ARG( viewFlags );
#endif
    return true;
  }

  template <typename OBJ_CLASS>
  CoreGuiButton*            CreateMenuButton(int viewFlags, const String& title, int x, int y, uint32 resourceId, void(OBJ_CLASS::*callback)(int), bool showFrame)
  {
    if ( !MatchViewFlags( viewFlags ) )
    {
      return NULL;
    }

    float width = 0.1f;
    float height = 0.075f;//CoreOpenGLRender::GetRatio() * 0.035f;
    CoreGuiButton* btn = new CoreGuiButton(title, Vector2(-0.98f + width + (x * width * 2.2f), 1.0f + height - ((y + 1) * height * 2.2f)), Vector2(width, height), resourceId);
    btn->SetAlpha(0.9f);
    btn->SetDebugGui(true);
    btn->SetTextScale(0.6f);
    btn->SetColor(Color::Grey);
    btn->m_SignalButtonClicked.Connect(this, callback);

    btn->ShowBorders(true);
    if (showFrame)
    {
      btn->SetFrameColor(Color::Red);
      btn->SetTextColor(Color::Red);
    }
    return btn;
  };

  CoreGuiButton*            m_MainButton;
  CoreGuiBase*              m_ActiveObject;
  CoreGuiBase*              m_RevertObject;
  CoreGuiBase*              m_Menu;
  CoreEngine&               m_CoreEngine;
  int                       m_DebugMode;
  Vector2                   m_StartPos;
};
