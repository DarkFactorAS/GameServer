#ifndef PROJECT_WORLDBUILDERMODULE_NETWORKPACKETS_BASEWORLDBUILDERNETWORKPACKET
#define PROJECT_WORLDBUILDERMODULE_NETWORKPACKETS_BASEWORLDBUILDERNETWORKPACKET 1

#include "EvilEngine/CoreLib/BasicTypes/BinaryStream/BinaryStream.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/Packets/BaseNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/Network/BaseAuthentcatedNetworkPacket.h"

#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/Data/WorldBuilderPacketData.h"

class BaseWorldBuilderNetworkPacket : public BaseAuthentcatedNetworkPacket
{
public:

  BaseWorldBuilderNetworkPacket(WorldBuilderPacketData::PacketData packetType, const BinaryStream* datastream);
  BaseWorldBuilderNetworkPacket(WorldBuilderPacketData::PacketData packetType);

  void SendPlayfieldErrorToClient(uint32 errorId);
  void SendPlayfieldErrorToClient(WorldBuilderPacketData::PacketError errorId);
};

#endif /// PROJECT_WORLDBUILDERMODULE_NETWORKPACKETS_BASEWORLDBUILDERNETWORKPACKET#pragma once
