#ifndef EVILENGINE_COREGUITOOLKIT_GUIELEMENTS_COREGUIVIEW
#define EVILENGINE_COREGUITOOLKIT_GUIELEMENTS_COREGUIVIEW

#include "../../CoreLib/BasicTypes/String/CoreString.h"
#include "../../CoreLib/Signal/SignalTarget.h"
#include "../BaseGuiElements/CoreGuiBase.h"
#include "../BaseGuiElements/CoreGuiButton.h"

class CoreGuiView : public CoreGuiBase, public SignalTarget
{
public:

  CoreGuiView( const Vector3& position, const Vector3& size );
  CoreGuiView( const String& name, const Vector3& position, const Vector3& size );
  ~CoreGuiView();

  static CoreGuiBase*         Initialize(){ return new CoreGuiView( Vector2::s_Vec2Zero, Vector2::s_Vec2One ); }

  virtual CoreGuiBase*        OnMouseButtonDown( int buttonId, float xPos, float yPos ) DF_OVERRIDE;

  CoreGuiButton*              AddYesButton();
  CoreGuiButton*              AddYesButton( const String& buttonTitle );
  CoreGuiButton*              AddNoButton();
  CoreGuiButton*              AddNoButton( const String& buttonTitle );
  CoreGuiButton*              AddNeutralButton();
  CoreGuiButton*              AddNeutralButton( const String& buttonTitle );
  CoreGuiButton*              AddButton( const String& buttonTitle, int buttonType, const Vector3& position );
  CoreGuiButton*              AddButton( CoreGuiButton* guiButton );

  Signal1<void,int>           m_SignalButtonDown;

  void                        SetHotkey( int buttonType, int hotkey );

#if defined DEBUG
  virtual String              GetStaticClassName() DF_OVERRIDE { return "CoreGuiView"; }
#endif

protected:

  static float                s_FrameSize;
  static float                s_TitleSize;
  static float                BORDER;

private:

  void                        BtnPressed( int btn );

  std::vector<CoreGuiButton*> m_ButtonList;
};

#endif /// EVILENGINE_COREGUITOOLKIT_GUIELEMENTS_COREGUIDIALOG
