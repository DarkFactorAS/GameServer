
#include "Precompile.h"

#include "EvilEngine/CoreDatabase/SQLResult.h"
#include "EvilEngine/CoreDatabase/CoreDatabase.h"
#include "EvilEngine/CoreEngine/CoreInput/Keyboard.h"
#include "EvilEngine/CoreLib/BasicTypes/BinaryStream/BinaryStream.h"
#include "EvilEngine/CoreIO/Logger/Logger.h"
#include "EvilEngine/CoreLib/BasicTypes/String/StringList.h"
#include "EvilEngine/CoreNetwork/SSLNetworkClient/SSLNetworkClient.h"
#include "EvilEngine/CoreEngine/CoreResourceManager/CoreResourceManager.h"
#include "EvilEngine/CoreEngine/CoreResourceManager/Resource.h"
#include "EvilEngine/CoreLib/Utils/CoreUtils.h"
#include "EvilEngine/CoreLib/Utils/Base64Util.h"

#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/GameEnginePacketData.h"
#include "EvilGameEngine/CoreGameLogin/Network/GameLoginPacketData.h"

#include "AccountManager.h"
#include "Account.h"
#include "GMFlags.h"

#define ACCOUNT_CACHE_TIME 600
#define PLAYER_START_ACCOUNT 1000

uint32          AccountManager::s_AccountId = PLAYER_START_ACCOUNT;
uint32          AccountManager::s_FakeAccountId = 1;

AccountManager::AccountManager()
{
}

AccountManager* AccountManager::GetInstance()
{
  static AccountManager* s_Instance = NULL;
  if ( s_Instance == NULL )
  {
    s_Instance = new AccountManager();
  }
  return s_Instance;
}

/************************************************************************************************
* LoadActiveAccount:
* Return the active account from disk if needed
*
* @return (Account)              - Return the Account 
*
* @author Thor Richard Hansen
*************************************************************************************************/
Account* AccountManager::LoadActiveAccount()
{
  Account* account = Account::GetActiveAccount();
  if ( account == NULL )
  {
    account = new Account();
    account->LoadFromUserConfig();
    Account::SetActiveAccount( account );
  }
  return account;
}

Account* AccountManager::GetCachedAccount( AccountID accountId )
{
  if (accountId == 0)
  {
    dfAssert( accountId != 0, "Trying to resolve account with ID == 0 !" );
    return NULL;
  }

  std::map< uint32, Account* >::iterator itAccount = m_AccountList.find(accountId);
  if (itAccount != m_AccountList.end())
  {
    Account* account = itAccount->second;

    // 10 minute timeout on the cache
    //uint32 nowTime = CoreUtils::GetTimestamp();
    //if ( account->GetLoadedTime() < nowTime - ACCOUNT_CACHE_TIME )
    //{
    //  LogInfoFMT("AccountManager","GetCachedAccount::Removed account from cache '%s'", account->GetUsername().c_str() );
    //  m_AccountList.erase( itAccount );
    //  delete account;
    //  return NULL;
    //}

    return account;
  }

  return NULL;
}

bool AccountManager::AddAccountToCache( Account* account )
{
  dfAssert( account->GetAccountId() != 0, "Trying to resolve account with ID == 0 !" );

  Account* existingAccount = GetCachedAccount( account->GetAccountId() );
  if ( existingAccount != account )
  {
    if ( existingAccount != NULL && account != NULL && existingAccount->GetConnectionId() != 0 && account->GetConnectionId() != existingAccount->GetConnectionId() )
    {
      // TODO : Disconnect this account

      //m_RemoveAccounts.push_back( existingAccount );
      //dfAssert( existingAccount->GetConnectionId() == 0 || account->GetConnectionId() != 0, "Trying to remove the connectionId on account!" );
    }
    delete existingAccount;
    m_AccountList[ account->GetAccountId() ] = account;
  }
  return true;
}

bool AccountManager::RemoveAccountFromCache(Account* account)
{
  if (account == NULL)
  {
    return false;
  }

  std::map< uint32, Account* >::iterator itAccount = m_AccountList.find(account->GetAccountId());
  if (itAccount != m_AccountList.end())
  {
    m_AccountList.erase(itAccount);
    return true;
  }
  return false;
}

Account* AccountManager::GetOfflineAccount( const String& username, int loginMethod )
{
  for ( std::map< uint32, Account* >::iterator itAccount = m_AccountList.begin(); itAccount != m_AccountList.end(); ++itAccount )
  {
    Account* account = itAccount->second;
    if ( account->GetUsername().EqualsWithCase( username ) && account->GetLoginMethod() == loginMethod )
    {
      // 10 minute timeout on the cache
      uint32 nowTime = CoreUtils::GetTimestamp();
      if (account->GetLoadedTime() < nowTime - 600)
      {
        m_AccountList.erase(itAccount);
        delete account;
        return NULL;
      }

      return account;
    }
  }
  return NULL;
}

#ifdef SERVER
Account* AccountManager::GetOnlineAccountFromNetwork(uint32 connectionId)
{
  std::map< uint32, uint32 >::const_iterator itFound = m_OnlineAccounts.find( connectionId );
  if ( itFound != m_OnlineAccounts.end() )
  {
    return GetCachedAccount( itFound->second );
  }
  //dfBugreportFMT( false, "GetOnlineAccountFromNetwork: Connection[%d] has no online character! (Not authorized?)", connectionId);
  return NULL;
}
#endif

#ifdef SERVER

uint32 AccountManager::GetOnlineConnectionIdFromAccountId(AccountID accountId)
{
  for ( std::map< uint32, uint32 >::const_iterator itOnlineAccount = m_OnlineAccounts.begin(); itOnlineAccount != m_OnlineAccounts.end(); itOnlineAccount++)
  {
    if ( itOnlineAccount->second == accountId )
    {
      return itOnlineAccount->first;
    }
  }
  //dfBugreportFMT(false, "GetOnlineAccountFromNetwork: Connection[%d] has no online character! (Not authorized?)", connectionId);
  return 0;
}

#endif


bool AccountManager::AddOnlineAccount(Account* account, uint32 connectionId)
{
  if ( account == NULL || account->GetAccountId() == 0 || connectionId == 0 )
  {
    dfBugreportFMT( false, "AddOnlineAccount:: AccoundId or ConnectionId[%d] is 0!", connectionId );
    return false;
  }

  // Make sure the connectionId is not mapped to a different account
  std::map< uint32, uint32 >::const_iterator itFound = m_OnlineAccounts.find(connectionId);
  if (itFound != m_OnlineAccounts.end())
  {
    if (itFound->second != account->GetAccountId())
    {
      dfBugreportFMT(itFound->second == account->GetAccountId(), "AddOnlineAccount:: Cannot add online connection with accountId[%d] since connection [%d] already exist with accountId[%d]",
        account->GetAccountId(), connectionId, itFound->second);
      return false;
    }
  }

  account->SetConnectionId( connectionId );
  m_OnlineAccounts[ connectionId ] = account->GetAccountId();
  return true;
}

Account* AccountManager::RemoveOnlineAccount(uint32 connectionId)
{
  std::map< uint32, uint32 >::iterator itFound = m_OnlineAccounts.find(connectionId);
  if (itFound != m_OnlineAccounts.end())
  {
    AccountID accountId = itFound->second;
    Account* account = GetCachedAccount( accountId );
    if ( account != NULL )
    {
      account->ResetConnectionId();
    }

    LogInfoFMT("AccountManager","Account %d with connectionId %d went offline.", accountId, connectionId );
    m_OnlineAccounts.erase( itFound );
    return account;
  }
  return NULL;
}

bool AccountManager::HasValidCharacters( const String& validateString, uint32 validator )
{
  for ( uint32 index = 0; index < validateString.length(); index++ )
  {
    if ( !Keyboard::IsValidCharacter( validateString.CharacterAt( index ), validator ) )
    {
      return false;
    }
  }
  return true;
}

int AccountManager::VerifyAccountData(Account* account)
{
  if (account == NULL)
  {
    return GameLoginPacketData::ErrorCode_BrokenAccountData;
  }

  switch (account->GetLoginMethod())
  {
    case Account::LOGIN_METHOD_ANDROIDUUID:
    {
      if (account->GetUsername().empty())
      {
        return GameLoginPacketData::ErrorCode_UsernameHasInvalidCharacters;
      }
      else if (account->GetName().empty())
      {
        return GameLoginPacketData::ErrorCode_YouMustEnterName;
      }
    }
    break;

  case Account::LOGIN_METHOD_IOSGAMECENTERCREATE:
    {
      if (account->GetUsername().empty())
      {
        return GameLoginPacketData::ErrorCode_UsernameHasInvalidCharacters;
      }
      else if (account->GetName().empty())
      {
        return GameLoginPacketData::ErrorCode_NameTooShort;
      }
    }
    break;

  // Not allowed to create a game center account without the verification data
  case Account::LOGIN_METHOD_IOSGAMECENTERLOGIN:
    return GameLoginPacketData::ErrorCode_YouMustLogInToGameCenter;

  default:
    {
      int error = IsValidName(account->GetName());
      if (error == GameEnginePacketData::ErrorCode_OK)
      {
        error = IsValidUsername(account->GetUsername());
      }
      if (error == GameEnginePacketData::ErrorCode_OK)
      {
        error = IsValidPassword(account->GetPassword());
      }
      if (error != GameEnginePacketData::ErrorCode_OK)
      {
        return error;
      }
    }
    break;
  }
  return 0;
}

/***********************************************************************************
* IsValidEmailFormat:
* Returns true if the given string is in a valid e-mail format. Format is
* [a-z A-Z 0-9] @ [a-z A-Z 0-9] . [a-z A-Z 0-9]
* Which means that we are not allowed to have two @ symbols. We must have a non 
* @ or dot character before the @. One valid character after the @ sign and a
* dot ( period ) character after that again.
* 
* @param  (String)     email - The string to check if it is a valid e-mail
* @return (bool)             - Return true if this string is a valid e-mail
* 
* @author Thor Richard Hansen
***********************************************************************************/
bool AccountManager::IsValidEmailFormat( const String& email )
{
  bool hasCharbeforeAt  = false;
  bool hasAt            = false;
  bool hasCharAfterAt   = false;
  bool hasDotAfterAt    = false;
  bool hasCharAfterDot  = false;

  for ( uint32 index = 0; index < email.length(); index++ )
  {
    char c = email.CharacterAt( index );
    if ( !Keyboard::IsValidCharacter( c, Keyboard::ALLOW_09 | Keyboard::ALLOW_AZ | Keyboard::ALLOW_EMAIL ) )
    {
      return false;
    }

    if ( c == Keyboard::KEY_AT && hasCharbeforeAt && !hasAt )
    {
      hasAt = true;
    }
    else if ( c == Keyboard::KEY_AT && hasAt )
    {
      return false;
    }
    else if ( c != Keyboard::KEY_AT && c != Keyboard::KEY_PERIOD && !hasCharbeforeAt )
    {
      hasCharbeforeAt = true;
    }
    else if ( c != Keyboard::KEY_AT && c != Keyboard::KEY_PERIOD && hasAt && !hasCharAfterAt )
    {
      hasCharAfterAt = true;
    }
    else if ( c == Keyboard::KEY_PERIOD && hasCharAfterAt && !hasDotAfterAt )
    {
      hasDotAfterAt = true;
    }
    else if ( c != Keyboard::KEY_AT && c != Keyboard::KEY_PERIOD && hasDotAfterAt && !hasCharAfterDot )
    {
      hasCharAfterDot = true;
    }
  }

  return ( hasCharbeforeAt && hasAt && hasCharAfterAt && hasDotAfterAt && hasCharAfterDot );
}

/***********************************************************************************
* IsValidName:
* Returns the errorcode if the given name is not valid ( not to short/long and only
* has valid characters in it ).
* 
* @param  (String)     name  - The string to check if it is a valid name
* @return (int)              - OK or the errorcode
* 
* @author Thor Richard Hansen
***********************************************************************************/
int AccountManager::IsValidName( const String& name )
{
  if ( name.length() < 4 )
  {
    return GameLoginPacketData::ErrorCode_NameTooShort;
  }
  else if ( name.length() > 16 )
  {
    return GameLoginPacketData::ErrorCode_NameTooLong;
  }
  else if ( !HasValidCharacters( name, Keyboard::ALLOW_09 | Keyboard::ALLOW_AZ ) )
  {
    return GameLoginPacketData::ErrorCode_NameHasInvalidCharacters;
  }
  return GameEnginePacketData::ErrorCode_OK;
}

int AccountManager::IsValidUsername( const String& username )
{
  if ( username.length() < 4 )
  {
    return GameLoginPacketData::ErrorCode_UsernameTooShort;
  }
  else if ( username.length() > 128 )
  {
    return GameLoginPacketData::ErrorCode_UsernameTooLong;
  }
  else if ( !HasValidCharacters( username, Keyboard::ALLOW_09 | Keyboard::ALLOW_AZ | Keyboard::ALLOW_EMAIL ) )
  {
    return GameLoginPacketData::ErrorCode_UsernameHasInvalidCharacters;
  }
  //else if ( !IsValidEmailFormat( username ) )
  //{
  //  return CoreGameNetworkCommon::ErrorCode_UsernameIsNotValidEmail;
  //}
  return GameEnginePacketData::ErrorCode_OK;
}

int AccountManager::IsValidPassword( const String& password )
{
  if (password.length() < 4)
  {
    return GameLoginPacketData::ErrorCode_PasswordTooShort;
  }
  else if (password.length() > 128)
  {
    return GameLoginPacketData::ErrorCode_PasswordTooLong;
  }
  else if ( !HasValidCharacters( password, Keyboard::ALLOW_09 | Keyboard::ALLOW_AZ | Keyboard::ALLOW_EMAIL ) )
  {
    return GameLoginPacketData::ErrorCode_PasswordHasInvalidCharacters;
  }
  return GameEnginePacketData::ErrorCode_OK;
}

/************************************************************************************************
* VerifyGameCenterAccount:
* Will verify Game Center login data against a certificate given by Apple. The data is packed
* in the username/password field and is unpacked here. The result is decrypted with openSSL
* using the gc-sb.cer public key.
*
* @param  (Account)   account  - Account with the data we want to verify
* @return (int)                - Returns the errorcode (or ok) if the verification failed/succeeded
*
* @author Thor Richard Hansen
*************************************************************************************************/
bool AccountManager::VerifyGameCenterAccount( Account* account )
{
#ifdef SERVER
  String username   = account->GetUsername();
  String signature  = account->GetPassword();

  StringList strList = StringList::Split( username, "|" );
  if ( strList.size() == 5 )
  {
    const String& bundleId  = strList.at( 0 );
    const String& playerId  = strList.at( 1 );
    const String& timeStamp = strList.at( 2 );

    account->SetUsername( playerId );
    account->SetPassword( playerId );

    //char* b = (char*)timeStamp.c_str();
    uint64 i                = CoreUtils::Atoll( (char*)timeStamp.c_str() );
    String fTimestamp       = String::FormatString("%016llX", i );
    String salt             = strList.at( 3 );
    String decodedSalt      = Base64Util::Decode( salt.stdstr() );
    String decodedSignature = Base64Util::Decode( signature.stdstr() );
    String keyUrl           = strList.at( 4 );

    String fKey = "";
    for ( uint32 tIndex = 0; tIndex<fTimestamp.length(); tIndex+=2 )
    {
      String hex;
      hex += fTimestamp.CharacterAt( tIndex );
      hex += fTimestamp.CharacterAt( tIndex + 1 );
      unsigned char hx = (unsigned char) strtoul( hex.c_str(), NULL, 16 );
      fKey += hx;
    }

    StringList keyList;
    keyList.push_back( playerId );
    keyList.push_back( bundleId );
    keyList.push_back( fKey );
    keyList.push_back( decodedSalt );

    // For now, just load the certificate from the resource folder.
    // TODO change to load if from apple/the request url
    // https://sandbox.gc.apple.com/public-key/gc-sb-2.cer
    ResourceRef* ref = CoreResourceManager::GetInstance()->LoadResource( "gc-sb-2.cer" );
    if ( ref == NULL )
    {
      dfBugreportFMT("AccountManager","VerifyGameCenterAccount failed. Missing SSL certificate [gc-sb-2.cer] for user[%s] : client wants to download from : %s", username.c_str(), keyUrl.c_str() );
      //return GameLoginPacketData::ErrorCode_MissingIOSKey;
      return false;
    }

    SSLNetworkClient* client = new SSLNetworkClient();
    int retKey = client->VerifyKeyList( (const unsigned char*) ref->m_Data, ref->m_Filesize, (unsigned char*) decodedSignature.c_str(), decodedSignature.length(), keyList );
    //int retKey = client->VerifyKey( (const unsigned char*) ref->m_Data, ref->m_Filesize, decodedSignature, fKey );
    switch( retKey )
    {
      case 1:
        break;
      default:
        {
          LogWarningFMT("AccountManager","VerifyGameCenterAccount failed. SSL returned %d for request[%s]", retKey, username.c_str() );
          String rawKey = "";
          rawKey += playerId;
          rawKey += bundleId;
          for ( uint32 tIndex = 0; tIndex<fTimestamp.length(); tIndex+=2 )
          {
            String hex;
            hex += fTimestamp.CharacterAt( tIndex );
            hex += fTimestamp.CharacterAt( tIndex + 1 );
            unsigned char hx = (unsigned char) strtoul( hex.c_str(), NULL, 16 );
            rawKey += hx;
          }
          rawKey += decodedSalt;
          LogWarningFMT("AccountManager","VerifyGameCenterAccount rawKeyLen[%d] rawKeyData[ %s ]", rawKey.length(), rawKey.AsReadable().c_str() );

          account->SetUsername( String::zero );
          account->SetPassword( String::zero );
        }
        return false;
    }
  }
  else
  {
    LogInfoFMT("AccountManager","VerifyGameCenterAccount:: Error with username params (expected 5, got %d)  username[%s]", strList.size(), username.c_str() );
    return false;
  }
#else
  VOLATILE_ARG(account);
#endif
  return true;
}

bool AccountManager::CanAutocreateAccount( uint8 loginMethod )
{
#ifdef SERVER
  switch (loginMethod)
  {
    case Account::LOGIN_METHOD_IOSGAMECENTERCREATE:
    case Account::LOGIN_METHOD_ANDROIDUUID:
      return true;

    default:
      return false;
  }
#else
  VOLATILE_ARG(loginMethod);
  return false;
#endif
}

#ifdef SERVER
Account* AccountManager::GetRandomAccount( const StringList& accountList )
{
#ifdef DATABASE

  String accountListExclude = StringList::Join( accountList, "," );

  const SQLResultSet& result = CoreDatabase::GetInstance()->ExecuteSelect( "select AccountId, Name, IconId from account " \
    "where AccountId not in (%s) "\
    "and AccountId >= (select (floor(max(AccountId) * rand()) + 1) from account) " \
    "limit 1 ", 
    accountListExclude.c_str() );

  if( result.Next() )
  {
    Account* account = new Account();
    account->SetAccountId( result.GetUInt32() );
    account->SetName( result.GetString() );
    account->SetIconId( result.GetUInt32() );
    return account;
  }

#endif

  return NULL;
}
#endif

std::vector< Account* > AccountManager::GetAccountsWithNick( Account* excludeAccount, String searchName)
{
  std::vector< Account* > accountList;
  if (AccountManager::GetInstance()->IsValidName(searchName) == GameEnginePacketData::ErrorCode_OK)
  {
#ifdef SERVER
    const SQLResultSet& result = CoreDatabase::GetInstance()->ExecuteSelect("select AccountId, Name, IconId from account where Name = '%s' ", searchName.c_str());
    while (result.Next())
    {
      uint32 accountId   = result.GetUInt32();
      String accountName = result.GetString();
      uint32 iconId      = result.GetUInt32();
      if (excludeAccount == NULL || accountId != excludeAccount->GetAccountId())
      {
        Account* account = new Account();
        account->SetAccountId( accountId );
        account->SetName( accountName );
        account->SetIconId( iconId );
        accountList.push_back( account );
      }
    }
#else
    VOLATILE_ARG( excludeAccount );
#endif
  }

  return accountList;
}


#ifdef SERVER
bool AccountManager::AddReceipt( Account* account, uint32 itemCode, const String& receiptId, const String& receipt )
{
  // Store the receipt
  bool addReceipt = false;
  if ( receipt.empty() )
  {
    addReceipt = CoreDatabase::GetInstance()->ExecuteInsert( "Insert into purchases ( AccountId, ItemCode, PurchaseDate, ReceiptId, Receipt ) values( %d, %d, now(), '%s', NULL )", account->GetAccountId(), itemCode, receiptId.c_str() );
  }
  else
  {
    addReceipt = CoreDatabase::GetInstance()->ExecuteInsert( "Insert into purchases ( AccountId, ItemCode, PurchaseDate, ReceiptId, Receipt ) values( %d, %d, now(), '%s', '%s' )", account->GetAccountId(), itemCode, receiptId.c_str(), receipt.c_str() );
  }
  if ( !addReceipt )
  {
    LogErrorFMT("AccountManager", "Failed to add receipt to the database AccountId[%d], ItemCode[%d], Receipt[%s]", 
      account->GetAccountId(),
      itemCode,
      receipt.c_str()
      );
  }
  return addReceipt;
}

uint32 AccountManager::GetReceiptWithId( const String& receiptId )
{
  const SQLResultSet& result = CoreDatabase::GetInstance()->ExecuteSelect( "select AccountId from purchases where ReceiptId = '%s' ", receiptId.c_str() );
  if( result.Next() )
  {
    return result.GetUInt32();
  }
  return 0;
}

bool AccountManager::UpdateMembership( Account* account, uint32 itemCode )
{
  // Cannot purchase itemcode 0 OR the GM flag 0
#if !defined DEBUG
  // Make sure the item flags are valid item purchases
  if ( itemCode <= GM_FLAGS::CLIENTGMFLAG )
  {
    return false;
  }
#endif

  if ( account->HasPurchased( itemCode ) )
  {
    //BinaryStream* outData = CoreGameNetworkCommon::CreatePacket(GameLoginPacketData::PacketData_UpdatedAccountFlags);
    //outData->WriteUInt32(account->GetPurchaseFlags());
    //gameConnection->SendData(outData);
    return true;
  }

  uint32 oldPurchaseFlags = account->GetPurchaseFlags();
  account->AddPurchaseFlags( itemCode );
  if ( CoreDatabase::GetInstance()->ExecuteUpdate( "Update account set PurchaseFlags=%d where AccountId=%d", account->GetPurchaseFlags(), account->GetAccountId() ) )
  {
    //BinaryStream* outData = CoreGameNetworkCommon::CreatePacket( GameLoginPacketData::PacketData_UpdatedAccountFlags );
    //outData->WriteUInt32( account->GetPurchaseFlags() );
    //gameConnection->SendData( outData );
    return true;
  }
  else
  {
    account->SetPurchaseFlags( oldPurchaseFlags );
    return false;
  }
}
#endif

#ifdef SERVER
Account* AccountManager::GetOtherAccount(AccountID accountId)
{
  if (accountId == 0)
  {
    dfAssert(accountId != 0, "Trying to resolve account with ID == 0 !");
    return NULL;
  }

  Account* cachedAccount = GetCachedAccount(accountId);
  if (cachedAccount != NULL)
  {
    return cachedAccount;
  }

#ifdef DATABASE
  const SQLResultSet& result = CoreDatabase::GetInstance()->ExecuteSelect("Select AccountId, Name, Username, Password, IconId, PurchaseFlags, LoginMethod, Xp, Level from account where AccountId=%d", accountId);
  if (result.Next())
  {
    Account* account = new Account();
    account->SetAccountId(result.GetUInt32());
    account->SetName(result.GetString());
    account->SetUsername(result.GetString());
    account->SetPassword(result.GetString());
    account->SetIconId(result.GetUInt32());
    account->SetPurchaseFlags(result.GetUInt32());
    account->SetLoginMethod((uint8)result.GetUInt32());
    account->SetScore(result.GetUInt32());
    account->SetLevel((uint8)result.GetUInt32());
    account->SetIsInDatabase(true);
    AddAccountToCache(account);
    return account;
  }

#endif

  return NULL;
}
#endif
