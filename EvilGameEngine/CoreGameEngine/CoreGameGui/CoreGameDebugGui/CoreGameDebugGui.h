#ifndef EVILENGINE_COREGAMEENGINE_COREGAMEGUI_COREGAMEENGINEDEBUGGUI
#define EVILENGINE_COREGAMEENGINE_COREGAMEGUI_COREGAMEENGINEDEBUGGUI 1
#pragma once

#include "EvilEngine/CoreGuiToolkit/DebugGui/DebugGui.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"

class CoreGameDebugGui : public DebugGui
{
public:
  CoreGameDebugGui( CoreGameEngine& parent, bool autoOpen );

  CoreGameDebugGui& operator  =   ( const CoreGameDebugGui& other )
  {
    DebugGui::operator=(other);
    m_CoreGameEngine        = other.m_CoreGameEngine;
    return *this;
  }

  //virtual void              ToggleDebug()               DF_OVERRIDE;

protected:

  virtual void              ShowButtons( int /*btnId*/ ) DF_OVERRIDE;
  CoreGameEngine*           GetCoreGameEngine() {
    return &m_CoreGameEngine;
  }

  CoreGameEngine&           m_CoreGameEngine;

private:

  void                      SlotEnableLocalhost( int /*btnId*/ );
  void                      SlotEnableTestLive( int /*btnId*/ );
  void                      SlotEnableLive( int /*btnId*/ );
  void                      ShowVersionText( int /*btnId*/ );
  bool                      IsVersionShown();
  void                      ToggleFPS( int /*btnId*/ );
  bool                      IsFPSShown();
  void                      SlotShowResolutionWindow(int btnId);
};

#endif /// EVILENGINE_COREGAMEENGINE_COREGAMEENGINEDEBUGGUI