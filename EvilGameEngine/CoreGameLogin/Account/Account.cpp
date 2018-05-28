/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : Account
*
* Description : Holds all the information on one Account. The account is stored on the server and
*               is used to log into the game as a unique player.
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "Account.h"
#include "GMFlags.h"

#include "EvilEngine/CoreLib/BasicTypes/BinaryStream/BinaryStream.h"
#include "EvilEngine/CoreIO/UserConfig/UserConfig.h"
#include "EvilEngine/CoreLib/Utils/CoreUtils.h"

#include "EvilGameEngine/CoreBoardGame/GameLogic/XPTable/XPTable.h"

Account*  Account::s_Account = NULL;
Signal0< void > Account::s_SignalChangedAccount;
Signal0< void > Account::s_SignalReceivedAccountInfo;

#if defined SERVER
  String Account::s_GmPassword = "";
#else
  String Account::s_GmPassword = "starstruck";
#endif

#if defined DEBUG
const char* s_AccountConfigName = "Account_Debug";
#else
const char* s_AccountConfigName = "Account";
#endif

#define ACCOUNT_VERSION 6

Account::Account() :
  m_Id( 0 ),
  m_IconId( 0 ),
  m_Xp( 0 ),
  m_Level( 1 ),
  m_Platform( 0 ),
  m_LoginMethod( 0 ),
  m_ConnectionId( 0 ),
  m_AutoLogin( false ),
  m_PurchaseFlags( 0 ),
  m_IsAuthenticated( false ),
  m_IsInDatabase( false ),
  m_GmEnabled( false )
{
  m_LoadedTime = CoreUtils::GetTimestamp();
}

Account::Account( AccountID accountId, const String& name, const String& username, const String& passowrd, uint32 iconId ) :
  m_Id( accountId ),
  m_Name( name ),
  m_Username( username ),
  m_Password( passowrd ),
  m_IconId( iconId ),
  m_Xp(0),
  m_Level(1),
  m_Platform( 0 ),
  m_LoginMethod( 0 ),
  m_ConnectionId( 0 ),
  m_AutoLogin( false ),
  m_PurchaseFlags( 0 ),
  m_IsAuthenticated( false ),
  m_IsInDatabase( false ),
  m_GmEnabled(false)
{
  m_LoadedTime = CoreUtils::GetTimestamp();
}

Account::~Account()
{
}

uint8 Account::GetAccountVersion()
{
  return ACCOUNT_VERSION;
}

AccountID Account::GetClientAccountId()
{
  Account* account = GetClientAccount();
  if ( account != NULL )
  {
    return account->GetAccountId();
  }
  return 0;
}

bool Account::HasClientGMFlag()
{
  return ((m_PurchaseFlags & GM_FLAGS::CLIENTGMFLAG) == GM_FLAGS::CLIENTGMFLAG ) || HasServerGMFlag();
}
bool Account::HasServerGMFlag()
{
  return (m_PurchaseFlags & GM_FLAGS::GMFLAG) == GM_FLAGS::GMFLAG;
}

bool Account::IsClientGMEnabled()
{
  return HasClientGMFlag() && m_GmEnabled;
}
bool Account::IsServerGMEnabled()
{
  return HasServerGMFlag() && m_GmEnabled;
}
void Account::ToggleGM()
{
  m_GmEnabled = !m_GmEnabled;
  s_SignalChangedAccount();
}

/************************************************************************************************
* CreateFromStream:
* Creates a new instance of an Account from a BinaryStream.
*
* @param  (BinaryStream)   data  - The data blob that contains the data
* @return (Account)              - Return the Account 
*
* @author Thor Richard Hansen
*************************************************************************************************/
bool Account::CreateFromStream( const BinaryStream* data )
{
  if ( ReadData( data ) )
  {
    return true;
  }
  return false;
}

/************************************************************************************************
* IsEmpty:
* Return true if the all fields are empty. If these specific fields are empty it means that 
* the account does not contain valid/any data.
*
* @return (bool)                 - Returns true if the fields are empy
*
* @author Thor Richard Hansen
*************************************************************************************************/
bool Account::IsEmpty()
{
  return ( m_Name.empty() && m_Username.empty() && m_Password.empty() );
}

bool Account::IsLoginEmpty()
{
  return ( m_Username.empty() && m_Password.empty() );
}

/************************************************************************************************
* Reset:
* Reset/Clear all the login data for this account
*
* @author Thor Richard Hansen
*************************************************************************************************/
void Account::Reset()
{
  m_Name            = String::zero;
  m_Username        = String::zero;
  m_Password        = String::zero;
  m_LoginMethod     = 0;
  m_Token           = String::zero;
  m_Xp              = 0;
  m_Level           = 0;
  m_IconId          = 0;
  m_PurchaseFlags   = 0;
  m_IsAuthenticated = false;
}

/************************************************************************************************
* ReadData:
* Reads all the account data from a BinaryStream blob. The binary stream is used to send/receive
* data on the network.
*
* @param  (BinaryStream)   data  - The data blob that contains the data
* @return (bool)                 - Returns true if we didn't read outside of the blob
*
* @author Thor Richard Hansen
*************************************************************************************************/
bool Account::ReadData( const BinaryStream* data )
{
  uint8 version   = data->ReadUInt8();
  if ( version > ACCOUNT_VERSION )
  {
    LogErrorFMT("Account","Wrong version! (%d)", ACCOUNT_VERSION);
    return false;
  }

  m_Id            = data->ReadUInt32();
  m_Name          = data->ReadString();
  m_Username      = data->ReadString();
  m_Password      = data->ReadString();
  m_Platform      = data->ReadUInt8();
  m_LoginMethod   = data->ReadUInt8();
  m_Token         = data->ReadString();
  m_IconId        = data->ReadUInt32();
  m_PurchaseFlags = data->ReadUInt32();
  m_Xp            = data->ReadUInt32();
  m_Level         = data->ReadUInt8();

#if defined CLIENT
  m_Platform      = CoreUtils::GetCurrentPlatform();
#endif

  return data->IsOK();
}

/************************************************************************************************
* WriteData:
* Writes all the account data to a BinaryStream blob. The binary stream is used to send/receive
* data on the network.
*
* @param  (BinaryStream)   data  - The data blob to fill the data into
* @return (bool)                 - Returns true if we didn't write outside of the blob
*
* @author Thor Richard Hansen
*************************************************************************************************/
bool Account::WriteData( BinaryStream* data ) const
{
  data->WriteUInt8( ACCOUNT_VERSION );
  data->WriteUInt32( m_Id );
  data->WriteString( m_Name );
  data->WriteString( m_Username );
  data->WriteString( m_Password );
  data->WriteUInt8( m_Platform );
  data->WriteUInt8( m_LoginMethod );
  data->WriteString( m_Token );
  data->WriteUInt32( m_IconId );
  data->WriteUInt32( m_PurchaseFlags );
  data->WriteUInt32(m_Xp);
  data->WriteUInt8(m_Level);

  return data->IsOK();
}

/************************************************************************************************
* SetActiveAccount:
* Sets the specified account as the active one.
*
* @param  (Account)     account  - The account to set as active
*
* @author Thor Richard Hansen
*************************************************************************************************/
void Account::SetActiveAccount( Account* account )
{
  if ( s_Account != NULL && s_Account != account )
  {
    delete s_Account;
  }
  s_Account = account;

  s_SignalChangedAccount();
}

/************************************************************************************************
* Copy:
* Copy all the data from one account to this one
*
* @param  (Account)     account  - The account to copy from
*
* @author Thor Richard Hansen
*************************************************************************************************/
void Account::Copy( Account* account )
{
  m_Id            = account->m_Id;
  m_Name          = account->m_Name;
  m_Username      = account->m_Username;
  m_Password      = account->m_Password;
  m_IconId        = account->m_IconId;
  m_PurchaseFlags = account->m_PurchaseFlags;
  m_LoginMethod   = account->m_LoginMethod;
  m_Xp            = account->m_Xp;
  m_Level         = account->m_Level;
  m_IsInDatabase  = account->m_IsInDatabase;

  if ( m_Platform == 0 )
  {
#if defined CLIENT
    m_Platform      = CoreUtils::GetCurrentPlatform();
#else
    m_Platform      = account->m_Platform;
#endif
  }
}

void Account::RemoveUserConfig()
{
#if defined CLIENT
  if ( UserConfig::Remove(s_AccountConfigName) )
  {
    Account::SetActiveAccount( NULL );
  }
#endif
}

/************************************************************************************************
* LoadFromUserConfig:
* Get all the account information from the userconfig ( locally stored on the device )
*
* @return  (Account)    - The account with the data in the config
*
* @author Thor Richard Hansen
*************************************************************************************************/
Account* Account::LoadFromUserConfig()
{
#if defined CLIENT
  UserConfig* config = new UserConfig(s_AccountConfigName);
  if ( config->Load() )
  {
    LoadFromUserConfig( config );
    return this;
  }
#endif
  return NULL;
}

Account* Account::LoadFromUserConfig( UserConfig* config )
{
  m_Id            = config->GetUInt32("m_Id");
  m_Name          = config->GetString("m_Name");
  m_Username      = config->GetString("m_Username");
  m_Password      = config->GetString("m_Password");
  m_LoginMethod   = (uint8)config->GetUInt32("m_LoginMethod");
  m_Platform      = CoreUtils::GetCurrentPlatform();
  m_PurchaseFlags = config->GetUInt32("m_PurchaseFlags");
  m_AutoLogin     = config->GetBool("m_Autologin");
  return this;
}

/************************************************************************************************
* SaveUserConfig:
* Save all the accountinfo to the config ( locally stored on the device )
*
* @return  (bool)    - Return true if the config was saved correctly
*
* @author Thor Richard Hansen
*************************************************************************************************/
bool Account::SaveUserConfig()
{
  UserConfig* config = new UserConfig(s_AccountConfigName);
  SaveUserConfig( config );
  return config->Save();
}

bool Account::SaveUserConfig( UserConfig* config )
{
  config->SetUInt32( "m_Id",            m_Id );
  config->SetString( "m_Name",          m_Name );
  config->SetString( "m_Username",      m_Username );
  config->SetString( "m_Password",      m_Password );
  config->SetUInt32( "m_LoginMethod",   m_LoginMethod );
  config->SetUInt32( "m_PurchaseFlags", m_PurchaseFlags );
  config->SetBool(   "m_Autologin",     m_AutoLogin );
  return true;
}

void Account::SetConnectionId( uint32 connectionID )
{
  dfAssert( !(m_ConnectionId != 0 && connectionID == 0 ), "Account:Trying to reset connectionId" );
  m_ConnectionId = connectionID;
}

void Account::ResetConnectionId()
{
  m_ConnectionId = 0;
}

/************************************************************************************************
* HasLoginData:
* Checks if the account has a username and password
*
* @return (bool)     - Return true if the account has valid data
*
* @author Thor Richard Hansen
*************************************************************************************************/
bool Account::HasLoginData()
{
  return ( !m_Username.empty() && !m_Password.empty() );
}

String Account::GetDebugInfo() const
{
  return String::FormatString("< %d:%s ip:%s >", m_Id, m_Username.c_str(), m_IpAddress.c_str() );
}

void Account::AddPurchase( uint32 flag )
{
  m_PurchaseFlags |= flag;
}

bool Account::HasPurchased( uint32 flag )
{
  //Account* account = GetActiveAccount();
  //if ( account != NULL )
  //{
  //  return ( account->GetPurchaseFlags() & flag ) == flag;
  //}
  //return false;

  return ( m_PurchaseFlags & flag ) == flag;
}

bool Account::IsMember()
{
  return (m_PurchaseFlags & MEMBERSHIP) == MEMBERSHIP;
  //return false;
}

bool Account::EnableGM( const String password )
{
  if ( !s_GmPassword.empty() && s_GmPassword.EqualsWithCase( password ) )
  {
    AddPurchase( GM_FLAGS::CLIENTGMFLAG | GM_FLAGS::GMFLAG );
    return true;
  }
  return false;
}

bool Account::IsAutoCreated() const
{
  switch( m_LoginMethod )
  {
    case LOGIN_METHOD_GOOGLEPLAY:
    case LOGIN_METHOD_IOSGAMECENTERCREATE:
    case LOGIN_METHOD_IOSGAMECENTERLOGIN:
    case LOGIN_METHOD_ANDROIDUUID:
      return true;
  }
  return false;
}

void Account::RemoveScore( uint32 /*score*/ )
{
/*
  if ( m_Xp > score )
  {
    m_Xp -= score;
  }
  else
  {
    m_Xp = 0;
  }
*/
}

void Account::AddScore(uint32 /*score*/)
{
/*
  uint8 newLevel = XPTable::GetInstance()->GetLevel(m_Xp + score);
  if (newLevel == 0)
  {
    newLevel = XPTable::GetInstance()->GetMaxLevel();
    if (newLevel == 0)
    {
      return;
    }
    m_Xp = XPTable::GetInstance()->GetMaxXp();
  }
  else
  {
    m_Xp += score;
  }

  if (m_Level != newLevel)
  {
    m_Level = newLevel;
    SignalNewLevel(newLevel);
  }
*/
}
