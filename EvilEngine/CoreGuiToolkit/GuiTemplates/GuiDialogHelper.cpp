/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014-2016
* File        : GuiDialogHelper
*
* Description : Helper class to manage the creation and destruction of a modal dialog window
*
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "GuiDialogHelper.h"
#include "EvilEngine/CoreGuiToolkit/GuiElements/CoreGuiDialog.h"

GuiDialogHelper::GuiDialogHelper() :
  m_Parent( NULL ),
  m_DialogWindow(NULL)
{
}

GuiDialogHelper::~GuiDialogHelper()
{
  if ( m_DialogWindow != NULL )
  {
    m_DialogWindow->Destroy();
  }
}

/************************************************************************************************
* HideDialog:
* Manually close the dialog window from the HUD
*
* @author Thor Richard Hansen
*************************************************************************************************/
void GuiDialogHelper::HideDialog()
{
  if ( m_DialogWindow != NULL )
  {
    m_DialogWindow->Destroy();
    m_DialogWindow = NULL;
  }
}

/************************************************************************************************
* DestroyDialog:
* Callback for when the dialog is closing. Reset the pointer to not have a dangling pointer here.
*
* @author Thor Richard Hansen
*************************************************************************************************/
void GuiDialogHelper::OnDialogDestroyed( const CoreBaseObject* dialog )
{
  if ( m_DialogWindow == dialog )
  {
    m_DialogWindow = NULL;
  }
}
