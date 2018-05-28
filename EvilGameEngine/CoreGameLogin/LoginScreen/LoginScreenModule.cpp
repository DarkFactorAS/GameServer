/*************************************************************************************************
 * Project     : EvilEngine Multiplatform 2014
 * File        : MainMenuScreenModule
 *
 * Description : All the code that is needed to run and render the main menuscreen. The main
 *               menu screen will just list a row of buttons that starts the game, closes the
 *               client or shows show debug information.
 *
 * Author      : Thor Richard Hansen
 *************************************************************************************************/

#include "PreCompile.h"
#include "LoginScreenModule.h"

#if defined CLIENT
  #include "EvilEngine/CoreRendering/OpenGLRender/CoreOpenGLRender.h"
  #include "EvilEngine/CoreRendering/RenderObjects/TextStringRenderObject.h"
  #include "EvilEngine/CoreGuiToolkit/BaseGuiElements/CoreGuiButton.h"
  #include "EvilEngine/CoreVersion/version.h"
#endif

#include "EvilEngine/CoreNetwork/Client/NetworkClient.h"
#include "EvilEngine/CoreEngine/CoreResourceManager/CoreResourceManager.h"
#include "EvilEngine/CoreNetwork/Client/NetworkClient.h"
#include "EvilEngine/CoreGuiToolkit/GuiCreator/GuiResource.h"
#include "EvilEngine/CoreGuiToolkit/CoreGuiResources.h"
#include "EvilEngine/CoreGuiToolkit/CoreGuiToolkit.h"
#include "EvilEngine/CoreGuiToolkit/CoreGuiToolkitInclude.h"

#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/GameEnginePacketData.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/Packets/RequestServerVersionNetworkPacket.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/Packets/ServerErrorNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/Network/GameLoginPacketData.h"
#include "EvilGameEngine/CoreGameLogin/Account/Account.h"
#include "EvilGameEngine/CoreGameLogin/Account/AccountManager.h"
#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ServerAccountNetworkPackets/CreateServerAccountNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ServerAccountNetworkPackets/LoginServerAccountNetworkPacket.h"

namespace EvilEngine
{
  Signal4< void, String, String, String, uint8 >  SignalLoginAccount;
  Signal1< void, String >                         SignalLoginFailed;
}

LoginScreenModule::LoginScreenModule( int screenId, CoreGameEngine& parent) :
  CoreScreenModule( screenId, parent )
  , m_DialogWindow( NULL )
  , m_AccountMode( ACCOUNTMODE_NONE )
  , m_NetworkMode( ACCOUNTMODE_NONE )
  , m_GuiMenu( NULL )
#if defined CLIENT
  , m_AccountInfoView(NULL)
#endif
{
#if defined CLIENT
  ConnectButton( m_HUD, "BtnCloseGame",  &LoginScreenModule::SlotShowCloseGameDialog );

  InitAccountManager();

  m_GameEngine.SignalChangeServer.Connect( this, &LoginScreenModule::OnChangedServers);

  Account::s_SignalReceivedAccountInfo.Connect( this, &LoginScreenModule::OnReceivedAccountInfo  );
  ServerErrorNetworkPacket::s_SignalPacketError.Connect(this, &LoginScreenModule::OnServerError);

  // Hook up the login signal in case we want to autocreate the user based on the device login
  //m_GameEngine.SignalLoginAccount.Connect( this, &LoginAccountScreenModule::SlotLoginDeviceAccount );
  EvilEngine::SignalLoginAccount.Connect( this, &LoginScreenModule::SlotLoginDeviceAccount );
  EvilEngine::SignalLoginFailed.Connect( this, &LoginScreenModule::SlotLoginFailed );

  m_GameEngine.GetResourceManager()->SyncLoadResource(new GuiResource(m_HUD, "LoginAccountScreen_HUD.xml"));

  // Load and send locally stored bugreports
  //BugReportNetworkClient* bugClient = BugReportNetworkClient::GetInstanceIfAny();  
  //if ( bugClient != NULL )
  //{
  //  bugClient->Load();
  //}
#endif
}

LoginScreenModule::~LoginScreenModule()
{
#if defined CLIENT
  m_GameEngine.RemoveGuiObject(m_AccountInfoView);
#endif
}

/************************************************************************************************
* ShowAccountInfo:
* If we have a logged in account, display the name of the account on top of the screen
*
* @author Thor Richard Hansen
*************************************************************************************************/
#if defined CLIENT
void LoginScreenModule::ShowAccountInfo()
{
  Vector2 position(0.0f, -0.9f);
  Vector2 size(0.4f, 0.07f);

  Account* account = Account::GetActiveAccount();
  if (account != NULL && m_AccountInfoView == NULL)
  {
    m_AccountInfoView = new CoreGuiView(position, size);
    m_AccountInfoView->AddChild(new CoreGuiImageView(position, size, Color::Yellow));
    m_AccountInfoView->AddChild(new CoreGuiTextfield(account->GetName(), position, size, TextStringRenderObject::ALIGNMENT_CENTER));
    m_AccountInfoView->AddChild(new CoreGuiFrame(position, size, Color::Black));
    m_GameEngine.AddGuiObject(m_AccountInfoView);
  }
  else
  {
    m_GameEngine.RemoveGuiObject(m_AccountInfoView);
    m_AccountInfoView = NULL;
  }
}
#endif

#if defined CLIENT
void LoginScreenModule::InitAccountManager()
{
  AccountManager* accountManager = AccountManager::GetInstance();
  if ( accountManager == NULL )
  {
    m_AccountMode = ACCOUNTMODE_MENU;
  }
  else
  {
    Account* account = accountManager->LoadActiveAccount();
    if ( account == NULL || !account->HasLoginData() || account->GetLoginMethod() == 0 || account->IsAuthenticated() )
    {
      m_AccountMode = ACCOUNTMODE_MENU;
    }
    else if ( account != NULL && account->AutoLogin() )
    {
      m_AccountMode = ACCOUNTMODE_AUTOLOGIN;
    }
    else
    {
      m_AccountMode = ACCOUNTMODE_MENU;
    }
  }
}

/************************************************************************************************
* SlotShowOptionMenu:
* Show an option gui to delete account, log out etc.
*
* @param  (int)   btnId         - Id of the button that initiated the callback
*
* @author Thor Richard Hansen
*************************************************************************************************/
void LoginScreenModule::SlotShowOptionMenu( int /*btnId*/ )
{
#if defined CLIENT
  SlotShowCloseGameDialog();
  //CoreGuiDialog* guiDialog = new CoreGuiDialog("Reset Account", "Are you sure you want\n to reset the account ?", Vector2::s_Vec2Zero, Vector2( 0.6f, 0.5f ), CoreGuiButton::BUTTONGROUP_YESNO );
  //m_GameEngine.AddGuiObject( guiDialog );
  //guiDialog->m_SignalButtonDown.Connect( this, &CoreGameLoginScreen::SlotHandleOptionMenu );
  //guiDialog->SetHotkey( CoreGuiButton::BUTTONTYPE_YES, CoreGameEngine::KeyboardYes() );
  //guiDialog->SetHotkey( CoreGuiButton::BUTTONTYPE_NO, CoreGameEngine::KeyboardNo() );
#endif
}

/************************************************************************************************
 * SlotCloseGameDialog:
 * Callback for when the user clicks a button in the close game dialog
 *
 * @param  (int)   btnId         - Id of the button that initiated the callback
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
void LoginScreenModule::SlotHandleOptionMenu( int btnId )
{
  if ( btnId == CoreGuiButton::BUTTONTYPE_YES )
  {
    Account::RemoveUserConfig();
  }
}

void LoginScreenModule::SlotCloseErrorDialog()
{
}

/************************************************************************************************
* SlotShowCloseGameDialog:
* Show a quit openGL dialog window when the player wants to close the client. This callback
* function is hooked up to the quit button.
*
* @param  (int)   btnId         - Id of the button that initiated the callback
*
* @author Thor Richard Hansen
*************************************************************************************************/
void LoginScreenModule::SlotShowCloseGameDialog()
{
#if defined CLIENT
  CoreGuiDialog* guiDialog = new CoreGuiDialog("Close Client", "Are you sure you want\n to close the client ?", Vector2::s_Vec2Zero, Vector2( 0.6f, 0.5f ), CoreGuiButton::BUTTONGROUP_YESNO );
  m_GameEngine.AddGuiObject( guiDialog );
  guiDialog->m_SignalButtonDown.Connect( this, &LoginScreenModule::SlotCloseGameDialog );
  guiDialog->SetHotkey( CoreGuiButton::BUTTONTYPE_YES, CoreGameEngine::KeyboardYes() );
  guiDialog->SetHotkey( CoreGuiButton::BUTTONTYPE_NO, CoreGameEngine::KeyboardNo() );
#endif
}

/************************************************************************************************
 * SlotCloseGameDialog:
 * Callback for when the user clicks a button in the close game dialog
 *
 * @param  (int)   btnId         - Id of the button that initiated the callback
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
void LoginScreenModule::SlotCloseGameDialog( int btnId )
{
  if ( btnId == CoreGuiButton::BUTTONTYPE_YES )
  {
#if defined PLATFORM_IOS || defined PLATFORM_OSX
    m_GameEngine.AddMessage( CoreGameEngine::MESSAGE_CLOSE_APPLICATION );
#elif defined PLATFORM_ANDROID
    exit(0);
#elif defined PLATFORM_WINDOWS
    PostQuitMessage( 0 );
#endif
  }
}

/************************************************************************************************
 * ShowMenu:
 * Display the menu
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
void LoginScreenModule::ShowMenu()
{
  switch( m_AccountMode )
  {
    case ACCOUNTMODE_MENU:
      SlotShowMainMenu();
      break;

    case ACCOUNTMODE_CREATE:
      ShowCreateAccountWindow();
      break;

    case ACCOUNTMODE_LOGIN:
      ShowLoginAccountWindow();
      break;

    case ACCOUNTMODE_AUTOLOGIN:
      {
        SlotShowMainMenu();

        Account* account = Account::GetActiveAccount();
        if ( account != NULL )
        {
          account->SetAutoLogin( false );
          
          switch( account->GetLoginMethod() )
          {
            case Account::LOGIN_METHOD_DARKFACTOR:
            case Account::LOGIN_METHOD_ANDROIDUUID:
              {
                m_NetworkMode = ACCOUNTMODE_AUTOLOGIN;
                LoginAccount(account);
              }
              break;

            case Account::LOGIN_METHOD_IOSGAMECENTERCREATE:
            case Account::LOGIN_METHOD_IOSGAMECENTERLOGIN:
              {
                // Only log in with game center if it is enabled etc.
                // Seperate the platform login to have a seperate ios login and ios auto login
                //m_GameEngine.AddMessage( CoreGameEngine::MESSAGE_PLATFORM_LOGIN );
                
                // Send a message to the server to auto log in Game Center
                account->SetLoginMethod( Account::LOGIN_METHOD_IOSGAMECENTERLOGIN );

                m_NetworkMode = ACCOUNTMODE_AUTOLOGIN;
                LoginAccount(account);
              }
              break;

            default:
              break;
          }
        }
      }
      break;

    default:
      break;
  }
}

/************************************************************************************************
 * HideMenu:
 * Removes all the menu buttons
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
void LoginScreenModule::HideMenu()
{
  m_GuiMenu = m_HUD->RemoveChild( m_GuiMenu );
  dfAssert( m_GuiMenu == NULL, "Gui Menu was not removed!");
}

#ifdef CLIENT
void LoginScreenModule::SlotShowMainMenu()
{
  HideMenu();

  m_AccountMode = ACCOUNTMODE_MENU;
  m_GuiMenu = m_HUD->AddChild( new CoreGuiBase() );
  m_GuiMenu->SetName("GuiMenu");

  Resource* res = NULL;

  Account* account = Account::GetActiveAccount();
  if ( account != NULL && account->IsAuthenticated() )
  {
    res = m_GameEngine.GetResourceManager()->SyncLoadResource( new GuiResource( m_GuiMenu, "AccountScreen_Menu_Authenticated.xml" ) );

    ConnectButton( m_GuiMenu, "BtnAnonymous",     &LoginScreenModule::SlotLoginAnonymously ); 
    ConnectButton( m_GuiMenu, "BtnPlayOnline",    &LoginScreenModule::SlotPlayOnlineGame ); 
    ConnectButton( m_GuiMenu, "BtnCloseGame",     &LoginScreenModule::SlotShowCloseGameDialog );
    ConnectButton( m_GuiMenu, "BtnOptionMenu",     &LoginScreenModule::SlotShowOptionMenu );

    m_GuiMenu->SetChildMember( "BtnLogout", "visible", "true" );
    m_GuiMenu->SetChildMember( "BtnRemoveAccount", "visible", "false" );
    ConnectButton( m_GuiMenu, "BtnLogout",        &LoginScreenModule::SlotLogoutAccount ); 
  }
  else
  {
#if defined PLATFORM_ANDROID
    res = m_GameEngine.GetResourceManager()->SyncLoadResource( new GuiResource( m_GuiMenu, "AccountScreen_Menu_Android.xml" ) );
#elif defined PLATFORM_IOS
    res = m_GameEngine.GetResourceManager()->SyncLoadResource( new GuiResource( m_GuiMenu, "AccountScreen_Menu_iOS.xml" ) );
#endif
    if ( res == NULL )
    {
      res = m_GameEngine.GetResourceManager()->SyncLoadResource( new GuiResource( m_GuiMenu, "AccountScreen_Menu_Windows.xml" ) );
    }

    
    ConnectButton( m_GuiMenu, "BtnLoginAccount",  &LoginScreenModule::ChooseLoginOrCreateAccount );
    ConnectButton( m_GuiMenu, "BtnAnonymous",     &LoginScreenModule::SlotLoginAnonymously ); 
    ConnectButton( m_GuiMenu, "BtnPlatformLogin", &LoginScreenModule::InitiatePlatformLogin );
    ConnectButton( m_GuiMenu, "BtnCloseGame",     &LoginScreenModule::SlotShowCloseGameDialog );
    ConnectButton( m_GuiMenu, "BtnOptionMenu",     &LoginScreenModule::SlotShowOptionMenu );

    if ( account != NULL && !account->IsEmpty() )
    {
      m_GuiMenu->SetChildMember( "BtnRemoveAccount", "visible", "true" );
    }
  }
}
#endif

void LoginScreenModule::EnterPlayerName( uint32 nextAction )
{
  Account* account = Account::GetActiveAccount();
  if ( account == NULL || account->GetName().empty() )
  {
    // TODO Thorh : This has to be fixed properly
    HideMenu();
    m_GuiMenu = m_HUD->AddChild( new CoreGuiBase() );

    m_NextAction = nextAction;
    m_GameEngine.GetResourceManager()->AsyncLoadResource( new GuiResource( m_GuiMenu, "AccountScreen_PlayerName.xml" ), 
      Resource::RESOURCEFLAG_CACHE,
      this,
      &LoginScreenModule::SlotLoadedPlayerNameXml
      );
  }
}

void LoginScreenModule::SlotLoadedPlayerNameXml( Resource* /*resource*/ )
{
  ConnectButton( m_GuiMenu, "BtnCancel", &LoginScreenModule::SlotShowMainMenu );
  ConnectButton( m_GuiMenu, "BtnCreateName", &LoginScreenModule::SlotCreatePlayerName );
}

void LoginScreenModule::SlotCreatePlayerName( int /*btnId*/ )
{
  CoreGuiInputfield* textfield = (CoreGuiInputfield*) m_HUD->GetChildWithName("PlayerName");
  if ( textfield != NULL )
  {
    String playerName = textfield->GetText();
    if ( !playerName.empty() )
    {
      // Store the changes locally
      Account* account = Account::GetActiveAccount();
      if ( account == NULL )
      {
        account = new Account();
        Account::SetActiveAccount( account );
      }
      account->SetAccountId( AccountManager::s_FakeAccountId );
      account->SetName( playerName );
      account->SaveUserConfig();

      // Update the server if we can
      switch( m_NextAction )
      {
        case PLAY_SOLOGAME:
          SlotLoginAnonymously();
          break;
        case SHOW_MAINMENU:
          SlotShowMainMenu();
          break;
        case CREATE_ACCOUNT:
          CreateAccount(account);
          SlotShowMainMenu();
          break;
        default:
          break;
      }
    }  
  }
}

void LoginScreenModule::NetworkConnectionFailed()
{
  switch( m_NetworkMode )
  {
    case ACCOUNTMODE_AUTOLOGIN:
      //{
      //  HideDialog();
      //  Account* account = Account::GetActiveAccount();
      //  if ( account == NULL || account->GetLoginMethod() == Account::LOGIN_METHOD_DARKFACTOR )
      //  {
      //    ShowLoginAccountWindow();
      //  }
      //  else
      //  {
      //    SlotShowMainMenu();
      //  }
      //}
      break;

      // Ignore the server error if we are in the menu
    case ACCOUNTMODE_MENU:
      break;

    case ACCOUNTMODE_CREATE:
    case ACCOUNTMODE_LOGIN:
    case ACCOUNTMODE_DEVICELOGIN:
      OnServerError( 0, GameEnginePacketData::ErrorCode_ConnectionFailed );
      break;

    default:
      break;
  }
}

#ifdef CLIENT
void LoginScreenModule::OnServerError( uint32 methodId, uint32 errorCode )
{
  const String& errorMessage = NetworkBase::GetErrorString( errorCode );
  switch( m_NetworkMode )
  {
  case ACCOUNTMODE_DEVICECREATE:
  case ACCOUNTMODE_CREATE:
    {
      if ( methodId == 0 || methodId == GameLoginPacketData::PacketData_CreateClientAccount)
      {
        ShowDialog( "Create Account : ERROR", errorMessage, &LoginScreenModule::ShowCreateAccountWindow );
      }
    }
    break;

  case ACCOUNTMODE_LOGIN:
    {
      if ( methodId == 0 ||  methodId == GameLoginPacketData::PacketData_ClientLoggedOnAccount)
      {
        ShowDialog( "Login Account : ERROR", errorMessage, &LoginScreenModule::ShowLoginAccountWindow );
      }
    }
    break;

  case ACCOUNTMODE_AUTOLOGIN:
    {
      Account* account = Account::GetActiveAccount();
      if ( account == NULL || account->GetLoginMethod() == Account::LOGIN_METHOD_DARKFACTOR )
      {
        ShowLoginAccountWindow();
        ShowDialog( "Login Account : ERROR", errorMessage, &LoginScreenModule::ShowLoginAccountWindow );
      }
      else
      {
        SlotShowMainMenu();
        ShowDialog( "Login Account : ERROR", errorMessage, &LoginScreenModule::SlotShowMainMenu );
      }
    }
    break;

  case ACCOUNTMODE_DEVICELOGIN:
    {
      if ( errorCode == GameLoginPacketData::ErrorCode_YouMustEnterName )
      {
        HideDialog();

        if ( methodId == GameLoginPacketData::PacketData_CreateClientAccount)
        {
          EnterPlayerName( LoginScreenModule::CREATE_ACCOUNT );
        }
        else
        {
          EnterPlayerName( LoginScreenModule::SHOW_MAINMENU );
        }
      }
      else
      {
        ShowDialog( "ERROR", errorMessage, &LoginScreenModule::ShowLoginAccountWindow );
      }
    }
    break;

  default:
    break;
  }
}
#endif

/************************************************************************************************
 * InitiateAmazonLogin:
 * This will initiate the device/platform login if this is a new install. The idea is that on
 * iOS we will auto create the account with the iTunes account on the iPad. On Google Android we
 * will do the same with Google Play account.
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
#ifdef CLIENT
void LoginScreenModule::InitiateAmazonLogin()
{
  m_GameEngine.AddMessage( CoreGameEngine::MESSAGE_AMAZONLOGIN );
}
#endif

/************************************************************************************************
 * InitiatePlatformLogin:
 * This will initiate the device/platform login if this is a new install. The idea is that on
 * iOS we will auto create the account with the iTunes account on the iPad. On Google Android we
 * will do the same with Google Play account.
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
#ifdef CLIENT
void LoginScreenModule::InitiatePlatformLogin()
{
  m_GameEngine.AddMessage( CoreGameEngine::MESSAGE_PLATFORM_LOGIN );
}
#endif

/************************************************************************************************
 * SlotLoginDeviceAccount:
 * Callback function from Android/iOS when the external login process has the login data.
 * This is forwarded to the server for login/create the account if needed.
 *
 * @param  (String)   username         - The username to use
 * @param  (String)   password         - The password ( using different fields on each platform )
 * @param  (String)   name             - The name for the account
 * @param  (uint8)    loginMethod      - Enum with the login method
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
#ifdef CLIENT
void LoginScreenModule::SlotLoginDeviceAccount( String username, String password, String name, uint8 loginMethod )
{
  HideMenu();
  m_NetworkMode = ACCOUNTMODE_DEVICELOGIN;

  Account* account = new Account();
  account->SetUsername( username );
  account->SetPassword( password );
  account->SetName( name );
  account->SetLoginMethod( loginMethod );
  Account::SetActiveAccount( account );

  LogInfoFMT("LoginAccountScreenModule", "LoginDeviceAccount : '%s' (LoginMethod:%d)", username.c_str(), loginMethod );
  ShowDialog( "Connecting", "Logging in", &LoginScreenModule::SlotCancelLogin );
  LoginAccount(account);
}
#endif

#ifdef CLIENT
void LoginScreenModule::SlotLoginFailed( String errorMessage )
{
  if ( m_NetworkMode != ACCOUNTMODE_NONE )
  {
    ShowDialog( "Login", errorMessage, &LoginScreenModule::SlotCancelLogin );
  }
}
#endif

/************************************************************************************************
 * ShowLoginAccountWindow:
 * Show the login account window on the screen
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/

void LoginScreenModule::ChooseLoginOrCreateAccount()
{
  Account* account = Account::GetActiveAccount();
  if ( account == NULL || account->IsLoginEmpty() )
  {
    ShowCreateAccountWindow();
  }
  else
  {
    ShowLoginAccountWindow();
  }
}

#ifdef CLIENT
void LoginScreenModule::ShowLoginAccountWindow()
{
  HideMenu();

  // Merge the stars and be done with it
  Account* account = Account::GetActiveAccount();
  if ( account != NULL && account->GetAccountId() == AccountManager::s_FakeAccountId && !account->GetName().IsEmpty() )
  {
    ShowDialog( "Local Account", 
      String::FormatString("If you log in with a different|account, your local account|'%s'|will be merged with it.", account->GetName().c_str() ),
      CoreGuiButton::BUTTONGROUP_YESNO,
      &LoginScreenModule::SlotConfirmLoginAccount );
  }
  else
  {
    ShowLoginAccountWindowOk();
  }
}
#endif

void LoginScreenModule::SlotConfirmLoginAccount( int btnId )
{
  if ( btnId == CoreGuiButton::BUTTONTYPE_YES )
  {
    ShowLoginAccountWindowOk();
  }
  else
  {
    SlotShowMainMenu();
  }
}

void LoginScreenModule::ShowLoginAccountWindowOk()
{
  HideMenu();

  m_AccountMode = ACCOUNTMODE_LOGIN;
  m_GuiMenu = m_HUD->AddChild( new CoreGuiBase() );

  // Load the menu from XML
  m_GameEngine.GetResourceManager()->SyncLoadResource( new GuiResource( m_GuiMenu, "AccountScreen_LoginAccount.xml" ) );
  ConnectButton( m_GuiMenu, "BtnCancel",  &LoginScreenModule::SlotShowMainMenu );
  ConnectButton( m_GuiMenu, "BtnLoginAccount", &LoginScreenModule::SlotRequestLoginAccount );
  ConnectButton( m_GuiMenu, "BtnCreateAccount", &LoginScreenModule::ShowCreateAccountWindow );

  CoreGuiInputfield* guiUsername  = (CoreGuiInputfield*) m_GuiMenu->GetChildWithName("username");
  CoreGuiInputfield* guiPassword  = (CoreGuiInputfield*) m_GuiMenu->GetChildWithName("password");
  if ( guiUsername != NULL && guiPassword != NULL )
  {
    Account* account = Account::GetActiveAccount();
    if ( account != NULL && !account->IsEmpty() && account->GetLoginMethod() == Account::LOGIN_METHOD_DARKFACTOR )
    {
      guiUsername->SetText( account->GetUsername() );
      guiPassword->SetText( account->GetPassword() );
    }
    else
    {
      guiUsername->SetText( String::zero );
      guiPassword->SetText( String::zero );
    }

    guiUsername->m_ActionEvent.Connect( this, &LoginScreenModule::VerifyLoginData );
    guiPassword->m_ActionEvent.Connect( this, &LoginScreenModule::VerifyLoginData );
  }
  else
  {
    dfAssert( false, "Login Account :: Broken xml data!");
    ShowLoginAccountWindow();
  }
}


void LoginScreenModule::VerifyLoginData()
{
  CoreGuiInputfield* guiUsername  = (CoreGuiInputfield*) m_GuiMenu->GetChildWithName("username");
  CoreGuiInputfield* guiPassword  = (CoreGuiInputfield*) m_GuiMenu->GetChildWithName("password");
  if ( guiUsername != NULL && guiPassword != NULL )
  {
    if ( !guiUsername->GetText().empty() && !guiPassword->GetText().empty() )
    {
      SlotRequestLoginAccount();
    }
  }
}

/************************************************************************************************
 * SlotRequestLoginAccount:
 * Callback function that is called when the user clicks on LOGIN ACCOUNT button in the 
 * login account window. This will send the data to the server and request the server to login
 * the actual account
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
#ifdef CLIENT
void LoginScreenModule::SlotRequestLoginAccount()
{
  if ( m_GuiMenu != NULL )
  {
    CoreGuiInputfield* guiUsername  = (CoreGuiInputfield*) m_GuiMenu->GetChildWithName("username");
    CoreGuiInputfield* guiPassword  = (CoreGuiInputfield*) m_GuiMenu->GetChildWithName("password");

    if ( guiUsername == NULL || guiPassword == NULL )
    {
      dfAssert( false, "CreateAccount :: Broken xml data!");
      ShowLoginAccountWindow();
      return;
    }

    Account* account = Account::GetActiveAccount();
    if ( account == NULL )
    {
      account = new Account();
      Account::SetActiveAccount( account );
    }

    account->SetUsername( guiUsername->GetText() );
    account->SetPassword( guiPassword->GetText() );
    account->SetLoginMethod( Account::LOGIN_METHOD_DARKFACTOR );

    if ( guiUsername->GetText().empty() )
    {
      ShowLoginAccountWindow();
      ShowDialog( "Create Account : ERROR", "You must specify a username \n(e-mail address)", &LoginScreenModule::HideDialog );
      return;
    }

    if ( guiPassword->GetText().empty() )
    {
      ShowLoginAccountWindow();
      ShowDialog( "Create Account : ERROR", "You must specify a password", &LoginScreenModule::HideDialog );
      return;
    }

    m_NetworkMode = ACCOUNTMODE_LOGIN;

    // Request the account info on the server
    HideMenu();
    ShowDialog( "Connecting", "Logging in", &LoginScreenModule::SlotCancelLogin );
    LoginAccount(account);
  }
  else
  {
    ShowLoginAccountWindow();
  }
}
#endif

#ifdef CLIENT
void LoginScreenModule::LoginAccount( Account* account )
{
  CoreGameEngine* gameEngine = GetCoreGameEngine();
  if (gameEngine != NULL)
  {
    String gameKey = gameEngine->GetGameKey();
    SendPacketToServer(new LoginServerAccountNetworkPacket(BUILD_REVISION, gameKey, account));
    SendPacketToServer(new RequestServerVersionNetworkPacket());
  }
}
#endif

#ifdef CLIENT
void LoginScreenModule::CreateAccount(Account* account)
{
  CoreGameEngine* gameEngine = GetCoreGameEngine();
  if (gameEngine != NULL)
  {
    const String& gameKey = gameEngine->GetGameKey();
    SendPacketToServer(new CreateServerAccountNetworkPacket(BUILD_REVISION, gameKey, account));
    SendPacketToServer(new RequestServerVersionNetworkPacket());
  }
}
#endif

#ifdef CLIENT
void LoginScreenModule::SlotCancelLogin()
{
  NetworkClient* connection = safe_cast<NetworkClient*> (m_GameEngine.GetNetwork());
  if ( connection != NULL )
  {
    connection->Disconnect();
  }
  ShowLoginAccountWindow();
}
#endif

/************************************************************************************************
 * ShowCreateAccountWindow:
 * Show the create account window on the screen
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
void LoginScreenModule::ShowCreateAccountWindow()
{
  HideMenu();

  m_AccountMode = ACCOUNTMODE_CREATE;
  m_GuiMenu = m_HUD->AddChildWithHack( new CoreGuiBase(), CoreGuiBase::LAYER_NORMAL );

  m_GameEngine.GetResourceManager()->SyncLoadResource( new GuiResource( m_GuiMenu, "AccountScreen_CreateAccount.xml" ) );
  ConnectButton( m_GuiMenu, "BtnCancel",  &LoginScreenModule::SlotShowMainMenu );
  ConnectButton( m_GuiMenu, "BtnCreateAccount", &LoginScreenModule::SlotRequestCreateAccount );
  ConnectButton( m_GuiMenu, "BtnLoginAccount", &LoginScreenModule::ShowLoginAccountWindow );
 
  CoreGuiInputfield* guiNickname = (CoreGuiInputfield*) m_GuiMenu->GetChildWithName("nickname");
  CoreGuiInputfield* guiUsername = (CoreGuiInputfield*) m_GuiMenu->GetChildWithName("username");
  CoreGuiInputfield* guiPassword = (CoreGuiInputfield*) m_GuiMenu->GetChildWithName("password");
  CoreGuiInputfield* guiPassword2 = (CoreGuiInputfield*) m_GuiMenu->GetChildWithName("password2");
  if ( guiNickname != NULL && guiUsername != NULL && guiPassword != NULL )
  {
    Account* account = Account::GetActiveAccount();
    if ( account != NULL && ( account->IsLoginEmpty() || account->GetLoginMethod() == Account::LOGIN_METHOD_DARKFACTOR ) )
    {
      guiNickname->SetText( account->GetName() );
      guiUsername->SetText( account->GetUsername() );
      guiPassword->SetText( account->GetPassword() );
      if ( guiPassword2 != NULL )
      {
        guiPassword2->SetText( account->GetPassword() );
      }
    }
  }
  else
  {
    dfAssert( false, "Create Account :: Broken xml data!");
    SlotShowMainMenu();
  }
}

/************************************************************************************************
 * SlotRequestCreateAccount:
 * Callback function that is called when the user clicks on CREATE ACCOUNT button in the 
 * create account window. This will send the data to the server and request the server to create
 * the actual account
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
void LoginScreenModule::SlotRequestCreateAccount()
{
  if ( m_GuiMenu != NULL )
  {
    CoreGuiInputfield* guiNickname = (CoreGuiInputfield*) m_GuiMenu->GetChildWithName("nickname");
    CoreGuiInputfield* guiUsername = (CoreGuiInputfield*) m_GuiMenu->GetChildWithName("username");
    CoreGuiInputfield* guiPassword = (CoreGuiInputfield*) m_GuiMenu->GetChildWithName("password");
    CoreGuiInputfield* guiPassword2 = (CoreGuiInputfield*) m_GuiMenu->GetChildWithName("password2");

    if ( guiNickname == NULL || guiUsername == NULL || guiUsername == NULL )
    {
      dfAssert( false, "CreateAccount :: Broken xml data!");
      ShowCreateAccountWindow();
      return;
    }

    // Set the account data
    Account* account = Account::GetActiveAccount();
    if ( account == NULL )
    {
      account = new Account();
      Account::SetActiveAccount( account );
    }
    account->SetName( guiNickname->GetText() );
    account->SetUsername( guiUsername->GetText() );
    account->SetPassword( guiPassword->GetText() );
    account->SetLoginMethod( Account::LOGIN_METHOD_DARKFACTOR );

    // Random icon for now
    uint32 iconId = 0;//( rand() % 4 ) + GuiResource_IconWizard;
    account->SetIconId( iconId );

    if ( guiNickname->GetText().empty() )
    {
      ShowCreateAccountWindow();
      ShowDialog("Create Account : ERROR", "You must specify a nickname", &LoginScreenModule::HideDialog );
      return;
    }

    if ( guiUsername->GetText().empty() )
    {
      ShowCreateAccountWindow();
      ShowDialog("Create Account : ERROR", "You must specify a username", &LoginScreenModule::HideDialog );
      return;
    }

    if ( guiPassword->GetText().empty() )
    {
      ShowCreateAccountWindow();
      ShowDialog("Create Account : ERROR", "You must specify a password", &LoginScreenModule::HideDialog );
      return;
    }

    if ( guiPassword2 != NULL && !guiPassword2->GetText().EqualsWithCase( guiPassword->GetText() ) )
    {
      ShowCreateAccountWindow();
      ShowDialog("Create Account : ERROR", "Passwords must match", &LoginScreenModule::HideDialog );
      return;
    }

    m_NetworkMode = ACCOUNTMODE_CREATE;

    // Request the account info on the server
    HideMenu();
    ShowDialog("Connecting", "Creating account", &LoginScreenModule::SlotCancelLogin );
    CreateAccount(account);
  }
  else
  {
    ShowCreateAccountWindow();
  }
}

/************************************************************************************************
* GetErrorString:
* Returns the errorcode as a human readable string
*
* @param  (uint8)         errorCode - ID of the error
* @return (String)                  - The readable string
*
* @author Thor Richard Hansen
*************************************************************************************************/
const String LoginScreenModule::GetErrorString(uint32 errorCode)
{
  //switch ( errorCode )
  //{
  //  case ErrorCode_UserAlreadyExists:
  //    {
  //      static String userAlreadyExistError = "User already exists on the server";
  //      return userAlreadyExistError;
  //    }
  //  case ErrorCode_WrongPassword:
  //    {
  //      static String userAlreadyExistError = "Unknown user/password (2)";
  //      return userAlreadyExistError;
  //    }
  //  case ErrorCode_LoginNoSuchUser:
  //    {
  //      static String unknownUserError = "Unknown user/password (1)";
  //      return unknownUserError;
  //    }
  //  case ErrorCode_NameTooShort:
  //    {
  //      static String usernameTooShortError = "Nickname too short";
  //      return usernameTooShortError;
  //    }

  //  case ErrorCode_NameTooLong:
  //    {
  //      static String usernameTooLongError = "Nickname too long";
  //      return usernameTooLongError;
  //    }

  //  case ErrorCode_NameHasInvalidCharacters:
  //    {
  //      static String invalidCharacters = "Invalid characters in nickname!";
  //      return invalidCharacters;
  //    }

  //  case ErrorCode_UsernameTooShort:
  //    {
  //      static String usernameTooShortError = "Username too short";
  //      return usernameTooShortError;
  //    }
  //  case ErrorCode_UsernameTooLong:
  //    {
  //      static String usernameTooLongError = "Username too long";
  //      return usernameTooLongError;
  //    }

  //  case ErrorCode_UsernameHasInvalidCharacters:
  //    {
  //      static String invalidCharacters = "Invalid characters in username!";
  //      return invalidCharacters;
  //    }

  //  case ErrorCode_UsernameIsNotValidEmail:
  //    {
  //      static String invalidCharacters = "Username is not a valid e-mail";
  //      return invalidCharacters;
  //    }

  //  case ErrorCode_PasswordHasInvalidCharacters:
  //    {
  //      static String invalidCharacters = "Invalid characters in password!";
  //      return invalidCharacters;
  //    }

  //  case ErrorCode_BrokenAccountData:
  //    {
  //      static String brokenAccountDataError = "Unknown error. \nPlease contact support";
  //      return brokenAccountDataError;
  //    }

  //  case ErrorCode_CodeError:
  //    {
  //      static String brokenAccountDataError = "Broken Code!";
  //      return brokenAccountDataError;
  //    }

  //  case ErrorCode_RequiresGM:
  //    {
  //      static String brokenAccountDataError = "You must be GM to do this";
  //      return brokenAccountDataError;
  //    }

  //  default:
  //    //dfAssertFMT(false, "Unknown errocode %d", errorCode );
  //    return String::FormatString("CoreGameLoginScreen:|Unknown error (%d)", errorCode);
  //}
  return String::FormatString("CoreGameLoginScreen:|Unknown error (%d)", errorCode);
}
#endif /// CLIENT
