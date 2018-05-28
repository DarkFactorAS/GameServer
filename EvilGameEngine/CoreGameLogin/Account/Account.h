#ifndef EVILENGINE_COREGAMELOGIN_ACCOUNT_ACCOUNT
#define EVILENGINE_COREGAMELOGIN_ACCOUNT_ACCOUNT 1
#pragma once

#include "EvilEngine/CoreLib/Signal/Signal.h"

class BinaryStream;
class Player;
class UserConfig;

typedef uint32 AccountID;

enum PURCHASE_FLAGS
{
  NONE = 0,
  MEMBERSHIP = 1 << 2,
  PLAYFIELD1 = 1 << 3,
};

class Account
{
public:

  enum LOGIN_METHOD
  {
    LOGIN_METHOD_NONE                 = 0,
    LOGIN_METHOD_DARKFACTOR           = 1,
    LOGIN_METHOD_WINDOWS              = 2,
    LOGIN_METHOD_AMAZON               = 3,
    LOGIN_METHOD_GOOGLEPLAY           = 4,
    LOGIN_METHOD_IOSGAMECENTERCREATE  = 5,
    LOGIN_METHOD_ANDROIDUUID          = 6,
    LOGIN_METHOD_IOSGAMECENTERLOGIN   = 7,    // Just log in with the game center data. Cannot create account with this info
  };

  Account();
  Account(AccountID accountId, const String& name, const String& username, const String& password, uint32 iconId);
  virtual ~Account();

  static uint8              GetAccountVersion();
  static void               SetActiveAccount( Account* account );
  static Account*           GetActiveAccount(){ return s_Account;}
  static Account*           GetClientAccount(){ return s_Account;}
  static AccountID          GetClientAccountId();

  virtual bool              ReadData( const BinaryStream* data );
  virtual bool              WriteData( BinaryStream* data ) const;
  virtual void              Reset();
  bool                      IsEmpty();
  bool                      IsLoginEmpty();

  virtual void              Copy( Account* otherAccount );
  bool                      CreateFromStream( const BinaryStream* data );

  bool                      HasClientGMFlag();
  bool                      HasServerGMFlag();
  bool                      IsClientGMEnabled();
  bool                      IsServerGMEnabled();
  void                      ToggleGM();
  bool                      EnableGM(const String password);
  bool                      IsGMEnabled() const { return m_GmEnabled; }

  AccountID                 GetAccountId() const { return m_Id; }
  String                    GetName() const { return m_Name; }
  String                    GetToken() const{ return String::zero; }
  String                    GetUsername() const { return m_Username; }
  String                    GetPassword() const { return m_Password; }
  uint32                    GetIconId() const{ return m_IconId; }
  uint8                     GetPlatform() const{ return m_Platform; }
  uint8                     GetLoginMethod() const { return m_LoginMethod; }
  bool                      IsAutoCreated() const;

  void                      SetScore(uint32 score) { m_Xp = score; }
  uint32                    GetScore() const { return m_Xp; }
  void                      RemoveScore(uint32 score);
  virtual void              AddScore( uint32 score );
  uint8                     GetLevel() { return m_Level; }
  void                      SetLevel(uint8 level) { m_Level = level; }
  Signal1< void, uint32 >   SignalNewLevel;


  uint32                    GetConnectionId() const { return m_ConnectionId; }
  bool                      IsAuthenticated() const{ return m_IsAuthenticated; }
  void                      SetAuthenticated( bool isAutenticated ){ m_IsAuthenticated = isAutenticated; }
  bool                      AutoLogin() const { return m_AutoLogin; }

  static void               RemoveUserConfig();
  virtual Account*          LoadFromUserConfig();
  virtual bool              SaveUserConfig();

  void                      SetAccountId( AccountID accountId ){ m_Id = accountId; }
  void                      SetName( const String& name ){ m_Name = name; }
  void                      SetUsername( const String& username ){ m_Username = username; }
  void                      SetPassword( const String& password ){ m_Password = password; }
  void                      SetConnectionId( uint32 connectionID );
  void                      ResetConnectionId();
  void                      SetIconId( uint32 iconId ){ m_IconId = iconId; }
  void                      SetAutoLogin( bool autologin ){ m_AutoLogin = autologin; }
  void                      SetPlatform( uint8 platform ){ m_Platform = platform; }
  void                      SetLoginMethod( uint8 loginMethod ){ m_LoginMethod = loginMethod; }

  bool                      HasLoginData();
  void                      SetIpAddress( const String& address ){ m_IpAddress = address; }
  String                    GetDebugInfo() const;
  uint32                    GetLoadedTime() const { return m_LoadedTime; }

  void                      AddPurchaseFlags( uint32 flags ){ m_PurchaseFlags |= flags; s_SignalChangedAccount(); }
  void                      SetPurchaseFlags( uint32 flags ){ m_PurchaseFlags = flags; s_SignalChangedAccount(); }
  void                      AddPurchase( uint32 flag );
  bool                      HasPurchased( uint32 flag );
  uint32                    GetPurchaseFlags(){ return m_PurchaseFlags; }

  virtual bool              IsMember();

  static Signal0< void >    s_SignalChangedAccount;
  static Signal0< void >    s_SignalReceivedAccountInfo;

  bool                      IsInDatabase() const{ return m_IsInDatabase; }
  void                      SetIsInDatabase( bool isInDatabase ){ m_IsInDatabase = isInDatabase; }

  static void               SetGMPassword( const String& password ){ s_GmPassword = password; }
  static String             GetGMPassword(){ return s_GmPassword; }

protected:

  virtual Account*          LoadFromUserConfig( UserConfig* config );
  virtual bool              SaveUserConfig( UserConfig* config );


  static Account*           s_Account;
//private:

  AccountID                 m_Id;         // Unique ID for the account
  String                    m_Name;       // Displayname that other player see
  String                    m_Username;   // Secret username to log in with
  String                    m_Password;   // Secret password to log in with
  String                    m_IpAddress;  // Used on the server to track IP address

  uint32                    m_IconId;
  uint32                    m_Xp;
  uint8                     m_Level;
  uint8                     m_Platform;
  uint8                     m_LoginMethod;
  String                    m_Token;
  uint32                    m_ConnectionId;
  bool                      m_AutoLogin;
  uint32                    m_PurchaseFlags;
  bool                      m_IsAuthenticated;
  uint32                    m_LoadedTime;
  bool                      m_IsInDatabase;
  bool                      m_GmEnabled;

  static String             s_GmPassword;
};

#endif /// EVILENGINE_COREGAMELOGIN_ACCOUNT_ACCOUNT
