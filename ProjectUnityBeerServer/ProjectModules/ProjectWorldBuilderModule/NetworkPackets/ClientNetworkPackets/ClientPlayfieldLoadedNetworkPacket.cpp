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
#include "ClientPlayfieldLoadedNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/Network/BaseAuthentcatedNetworkPacket.h"
#include "ProjectWorldBuilderModule/Data/WorldBuilderPacketData.h"
#include "ProjectWorldBuilderModule/Data/Playfield.h"
#include "ProjectWorldBuilderModule/WorldBuilderModule/ProjectWorldBuilderClientModule.h"

ClientPlayfieldLoadedNetworkPacket::ClientPlayfieldLoadedNetworkPacket(const BinaryStream* datastream) :
  BaseNetworkPacket(WorldBuilderPacketData::PacketData_ClientLoadPlayfield, datastream)
{
  m_Playfield = Playfield::CreatePlayfieldFromStream(datastream);
}

ClientPlayfieldLoadedNetworkPacket::ClientPlayfieldLoadedNetworkPacket(Playfield* playfield) :
  BaseNetworkPacket(WorldBuilderPacketData::PacketData_ClientLoadPlayfield),
  m_Playfield(playfield)
{
}

BinaryStream* ClientPlayfieldLoadedNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseNetworkPacket::GetDataStream();
  m_Playfield->CreateBinaryStream(datastream, true);
  return datastream;
}

void ClientPlayfieldLoadedNetworkPacket::Execute()
{
  ProjectWorldBuilderClientModule* worldbuilderModule = safe_cast<ProjectWorldBuilderClientModule*> (GetEngineModule(ProjectWorldBuilderClientModule::PROJECT_MODULETYPE_CLIENTWORLDBUILDER));
  if (worldbuilderModule != NULL)
  {
    worldbuilderModule->LoadedPlayfield(m_Playfield);
  }
}
