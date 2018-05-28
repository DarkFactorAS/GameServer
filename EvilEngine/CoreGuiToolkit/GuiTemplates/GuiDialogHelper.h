#ifndef EVILENGINE_COREGUITOOLKIT_GUITEMPLATES_GUIDIALOGHELPER
#define EVILENGINE_COREGUITOOLKIT_GUITEMPLATES_GUIDIALOGHELPER

#include "EvilEngine/CoreGuiToolkit/CoreGuiToolkitInclude.h"
#include "EvilEngine/CoreLib/Signal/SignalTarget.h"
#include "EvilEngine/CoreLib/Assert/DFAssert.h"
#include "EvilEngine/CoreGuiToolkit/CoreGuiToolkit.h"

class CoreGuiDialog;

class GuiDialogHelper : public SignalTarget
{
public:

  GuiDialogHelper();
  ~GuiDialogHelper();

  void                      HideDialog();
  void                      OnDialogDestroyed(const CoreBaseObject* dialog);
  void                      SetParent( CoreGuiBase* parent){ m_Parent = parent; }

#ifdef CLIENT
  template <typename OBJ_CLASS>
  void                      ShowDialog(const String& title, const String& message, void (OBJ_CLASS::*callback)())
  {
    HideDialog();
    dfBugreport(m_Parent != NULL, "GuiDialogHelper: Parent is NULL!");
    if (m_Parent != NULL)
    {
      m_DialogWindow = safe_cast<CoreGuiDialog*> (CoreGuiToolkit::GetInstance()->CreateGuiObject(CoreGuiToolkit::GuiObject_Dialog));
      m_DialogWindow->SetTitle(title);
      m_DialogWindow->SetMessage(message);
      m_DialogWindow->SetSize(0.5f, 0.5f);
      m_DialogWindow->CreateButtonGroup(CoreGuiButton::BUTTONGROUP_CLOSE);
      m_DialogWindow->m_SignalButtonDown.Connect(this, callback);
      m_DialogWindow->m_SignalOnDeath.Connect(this, &GuiDialogHelper::OnDialogDestroyed);
      m_DialogWindow->SetHotkey(CoreGuiButton::BUTTONTYPE_CLOSE, Keyboard::KEY_RETURN);
      m_Parent->AddChild(m_DialogWindow);
    }
  }

  template <typename OBJ_CLASS>
  void              ShowDialog(const String& title, const String& message, int buttonGroup, void (OBJ_CLASS::*callback)())
  {
    HideDialog();
    if (m_Parent != NULL)
    {
      m_DialogWindow = safe_cast<CoreGuiDialog*> (CoreGuiToolkit::GetInstance()->CreateGuiObject(CoreGuiToolkit::GuiObject_Dialog));
      m_DialogWindow->SetTitle(title);
      m_DialogWindow->SetMessage(message);
      m_DialogWindow->SetSize(0.5f, 0.5f);
      m_DialogWindow->CreateButtonGroup(buttonGroup);
      m_DialogWindow->m_SignalButtonDown.Connect(this, callback);
      m_DialogWindow->m_SignalOnDeath.Connect(this, &GuiDialogHelper::OnDialogDestroyed);
      m_DialogWindow->SetHotkey(CoreGuiButton::BUTTONTYPE_CLOSE, Keyboard::KEY_RETURN);
      m_Parent->AddChild(m_DialogWindow);
    }
  }

  template <typename OBJ_CLASS, typename ARG1>
  void              ShowDialog(const String& title, const String& message, int buttonGroup, void (OBJ_CLASS::*callback)(ARG1))
  {
    HideDialog();
    if (m_Parent != NULL)
    {
      m_DialogWindow = safe_cast<CoreGuiDialog*> (CoreGuiToolkit::GetInstance()->CreateGuiObject(CoreGuiToolkit::GuiObject_Dialog));
      m_DialogWindow->SetTitle(title);
      m_DialogWindow->SetMessage(message);
      m_DialogWindow->SetSize(0.5f, 0.5f);
      m_DialogWindow->CreateButtonGroup(buttonGroup);
      m_DialogWindow->m_SignalButtonDown.Connect(this, callback);
      m_DialogWindow->m_SignalOnDeath.Connect(this, &GuiDialogHelper::OnDialogDestroyed);
      m_DialogWindow->SetHotkey(CoreGuiButton::BUTTONTYPE_CLOSE, Keyboard::KEY_RETURN);
      m_Parent->AddChild(m_DialogWindow);
    }
  }
#endif

protected:

  CoreGuiDialog*            m_DialogWindow;
  CoreGuiBase*              m_Parent;

};

#endif /// EVILENGINE_COREGUITOOLKIT_GUITEMPLATES_GUIDIALOGHELPER
