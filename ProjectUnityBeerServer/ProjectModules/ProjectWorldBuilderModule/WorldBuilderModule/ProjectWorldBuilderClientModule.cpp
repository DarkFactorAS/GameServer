
#include "PreCompile.h"
#include "ProjectWorldBuilderClientModule.h"

#include "EvilEngine/CoreLib/Utils/Base64Util.h"
#include "EvilEngine/CoreDatabase/CoreDatabase.h"

#include "ProjectWorldBuilderModule/Data/Playfield.h"
#include "ProjectWorldBuilderModule/Data/WorldBuilderPacketData.h"
#include "ProjectWorldBuilderModule/NetworkPackets/ClientNetworkPackets/ClientPlayfieldErrorNetworkPacket.h"
#include "ProjectWorldBuilderModule/NetworkPackets/ClientNetworkPackets/ClientPlayfieldListNetworkPacket.h"
#include "ProjectWorldBuilderModule/NetworkPackets/ClientNetworkPackets/ClientPlayfieldLoadedNetworkPacket.h"
#include "ProjectWorldBuilderModule/NetworkPackets/ClientNetworkPackets/ClientPlayfieldSavedNetworkPacket.h"
#include "ProjectWorldBuilderModule/NetworkPackets/ClientNetworkPackets/ClientPlayfieldDeletedNetworkPacket.h"

ProjectWorldBuilderClientModule::ProjectWorldBuilderClientModule() :
  CoreGameEngineModule(PROJECT_MODULETYPE_CLIENTWORLDBUILDER)
{
  // Server account packets
  RegisterPacketType(WorldBuilderPacketData::PacketData_ClientPlayfieldError, ClientPlayfieldErrorNetworkPacket::Create);
  RegisterPacketType(WorldBuilderPacketData::PacketData_ClientPlayfieldList, ClientPlayfieldListNetworkPacket::Create);
  RegisterPacketType(WorldBuilderPacketData::PacketData_ClientLoadPlayfield, ClientPlayfieldLoadedNetworkPacket::Create);
  RegisterPacketType(WorldBuilderPacketData::PacketData_ClientPlayfieldSaved, ClientPlayfieldSavedNetworkPacket::Create);
  RegisterPacketType(WorldBuilderPacketData::PacketData_ClientPlayfieldDeleted, ClientPlayfieldDeletedNetworkPacket::Create);
}

void ProjectWorldBuilderClientModule::DeletedPlayfield(PlayfieldSource pfSource)
{
  SignalDeletedPlayfield(pfSource);
}

void ProjectWorldBuilderClientModule::SavedPlayfield(PlayfieldSource pfSource)
{
  SignalSavedPlayfield(pfSource);
}

void ProjectWorldBuilderClientModule::LoadedPlayfield(Playfield* playfield)
{
  SignalLoadedPlayfield(playfield);
}

void ProjectWorldBuilderClientModule::PlayfieldNetworkError(uint32 packetType, uint32 errorCode, const String& errorMessage)
{
  SignalPlayfieldNetworkError(packetType, errorCode, errorMessage);
}

