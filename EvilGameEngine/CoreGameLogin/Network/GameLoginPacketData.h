#ifndef EVILENGINE_COREGAMELOGIN_NETWORK_GameLoginPacketData
#define EVILENGINE_COREGAMELOGIN_NETWORK_GameLoginPacketData 1
#pragma once

namespace GameLoginPacketData
{
  enum PacketData
  {
    // 300-399 Account data
    PacketData_AccountList            = 300,
    PacketData_CreateServerAccount    = 301,
    PacketData_LoginServerAccount     = 302,
    PacketData_AccountDelete          = 304,
    PacketData_AccountUpdate          = 305,
    PacketData_WindowsItemPurchase    = 306,
    PacketData_AmazonItemPurchase     = 307,
    PacketData_AmazonPurchaseFailed   = 308,
    PacketData_AmazonPurchaseIgnore   = 309,
    PacketData_IOSItemPurchase        = 310,
    PacketData_UpdatedAccountFlags    = 311,
    PacketData_LoginGM                = 312,
    PacketData_SwitchUser             = 313,

    PacketData_CreateClientAccount    = 331,
    PacketData_ClientLoggedOnAccount  = 332,
    PacketData_LoginClientError       = 333,
    PacketData_ClientLoggedOffAccount = 334,
  };

  enum PacketError
  {
    // 300-399 Account errors
    ErrorCode_CodeError                     = 300,
    ErrorCode_BrokenAccountData             = 301,
    ErrorCode_NotLoggedIn                   = 302,
    ErrorCode_WrongPassword                 = 303,
    ErrorCode_LoginNoSuchUser               = 304,
    ErrorCode_MissingIOSKey                 = 305,
    ErrorCode_UserAlreadyExists             = 306,
    ErrorCode_NameTooShort                  = 307,
    ErrorCode_NameTooLong                   = 308,
    ErrorCode_NameHasInvalidCharacters      = 309,
    ErrorCode_UsernameTooShort              = 310,
    ErrorCode_UsernameTooLong               = 311,
    ErrorCode_UsernameHasInvalidCharacters  = 312,
    ErrorCode_UsernameIsNotValidEmail       = 313,
    ErrorCode_PasswordHasInvalidCharacters  = 314,
    ErrorCode_YouMustLogInToGameCenter      = 315,
    ErrorCode_YouMustEnterName              = 316,
    ErrorCode_RequiresGM                    = 317,
    ErrorCode_AccountAlreadyLoggedIn        = 318,
    ErrorCode_PasswordTooShort              = 319,
    ErrorCode_PasswordTooLong               = 320,
  };

  //static const String           GetErrorString( uint8 errorCode );
};

#endif /// EVILENGINE_COREGAMELOGIN_NETWORK_GameLoginPacketData
