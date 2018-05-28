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
#include "ClientPlayfieldErrorNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/Network/BaseAuthentcatedNetworkPacket.h"
#include "ProjectWorldBuilderModule/Data/WorldBuilderPacketData.h"
#include "ProjectWorldBuilderModule/WorldBuilderModule/ProjectWorldBuilderClientModule.h"

ClientPlayfieldErrorNetworkPacket::ClientPlayfieldErrorNetworkPacket(const BinaryStream* datastream) :
  BaseNetworkPacket(WorldBuilderPacketData::PacketData_ClientPlayfieldError, datastream)
{
  m_PacketType = datastream->ReadUInt32();
  m_ErrorCode = datastream->ReadUInt32();
  m_Message   = datastream->ReadString();
}

ClientPlayfieldErrorNetworkPacket::ClientPlayfieldErrorNetworkPacket(uint32 packetType, uint32 errorCode, const String& message) :
  BaseNetworkPacket(WorldBuilderPacketData::PacketData_ClientPlayfieldError),
  m_PacketType(packetType),
  m_ErrorCode(errorCode),
  m_Message(message)
{
}

BinaryStream* ClientPlayfieldErrorNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseNetworkPacket::GetDataStream();

  datastream->WriteUInt32(m_PacketType);
  datastream->WriteUInt32(m_ErrorCode);
  datastream->WriteString(m_Message);
 
  return datastream;
}

void ClientPlayfieldErrorNetworkPacket::Execute()
{
  ProjectWorldBuilderClientModule* worldbuilderModule = safe_cast<ProjectWorldBuilderClientModule*> (GetEngineModule(ProjectWorldBuilderClientModule::PROJECT_MODULETYPE_CLIENTWORLDBUILDER));
  if (worldbuilderModule != NULL)
  {
    worldbuilderModule->PlayfieldNetworkError(m_PacketType, m_ErrorCode, m_Message);
  }
}
