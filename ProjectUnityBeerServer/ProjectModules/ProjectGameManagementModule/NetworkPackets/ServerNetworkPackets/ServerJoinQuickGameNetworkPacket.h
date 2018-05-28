#ifndef PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_SERVERJOINQUICKGAMEWORKPACKET
#define PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_SERVERJOINQUICKGAMEWORKPACKET 1

#include "ProjectUnityBeerServer/ProjectModules/ProjectGameManagementModule/NetworkPackets/BaseGameManagementNetworkPacket.h"

#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/Data/Playfield.h"
#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/WorldBuilderModule/ProjectWorldBuilderServerModule.h"

class ServerJoinQuickGameNetworkPacket : public BaseGameManagementNetworkPacket
{
public:

  ServerJoinQuickGameNetworkPacket();
  ServerJoinQuickGameNetworkPacket(const BinaryStream* datastream);

  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new ServerJoinQuickGameNetworkPacket(datastream); }

  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("ServerJoinQuickGame"); }
  virtual void                  Execute()         DF_OVERRIDE;
  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;

};

#endif /// PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_SERVERJOINQUICKGAMEWORKPACKET#pragma once
