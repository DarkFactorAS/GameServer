#ifndef EVILENGINE_COREGAMEENGINE_COREGAMEGUI_DEBUGGUIRESOLUTIONDIALOG
#define EVILENGINE_COREGAMEENGINE_COREGAMEGUI_DEBUGGUIRESOLUTIONDIALOG 1
#pragma once

#include "EvilEngine/CoreGuiToolkit/BaseGuiElements/CoreGuiXMLView.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"

class DebugGuiResolutionDialog : public CoreGuiXMLView
{
public:
  DebugGuiResolutionDialog(const String& windowName, CoreGameEngine& parent);

  DebugGuiResolutionDialog& operator  =   (const DebugGuiResolutionDialog& other)
  {
    CoreGuiXMLView::operator=(other);
    m_Parent = other.m_Parent;
    return *this;
  }

  virtual void  OnDataLoaded(Resource* resource) DF_OVERRIDE;
  void          SlotChangeResolution(int btnId);
  void          SlotCloseWindow(int btnId);

  enum RESOLUTIONS
  {
    RESOLUTION_SAMSUNG_S3 = 1000,
    RESOLUTION_IPAD_MINI = 1001,
    RESOLUTION_HP_LAPTOP = 1002,

    RESOLUTION_320 = 1100,
    RESOLUTION_640 = 1101,
    RESOLUTION_1024 = 1102,
    RESOLUTION_1920 = 1103,

    RESOLUTION_TOGGLE_FULLSCREEN = 1200,
    RESOLUTION_BORDERLESS = 1201,
  };

private:

  CoreGameEngine&           m_Parent;

};

#endif /// EVILENGINE_COREGAMEENGINE_COREGAMEGUI_DEBUGGUIRESOLUTIONDIALOG