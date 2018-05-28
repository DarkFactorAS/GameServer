#ifndef EVILENGINE_COREGUITOOLKIT_GUIELEMENTS_COREGUIWINDOW
#define EVILENGINE_COREGUITOOLKIT_GUIELEMENTS_COREGUIWINDOW

#include "../../CoreLib/BasicTypes/String/CoreString.h"
#include "CoreGuiView.h"
#include <functional>

class CoreGuiFrame;
class CoreGuiTextfield;
class CoreGuiImageView;
class CoreGuiView;

class CoreGuiWindow : public CoreGuiView
{
public:

  CoreGuiWindow();
  CoreGuiWindow( const String& title, const Vector2& position, const Vector2& size );
  CoreGuiWindow( const String& title, const Vector2& position, const Vector2& size, uint32 bgResourceId, bool showBorders );

  //virtual void                ShowBorders( bool showBorders ) DF_OVERRIDE;

  static CoreGuiBase*         Initialize(){ return new CoreGuiWindow( "", Vector2::s_Vec2Zero, Vector2::s_Vec2One ); }
  virtual void                SetMember( const String& key, const String& value ) DF_OVERRIDE;

  void                        SetTitle( const String& title );

  CoreGuiBase*                OnMouseButtonUp( int buttonId, float xPos, float yPos ) DF_OVERRIDE;
  CoreGuiBase*                OnMouseButtonDown( int buttonId, float xPos, float yPos ) DF_OVERRIDE;
  virtual bool                OnMouseDrag( float  xPos , float  yPos  ) DF_OVERRIDE;
  float                       GetContentTop();
  const Vector3&              GetContentSize();
  const Vector3&              GetContentPosition();
  void                        CloseWindow( int btnId );

  void                        SetTitleSize( float x, float y );
  virtual bool                GainFocus() DF_OVERRIDE { return true; } ;
  void                        SetTitleBgColor( const Color& color );
  void                        SetBgColor( const Color& color );

#if defined DEBUG
  virtual String              GetStaticClassName() DF_OVERRIDE { return "CoreGuiWindow"; }
#endif

protected:

  void                        InitWindow( const String& title, const Vector2& position, const Vector2& size, uint32 bgResourceId, bool showBorders );

  CoreGuiImageView*           m_WindowTitleBg;
  CoreGuiTextfield*           m_WindowTitleText;
  CoreGuiImageView*           m_WindowBackground;
  CoreGuiView*                m_ContentView;

  bool                        m_Drag;
  float                       m_TitleHeight;
  float                       m_BackgroundAlpha;
  Color                       m_WindowTitleBgColor;
  Color                       m_WindowBgColor;
};

#endif /// EVILENGINE_COREGUITOOLKIT_GUIELEMENTS_COREGUIWINDOW
