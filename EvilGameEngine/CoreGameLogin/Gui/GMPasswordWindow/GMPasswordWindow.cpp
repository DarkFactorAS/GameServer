/*************************************************************************************************
* Project     : Valyrian Adventures 2014-2016
* File        : GMPasswordWindow
*
* Description : Debug GUI that displays a password inputfield to enable the user to elevate his
*               account to a GM account. 
*
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "GMPasswordWindow.h"
#include "EvilEngine/CoreBugReport/Client/BugReportClient.h"
#include "EvilEngine/CoreBugReport/Common/BugReport.h"
#include "EvilEngine/CoreGuiToolkit/BaseGuiElements/CoreGuiInputfield.h"
#include "EvilGameEngine/CoreGameLogin/Account/Account.h"
//#include "ProjectGameEngine/Gui/GuiElements/ProjectGuiDialog.h"

GMPasswordWindow::GMPasswordWindow( const String& windowName ) :
  CoreGuiXMLView( "GMPasswordWindow.xml")
{
  SetName( windowName );
}

void GMPasswordWindow::OnDataLoaded( Resource* /*resource*/ )
{
  ConnectButton( "XButton", &GMPasswordWindow::Cancel );
  CoreGuiInputfield* guiPassword = (CoreGuiInputfield*)GetChildWithName("password");
  if ( guiPassword != NULL )
  {
    guiPassword->m_ActionEvent.Connect(this, &GMPasswordWindow::VerifyPassword);
  }
}

void GMPasswordWindow::Cancel( int /*btnId*/ )
{
  Destroy();
}

void GMPasswordWindow::VerifyPassword( int /*btnId*/ )
{
  CoreGuiInputfield* guiPassword   = safe_cast< CoreGuiInputfield* > ( GetChildWithName("password") );
  if (guiPassword != NULL && !guiPassword->GetText().IsEmpty() )
  {
    SignalPassword( guiPassword->GetText() );
  }

  Destroy();
}
