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
#include "ClientPlayfieldSavedNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/Network/BaseAuthentcatedNetworkPacket.h"
#include "ProjectWorldBuilderModule/Data/WorldBuilderPacketData.h"
#include "ProjectWorldBuilderModule/Data/Playfield.h"
#include "ProjectWorldBuilderModule/WorldBuilderModule/ProjectWorldBuilderClientModule.h"

ClientPlayfieldSavedNetworkPacket::ClientPlayfieldSavedNetworkPacket(const BinaryStream* datastream) :
  BaseNetworkPacket(WorldBuilderPacketData::PacketData_ClientPlayfieldSaved, datastream)
{
  m_PlayfieldSource = datastream->ReadUInt32();
  m_PlayfieldId = datastream->ReadUInt32();
  m_PlayfieldTokenId = datastream->ReadString();
}

ClientPlayfieldSavedNetworkPacket::ClientPlayfieldSavedNetworkPacket(uint32 playfieldSource, uint32 playfieldId, const String& playfieldTokenId) :
  BaseNetworkPacket(WorldBuilderPacketData::PacketData_ClientPlayfieldSaved),
  m_PlayfieldSource(playfieldSource),
  m_PlayfieldId(playfieldId),
  m_PlayfieldTokenId(playfieldTokenId)
{
}

BinaryStream* ClientPlayfieldSavedNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseNetworkPacket::GetDataStream();

  datastream->WriteUInt32(m_PlayfieldSource);
  datastream->WriteUInt32(m_PlayfieldId);
  datastream->WriteString(m_PlayfieldTokenId);
 
  return datastream;
}

void ClientPlayfieldSavedNetworkPacket::Execute()
{
  ProjectWorldBuilderClientModule* worldbuilderModule = safe_cast<ProjectWorldBuilderClientModule*> (GetEngineModule(ProjectWorldBuilderClientModule::PROJECT_MODULETYPE_CLIENTWORLDBUILDER));
  if (worldbuilderModule != NULL)
  {
    worldbuilderModule->SavedPlayfield(PlayfieldSource(m_PlayfieldSource, m_PlayfieldId, m_PlayfieldTokenId));
  }
}
