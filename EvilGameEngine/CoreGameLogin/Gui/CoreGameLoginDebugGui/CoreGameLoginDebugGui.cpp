
#include "PreCompile.h"

#ifdef CLIENT

#include "CoreGameLoginDebugGui.h"
//#include "EvilEngine/CoreGuiToolkit/CoreGuiToolkitInclude.h"
#include "EvilGameEngine/CoreGameLogin/Account/Account.h"
#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ServerAccountNetworkPackets/LoginGMNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/Gui/GMPasswordWindow/GMPasswordWindow.h"

CoreGameLoginDebugGui::CoreGameLoginDebugGui( CoreGameEngine& parent, bool autoOpen ) :
  CoreGameDebugGui( parent, autoOpen )
{
}

bool CoreGameLoginDebugGui::MatchViewFlags(int viewFlags)
{
  Account* clientAccount = Account::GetClientAccount();

  // If the button requires GM powers, check if the client is GM enabled
  //if (((viewFlags & FLAG_CANGM) == FLAG_CANGM) && !clientAccount->ClientHasGMPowers())
  //{
  //  return NULL;
  //}

  // If the button requires GM powers, check if the client is GM enabled
  if (((viewFlags & FLAG_HASCLIENTGM) == FLAG_HASCLIENTGM) && !clientAccount->HasClientGMFlag())
  {
    return false;
  }

  // If the button requires GM powers, check if the client is GM enabled
  if (((viewFlags & FLAG_HASSERVERGM) == FLAG_HASSERVERGM) && !clientAccount->HasServerGMFlag())
  {
    return false;
  }

  // If the button requires GM powers, check if the client is GM enabled
  if (((viewFlags & FLAG_ISGM) == FLAG_ISGM) && !clientAccount->IsClientGMEnabled())
  {
    return false;
  }

  return CoreGameDebugGui::MatchViewFlags( viewFlags );
}

void CoreGameLoginDebugGui::ShowButtons( int btnId )
{
  CoreGameDebugGui::ShowButtons( btnId );
  if ( m_Menu != NULL )
  {
    Account* clientAccount = Account::GetClientAccount();

    m_Menu->AddChild(CreateMenuButton(FLAG_NONE, "Enable|GM", 2, 4, GuiResource_ButtonBackground, &CoreGameLoginDebugGui::SlotLoginGM, false ));
    if ( clientAccount != NULL && Account::GetClientAccount()->HasClientGMFlag() )
    {
      m_Menu->AddChild(CreateMenuButton(FLAG_HASCLIENTGM, "Toggle|GM", 3, 4, GuiResource_ButtonBackground, &CoreGameLoginDebugGui::SlotToggleGM, Account::GetClientAccount()->IsGMEnabled() ));
    }
    if ( clientAccount != NULL && clientAccount->IsAuthenticated() && clientAccount->HasServerGMFlag() )
    {
      m_Menu->AddChild(CreateMenuButton(FLAG_HASSERVERGM, "Switch|User", 4, 4, GuiResource_ButtonBackground, &CoreGameLoginDebugGui::SlotSwitchUser, false));
    }
  }
}

void CoreGameLoginDebugGui::SlotToggleGM( int /*btnId*/ )
{
  if ( Account::GetClientAccount() != NULL && Account::GetClientAccount()->HasClientGMFlag() )
  {
    Account::GetClientAccount()->ToggleGM();
    RefreshButtons();
  }
}

void CoreGameLoginDebugGui::SlotLoginGM(int /*btnId*/)
{
    CoreGuiBase* parent = m_Menu->GetTopParent();
    parent->RemoveChild( parent->GetChildWithName("GMPasswordWindow"));
    GMPasswordWindow* passwordWindow = new GMPasswordWindow("GMPasswordWindow");
    passwordWindow->SignalPassword.Connect( this, &CoreGameLoginDebugGui::LoginGM );
    parent->AddChild( passwordWindow );
}

void CoreGameLoginDebugGui::LoginGM( String password )
{
    // Enable client side GM ( enable both for testing right now )
    Account* account = Account::GetClientAccount();
    if ( account != NULL )
    {
        if ( account->EnableGM( password ) )
        {
            RefreshButtons();
        }
    }
    
    // Request GM on the server
    CoreGameEngine* gameEngine = GetCoreGameEngine();
    if ( gameEngine != NULL )
    {
      gameEngine->SendPacketToServer(new LoginGMNetworkPacket(password));
    }
}

#endif
