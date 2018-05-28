/*************************************************************************************************
* Project     : EvilEngine Multi-platform 2014-2016
* File        : LoginAccountNetworkPacket
*
* Description : Network packet to log in the player on the server. Most of the game related
*               packets require the player to be logged on to the server.
*
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "ClientGameErrorNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/Network/BaseAuthentcatedNetworkPacket.h"
#include "ProjectGameManagementModule/Data/GameManagementPacketData.h"
#include "ProjectGameManagementModule/Logic/GameManagementModule/ProjectGameManagementClientModule.h"

ClientGameErrorNetworkPacket::ClientGameErrorNetworkPacket(const BinaryStream* datastream) :
  BaseNetworkPacket(GameEnginePacketData::PacketData_ClientGameError, datastream)
{
  m_PacketType = datastream->ReadUInt32();
  m_ErrorCode = datastream->ReadUInt32();
  m_Message = datastream->ReadString();
}

ClientGameErrorNetworkPacket::ClientGameErrorNetworkPacket(uint32 packetType, uint32 errorCode, const String& message) :
  BaseNetworkPacket(GameEnginePacketData::PacketData_ClientGameError),
  m_PacketType(packetType),
  m_ErrorCode(errorCode),
  m_Message(message)
{
}

BinaryStream* ClientGameErrorNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseNetworkPacket::GetDataStream();

  datastream->WriteUInt32(m_PacketType);
  datastream->WriteUInt32(m_ErrorCode);
  datastream->WriteString(m_Message);

  return datastream;
}

void ClientGameErrorNetworkPacket::Execute()
{
  ProjectGameManagementClientModule* module = safe_cast<ProjectGameManagementClientModule*> (GetEngineModule(ProjectGameManagementClientModule::PROJECT_MODULETYPE_CLIENTGAMEMANAGEMENT));
  if (module != NULL)
  {
    module->GameNetworkError(m_PacketType, m_ErrorCode, m_Message);
  }
}
