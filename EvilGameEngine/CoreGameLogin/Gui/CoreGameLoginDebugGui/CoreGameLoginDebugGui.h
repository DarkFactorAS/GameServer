#ifndef EVILENGINE_COREGAMELOGIN_COREGAMELOGINDEBUGGUI
#define EVILENGINE_COREGAMELOGIN_COREGAMELOGINDEBUGGUI 1
#pragma once

#ifdef CLIENT

#include "EvilGameEngine/CoreGameEngine/CoreGameGui/CoreGameDebugGui/CoreGameDebugGui.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"

class CoreGameLoginDebugGui : public CoreGameDebugGui
{
public:
  CoreGameLoginDebugGui( CoreGameEngine& parent, bool autoOpen );

  void                      LoginGM(String password);

protected:

  virtual bool              MatchViewFlags(int viewFlags) DF_OVERRIDE;
  virtual void              ShowButtons( int /*btnId*/ ) DF_OVERRIDE;
  virtual void              SlotToggleGM(int /*btnId*/);

  virtual void              SlotLoginGM(int /*btnId*/);
  virtual void              SlotSwitchUser(int /*btnId*/){};
};
#endif
#endif /// EVILENGINE_COREGAMELOGIN_COREGAMELOGINDEBUGGUI