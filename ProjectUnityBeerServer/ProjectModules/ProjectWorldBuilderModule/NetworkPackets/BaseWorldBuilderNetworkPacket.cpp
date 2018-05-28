
#include "Precompile.h"
#include "BaseWorldBuilderNetworkPacket.h"
#include "ProjectWorldBuilderModule/NetworkPackets/ClientNetworkPackets/ClientPlayfieldErrorNetworkPacket.h"
#include "ProjectWorldBuilderModule/WorldBuilderModule/ProjectWorldBuilderServerModule.h"

BaseWorldBuilderNetworkPacket::BaseWorldBuilderNetworkPacket(WorldBuilderPacketData::PacketData packetType, const BinaryStream* datastream) :
  BaseAuthentcatedNetworkPacket((uint32)packetType, datastream)
{
}

BaseWorldBuilderNetworkPacket::BaseWorldBuilderNetworkPacket(WorldBuilderPacketData::PacketData packetType) :
  BaseAuthentcatedNetworkPacket((uint32)packetType)
{
}

void BaseWorldBuilderNetworkPacket::SendPlayfieldErrorToClient(WorldBuilderPacketData::PacketError errorId)
{
  SendPlayfieldErrorToClient( (uint32) errorId );
}

void BaseWorldBuilderNetworkPacket::SendPlayfieldErrorToClient(uint32 errorCodeId)
{
  CoreGameEngine* gameEngine = GetGameEngine();
  if (gameEngine != NULL)
  {
    String errorMessage = String::zero;
    ProjectWorldBuilderServerModule* worldBuilderModule = safe_cast<ProjectWorldBuilderServerModule*> (gameEngine->GetEngineModule(ProjectWorldBuilderServerModule::PROJECT_MODULETYPE_WORLDBUILDER));
    if (worldBuilderModule != NULL)
    {
      errorMessage = worldBuilderModule->GetLoginError(errorCodeId);
    }

    LogInfoFMT("BaseWorldBuilderNetworkPacket", "SendPlayfieldErrorToClient::Received error => %d/%d [%s]", m_PacketType, errorCodeId, errorMessage.c_str());
    gameEngine->SendPacketToEndpoint(m_ConnectionId, new ClientPlayfieldErrorNetworkPacket(m_PacketType, errorCodeId, errorMessage));
  }
}
