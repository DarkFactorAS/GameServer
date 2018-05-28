
#ifndef EVILENGINE_COREGUITOOLKIT_BASEGUIELEMENTS_COREGUIBUTTON
#define EVILENGINE_COREGUITOOLKIT_BASEGUIELEMENTS_COREGUIBUTTON

#include "EvilEngine/CoreLib/BasicTypes/String/CoreString.h"
#include "EvilEngine/CoreLib/Signal/Signal.h"
#include "CoreGuiBase.h"
#include "EvilEngine/CoreGuiToolkit/CoreGuiResources.h"

class CoreGuiImageView;
class CoreGuiRoundedImageView;
class CoreGuiTextfield;
class CoreGuiFrame;

class CoreGuiButton : public CoreGuiBase
{
public:

  enum BUTTON_LAYER
  {
    LAYER_BACKGROUND = 1,
    LAYER_IMAGE,
    LAYER_TEXT,
    LAYER_BORDER,
    LAYER_CHILD,
  };

  enum BUTTON_GROUP
  {
    BUTTONGROUP_YESNO,
    BUTTONGROUP_CLOSE,
    BUTTONGROUP_CANCEL,
    BUTTONGROUP_OK,
    BUTTONGROUP_RETRYCANCEL,
    BUTTONGROUP_ABORT,
  };

  enum BUTTON_TYPE
  {
    BUTTONTYPE_NONE   = 0,
    BUTTONTYPE_YES    = 1,
    BUTTONTYPE_NO     = 2,
    BUTTONTYPE_CLOSE  = 3,
  };

  CoreGuiButton( const String& text, const Vector2& position, uint32 resourceId );
  CoreGuiButton( const String& text, const Vector2& position, const Vector2& size, uint32 resourceId );
  CoreGuiButton( int buttonId, const String& text, const Vector2& position, uint32 resourceId );
  CoreGuiButton( const String& name, int buttonId, const String& text, const Vector2& position, const Vector2& size, uint32 resourceId );
  ~CoreGuiButton();

  static CoreGuiBase*       Initialize(){ return new CoreGuiButton( "",Vector2::s_Vec2One, Vector2::s_Vec2One, GuiResource_ButtonBackground ); }
  virtual void              SetMember( const String& key, const String& value ) DF_OVERRIDE;
  Color                     GetDefaultButtonColor( int buttonType );
  virtual void              ShowBorders( bool showBorder ) DF_OVERRIDE;
  virtual void              SetResourceId( uint32 resourceId ) DF_OVERRIDE;
  void                      SetBackground( uint32 resourceId, float alpha, const Color& color );
  void                      SetImage( CoreGuiImageView* image );
  void                      SetImage( uint32 resourceId, float alpha, const Color& color );
  void                      SetImage( uint32 resourceId );
  void                      SetBackgroundColor( const Color& color, float alpha );
  void                      SetImageColor( const Color& color );

  virtual void              SetSize( float x, float y ) DF_OVERRIDE;

  virtual CoreGuiBase*      OnMouseButtonDown( int buttonId, float xPos, float yPos ) DF_OVERRIDE;
  virtual CoreGuiBase*      OnMouseButtonUp( int buttonId, float xPos, float yPos ) DF_OVERRIDE;
  virtual bool              OnKeyDown( int keyCode  ) DF_OVERRIDE;
  void                      SetHotkey( int hotkey );
  virtual void              SetWireframe( bool isWireframe ) DF_OVERRIDE;
  CoreGuiBase*              GetSolidGuiUnderMouse( float xPos, float yPos ) DF_OVERRIDE;

  virtual void              SetColor( const Color& color ) DF_OVERRIDE { SetColor( color.x, color.y, color.z ); }
  virtual void              SetColor( float red, float green, float blue ) DF_OVERRIDE;
  virtual void              SetAlpha( float alpha ) DF_OVERRIDE;

  void                      SetText( const String& text );
  void                      SetTextScale( float scale );
  void                      SetTextColor( const Color& color );
  void                      SetTextColor( float red, float green, float blue );

  void                      SetButtonType( int buttonType ){ m_ButtonType = buttonType;}
  int                       GetButtonType() const { return m_ButtonType; }
  void                      SetFrameColor( const Color& color );

  void                      CreateButtons( bool useRoundedIcons );
  void                      UseRoundedIcons( bool useRoundedIcons );

  void                      SetEnabled( bool isEnabled );

#if defined DEBUG
  virtual String            GetStaticClassName() DF_OVERRIDE { return "CoreGuiButton"; }
#endif

  static float              s_DefaultWidth;
  static float              s_DefaultHeight;

  //Signal1<void, int>                      m_SignalButtonClicked;
  Signal2<void, int, CoreGuiButton*> m_SignalButtonClicked;
  Signal0<void>             m_SignalButtonReleased;

  static Color              ButtonColorNo;
  static Color              ButtonColorYes;

  static Vector2            s_DefaultSize;
  static bool               s_UseRoundedButtons;

private:

  void                      Init( int buttonType, const String& text, const Vector2& position, const Vector2& size, uint32 resourceId );

  CoreGuiRoundedImageView*  m_ButtonRoundBg;
  CoreGuiImageView*         m_ButtonNormalBg;
  CoreGuiImageView*         m_ButtonFrame;
  CoreGuiImageView*         m_Image;
  CoreGuiTextfield*         m_Title;
  CoreGuiFrame*             m_GuiFrame1;
  CoreGuiFrame*             m_GuiFrame2;
  int                       m_ButtonType;
  bool                      m_IsDown;
  float                     m_Textscale;
  Color                     m_FrameColor;
  bool                      m_UseRoundedIcons;
  bool                      m_IsEnabled;
};

#endif /// EVILENGINE_COREGUITOOLKIT_BASEGUIELEMENTS_COREGUIBUTTON
