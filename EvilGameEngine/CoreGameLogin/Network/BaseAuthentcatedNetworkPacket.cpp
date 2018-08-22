
#include "Precompile.h"
#include "BaseAuthentcatedNetworkPacket.h"
#include "GameLoginPacketData.h"

#include "EvilGameEngine/CoreGameLogin/Account/AccountManager.h"

BaseAuthentcatedNetworkPacket::BaseAuthentcatedNetworkPacket( uint32 packetType, const BinaryStream* datastream ) :
  BaseLoginModuleNetworkPacket(packetType, datastream),
  m_Account(NULL)
{
}

BaseAuthentcatedNetworkPacket::BaseAuthentcatedNetworkPacket( uint32 packetType) :
  BaseLoginModuleNetworkPacket( packetType ),
  m_Account(NULL)
{
}

/************************************************************************************************
* GetAccount:
* Return the account for this connection
*
* @param  (uint32)              networkInstance - The ID of the connection from the client
* @return (Accoun*)                             - The account belonging to this packet
*
* @author Thor Richard Hansen
*************************************************************************************************/
Account* BaseAuthentcatedNetworkPacket::GetAccount()
{
  if ( m_Account != NULL )
  {
    return m_Account;
  }

  AccountManager* accountManager = AccountManager::GetInstance();
  if (accountManager == NULL)
  {
    return NULL;
  }

#ifdef SERVER
  m_Account = accountManager->GetOnlineAccountFromNetwork( m_ConnectionId );
#endif
  return m_Account;
}

/************************************************************************************************
* CanExecute:
* Check if the account is logged on to the server. If it isn't, refuse to execute the packet and
* send an error message back to the client
*
* @param  (CoreGameEngine&)     engine          - The engine to execute on
* @param  (uint32)              networkInstance - The ID of the connection from the client
* @return (bool)                                - Return true if the account is logged in
*
* @author Thor Richard Hansen
*************************************************************************************************/
bool BaseAuthentcatedNetworkPacket::IsSenderGM()
{
  Account* account = GetAccount();
  if ( account != NULL )
  {
    return account->IsServerGMEnabled();  
  }
  return false;
}

/************************************************************************************************
* CanExecute:
* Check if the account is logged on to the server. If it isn't, refuse to execute the packet and
* send an error message back to the client
*
* @param  (CoreGameEngine&)     engine          - The engine to execute on
* @param  (uint32)              networkInstance - The ID of the connection from the client
* @return (bool)                                - Return true if the account is logged in
*
* @author Thor Richard Hansen
*************************************************************************************************/
#ifdef SERVER
bool BaseAuthentcatedNetworkPacket::CanExecute()
{
  Account* account = GetAccount();
  if ( account == NULL || !account->IsAuthenticated() )
  {
    SendErrorToClient(GameLoginPacketData::ErrorCode_NotLoggedIn);
    return false;
  }
  return true;
}

#endif
