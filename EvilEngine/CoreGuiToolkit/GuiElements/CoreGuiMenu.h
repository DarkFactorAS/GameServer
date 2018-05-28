#ifndef EVILENGINE_COREGUITOOLKIT_GUIELEMENTS_COREGUIMENU
#define EVILENGINE_COREGUITOOLKIT_GUIELEMENTS_COREGUIMENU

#include "../BaseGuiElements/CoreGuiButton.h"
#include "EvilEngine/CoreLib/Signal/SignalTarget.h"

class CoreGuiButton;

class CoreGuiMenu : public CoreGuiBase, public SignalTarget
{
public:

  enum ALIGNMENT
  {
    ALIGNMENT_DOWNARDS
  };

  CoreGuiMenu( const String& title, const Vector2& position, const Vector2& size, uint32 resourceId, int direction );

  CoreGuiButton*                AddSubMenu( const String& title, const Color& color );
  void                          Show( bool showSubMenu );
  void                          AddSpacer();
  void                          Clear();
  void                          ShowMainMenu( bool showMainMenu );

#if defined DEBUG
  virtual String              GetStaticClassName() DF_OVERRIDE { return "CoreGuiMenu"; }
#endif

private:
  void                          SlotToggleMenu( int /*btnId*/ );

  std::vector< CoreGuiButton* > m_SubMenuList;
  int                           m_Direction;
  bool                          m_Show;
  int                           m_MenuId;
  float                         m_Offset;
};

#endif /// EVILENGINE_COREGUITOOLKIT_GUIELEMENTS_COREGUIMENU
