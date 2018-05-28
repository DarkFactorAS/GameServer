#ifndef EVILENGINE_COREGAMELOGIN_LOGINSCREEN_COREGAMELOGINSCREEN
#define EVILENGINE_COREGAMELOGIN_LOGINSCREEN_COREGAMELOGINSCREEN 1
#pragma once

#include "EvilEngine/CoreGuiToolkit/GuiElements/CoreGuiView.h"
#include "EvilEngine/CoreGuiToolkit/GuiElements/CoreGuiDialog.h"
#include "EvilEngine/CoreEngine/CoreInput/Keyboard.h"
#include "EvilEngine/CoreGuiToolkit/GuiTemplates/GuiButtonTemplate.h"
#include "EvilGameEngine/CoreGameEngine/CoreScreenModule/CoreScreenModule.h"

class CoreBaseObject;
class AccountManager;
class Resource;
class Account;

namespace EvilEngine
{
  extern Signal4< void, String, String, String, uint8 >  SignalLoginAccount;
  extern Signal1< void, String >                         SignalLoginFailed;
}

class LoginScreenModule : public CoreScreenModule
{
public:
  LoginScreenModule( int screenId, CoreGameEngine& parent );
  ~LoginScreenModule();

#if defined CLIENT
  void                      LoginAccount(Account* account);
  void                      CreateAccount(Account* account);
  void                      ShowAccountInfo();
#endif

protected:

  enum ACCOUNT_MODE
  {
    ACCOUNTMODE_NONE,
    ACCOUNTMODE_MENU,
    ACCOUNTMODE_CREATE,
    ACCOUNTMODE_LOGIN,
    ACCOUNTMODE_AUTOLOGIN,
    ACCOUNTMODE_DEVICELOGIN,
    ACCOUNTMODE_DEVICECREATE,
  };

  enum NEXT_ACTION
  {
    PLAY_SOLOGAME,
    SHOW_MAINMENU,
    CREATE_ACCOUNT,
  };

#ifdef CLIENT

  void                      InitAccountManager();
  
  virtual void              SlotCloseErrorDialog();
  virtual void              SlotShowCloseGameDialog();
  virtual void              SlotShowOptionMenu( int btnId );
  void                      SlotHandleOptionMenu( int btnId );
  void                      SlotCloseGameDialog( int btnId );

  void                      NetworkConnectionFailed();
  virtual void              OnServerError( uint32 methodId, uint32 loginError );
  static const String       GetErrorString(uint32 errorCode);

  void                      HideMenu();
  void                      ShowMenu();
  virtual void              SlotShowMainMenu();
  void                      EnterPlayerName( uint32 nextAction );
  void                      SlotLoadedPlayerNameXml( Resource* /*resource*/ );
  void                      SlotCreatePlayerName( int btnId );

  virtual void              SlotLoginAnonymously(){};
  virtual void              SlotPlayOnlineGame(){};
  virtual void              SlotLogoutAccount(){};
  virtual void              OnChangedServers(){};

  void                      InitiateAmazonLogin();
  void                      InitiatePlatformLogin();
  virtual void              OnReceivedAccountInfo(){};
  virtual void              ServerDisconnected(){};

  void                      SlotLoginDeviceAccount( String username, String password, String name, uint8 loginMethod );
  void                      ChooseLoginOrCreateAccount();

  void                      ShowLoginAccountWindow();
  void                      SlotConfirmLoginAccount( int btnId );
  void                      ShowLoginAccountWindowOk();
  void                      SlotRequestLoginAccount();
  void                      ShowCreateAccountWindow();
  void                      SlotRequestCreateAccount();

  void                      SlotLoginFailed( String errorMessage );

  void                      SlotCancelLogin();
  void                      VerifyLoginData();
#endif  /// CLIENT

  ACCOUNT_MODE              m_AccountMode;
  ACCOUNT_MODE              m_NetworkMode;
  CoreGuiDialog*            m_DialogWindow;
  CoreGuiBase*              m_GuiMenu;
  uint32                    m_NextAction;

  CoreGuiView*              m_AccountInfoView;
};

#endif /// EVILENGINE_COREGAMELOGIN_LOGINSCREEN_COREGAMELOGINSCREEN