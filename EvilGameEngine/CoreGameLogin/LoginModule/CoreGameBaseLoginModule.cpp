/*************************************************************************************
 * Project     : EvilEngine Multiplatform 2014
 * File        : EngineModule
 * Description : Abstract interface class for CoreGameEngine modules. This interface
 *               has to be implemented to be able to run as a sub component in the
 *               CoreGameEngine.
 *
 * Author      : Thor Richard Hansen
 *************************************************************************************/

#include "Precompile.h"
#include "CoreGameBaseLoginModule.h"

#include "EvilEngine/CoreEngine/CoreEngine/EngineModule.h"
#include "EvilEngine/CoreNetwork/Common/NetworkBase.h"

#include "EvilEngine/CoreLib/BasicTypes/Definitions/VolatileArgs.h"

#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngineModule.h"

#include "EvilGameEngine/CoreGameLogin/Account/AccountManager.h"
#include "EvilGameEngine/CoreGameLogin/Network/GameLoginPacketData.h"

#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ClientAccountNetworkPackets/ClientAccountLoggedOnNetworkPacket.h"

#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ServerAccountNetworkPackets/CreateServerAccountNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ServerAccountNetworkPackets/LoginServerAccountNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ServerAccountNetworkPackets/LoginGMNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ServerAccountNetworkPackets/UpdatedAccountFlagsNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ServerAccountNetworkPackets/SwitchLoggedInAccountNetworkPacket.h"

#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ItemShopPackets/PurchaseItemFromAmazonNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ItemShopPackets/PurchaseItemFromAppleNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ItemShopPackets/PurchaseItemFromDarkfactorNetworkPacket.h"

CoreGameBaseLoginModule::CoreGameBaseLoginModule(int moduleId, bool isLive) : CoreGameEngineModule(moduleId)
{
  // Network error messages
  AddLoginError(GameLoginPacketData::ErrorCode_NotLoggedIn, "User is not logged in");
  AddLoginError(GameLoginPacketData::ErrorCode_UserAlreadyExists, "User already exists on the server");
  AddLoginError(GameLoginPacketData::ErrorCode_NameTooShort, "Nickname too short");
  AddLoginError(GameLoginPacketData::ErrorCode_NameTooLong, "Nickname too long");
  AddLoginError(GameLoginPacketData::ErrorCode_NameHasInvalidCharacters, "Invalid characters in nickname");
  AddLoginError(GameLoginPacketData::ErrorCode_UsernameTooShort, "Username too short");
  AddLoginError(GameLoginPacketData::ErrorCode_UsernameTooLong, "Username too long");
  AddLoginError(GameLoginPacketData::ErrorCode_UsernameHasInvalidCharacters, "Invalid characters in username");
  AddLoginError(GameLoginPacketData::ErrorCode_UsernameIsNotValidEmail, "Username is not a valid e-mail");
  AddLoginError(GameLoginPacketData::ErrorCode_PasswordHasInvalidCharacters, "Invalid characters in password");
  AddLoginError(GameLoginPacketData::ErrorCode_AccountAlreadyLoggedIn, "You are already logged on the server");
  AddLoginError(GameLoginPacketData::ErrorCode_PasswordTooShort, "Password too short");
  AddLoginError(GameLoginPacketData::ErrorCode_PasswordTooLong, "Password too long");

  if (!isLive)
  {
    AddLoginError(GameLoginPacketData::ErrorCode_WrongPassword, "Wrong password");
    AddLoginError(GameLoginPacketData::ErrorCode_LoginNoSuchUser, "No such user");
    AddLoginError(GameLoginPacketData::ErrorCode_BrokenAccountData, "Broken account data");
    AddLoginError(GameLoginPacketData::ErrorCode_RequiresGM, "You must be GM to do this");

  }
  else
  {
    AddLoginError(GameLoginPacketData::ErrorCode_WrongPassword, "Unknown user/password");
    AddLoginError(GameLoginPacketData::ErrorCode_LoginNoSuchUser, "Unknown user/password");
    AddLoginError(GameLoginPacketData::ErrorCode_BrokenAccountData, "Unknown error. \nPlease contact support");
    AddLoginError(GameLoginPacketData::ErrorCode_RequiresGM, "You must be GM to do this");
  }
}

void CoreGameBaseLoginModule::AddLoginError(uint32 errorCode, const String& errorMessage)
{
  // TODO > Check if the string is the same as well If it is different assert here!
  std::map< uint32, String >::iterator itFound = m_ErrorMessages.find(errorCode);
  if (itFound != m_ErrorMessages.end())
  {
    LogWarningFMT("NetworkBase::AddLoginError", "Error message [%d] is already registered (%s)", errorCode, errorMessage.c_str());
  }
  m_ErrorMessages[errorCode] = errorMessage;
}

const String& CoreGameBaseLoginModule::GetLoginError(uint32 errorCode)
{
  std::map< uint32, String >::iterator itFound = m_ErrorMessages.find(errorCode);
  if (itFound != m_ErrorMessages.end())
  {
    return itFound->second;
  }
  LogWarningFMT("NetworkBase::GetLoginError", "Error message [%d] not found in map", errorCode);
  return String::zero;
}

