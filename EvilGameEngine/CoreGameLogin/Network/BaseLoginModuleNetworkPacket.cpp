
#include "Precompile.h"

#include "BaseLoginModuleNetworkPacket.h"
#include "GameLoginPacketData.h"

#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngineModule.h"

#include "EvilGameEngine/CoreGameLogin/LoginModule/CoreGameServerLoginModule.h"
#include "EvilGameEngine/CoreGameLogin/Account/AccountManager.h"
#include "EvilGameEngine/CoreGameLogin/Account/Account.h"

#include "EvilGameEngine/CoreGameLogin/NetworkPackets/ClientAccountNetworkPackets/ClientAccountErrorNetworkPacket.h"

BaseLoginModuleNetworkPacket::BaseLoginModuleNetworkPacket( uint32 packetType, const BinaryStream* datastream ) :
  BaseNetworkPacket(packetType, datastream)
{
}

BaseLoginModuleNetworkPacket::BaseLoginModuleNetworkPacket( uint32 packetType) :
  BaseNetworkPacket( packetType )
{
}


void BaseLoginModuleNetworkPacket::SendLoginErrorToClient(uint32 packetTypeId, uint32 errorCodeId)
{
  CoreGameEngine* gameEngine = GetGameEngine();
  if (gameEngine != NULL)
  {

    String errorMessage = String::zero;
    CoreGameServerLoginModule* loginModule = safe_cast<CoreGameServerLoginModule*> (gameEngine->GetEngineModule(EngineModule::COREMODULETYPE_SERVERLOGIN));
    if (loginModule != NULL)
    {
      errorMessage = loginModule->GetLoginError(errorCodeId);
    }

    LogInfoFMT("BaseLoginModuleNetworkPacket", "SendLoginErrorToClient::Received error => %d/%d (%s)", packetTypeId, errorCodeId, errorMessage.c_str());

    gameEngine->SendPacketToEndpoint( m_ConnectionId, new ClientAccountErrorNetworkPacket(packetTypeId, errorCodeId, errorMessage) );
  }
}
