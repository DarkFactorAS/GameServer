/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014-2016
* File        : GuiButtonTemplate
*
* Description : Helper class to create a button with callback
*
* Author      : Thor Richard Hansen
*************************************************************************************************/

#ifndef EVILENGINE_COREGUITOOLKIT_GUITEMPLATES_GUIBUTTONTEMPLATE
#define EVILENGINE_COREGUITOOLKIT_GUITEMPLATES_GUIBUTTONTEMPLATE

#include "EvilEngine/CoreGuiToolkit/CoreGuiToolkitInclude.h"

class GuiButtonTemplate
{
public:

  template <typename OBJ_CLASS, typename OBJ_ARG>
  void                      ConnectButton(CoreGuiButton *button, void (OBJ_CLASS::*callback)(OBJ_ARG)) const
  {
    if (button != NULL)
    {
      button->m_SignalButtonClicked.Connect(this, callback);
    }
  }

  template <typename OBJ_CLASS>
  void                      ConnectButton(CoreGuiButton *button, void (OBJ_CLASS::*callback)()) const
  {
    if (button != NULL)
    {
      button->m_SignalButtonClicked.Connect(this, callback);
    }
  }

  template <typename OBJ_CLASS, typename OBJ_ARG>
  void                      ConnectButton(CoreGuiBase* parent, const String& buttonName, void (OBJ_CLASS::*callback)(OBJ_ARG)) const
  {
    if (parent == NULL)
    {
      return;
    }
    CoreGuiButton* button = (CoreGuiButton*)parent->GetChildWithName(buttonName);
    if (button != NULL)
    {
      button->m_SignalButtonClicked.Connect(this, callback);
    }
  }

  template <typename OBJ_CLASS>
  void                      ConnectButton(CoreGuiBase* parent, const String& buttonName, void (OBJ_CLASS::*callback)()) const
  {
    if (parent == NULL)
    {
      return;
    }
    CoreGuiButton* button = (CoreGuiButton*)parent->GetChildWithName(buttonName);
    if (button != NULL)
    {
      button->m_SignalButtonClicked.Connect(this, callback);
    }
  }
};

#endif /// EVILENGINE_COREGUITOOLKIT_GUITEMPLATES_GUIBUTTONTEMPLATE
