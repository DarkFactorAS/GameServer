#ifndef EVILENGINE_COREGUITOOLKIT_GUIELEMENTS_COREGUIDIALOG
#define EVILENGINE_COREGUITOOLKIT_GUIELEMENTS_COREGUIDIALOG

#include "../../CoreLib/BasicTypes/String/CoreString.h"
#include "CoreGuiWindow.h"

class CoreGuiTextfield;

class CoreGuiDialog : public CoreGuiWindow
{
public:

  CoreGuiDialog();
  CoreGuiDialog( const String& title, const String& message, const Vector2& position, const Vector2& size );
  CoreGuiDialog( const String& title, const String& message, const Vector2& position, const Vector2& size, int buttonGroup );
  virtual CoreGuiBase*        OnMouseButtonDown( int buttonId, float xPos, float yPos ) DF_OVERRIDE;
  static CoreGuiBase*         Initialize(){ return new CoreGuiDialog( "", "", Vector2::s_Vec2Zero, Vector2::s_Vec2One ); }
  void                        SetMessage(const String& message);

#if defined DEBUG
  virtual String              GetStaticClassName() DF_OVERRIDE { return "CoreGuiDialog"; }
#endif
  virtual void                CreateButtonGroup(int buttonGroup);

protected:
  CoreGuiTextfield* m_BodyText;
};

#endif /// EVILENGINE_COREGUITOOLKIT_GUIELEMENTS_COREGUIDIALOG
