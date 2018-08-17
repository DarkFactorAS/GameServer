#ifndef PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_SERVERACTIVEGAMELISTNETWORKPACKET
#define PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_SERVERACTIVEGAMELISTNETWORKPACKET 1

#include "ProjectUnityBeerServer/ProjectModules/ProjectLobbyGameModule/NetworkPackets/BaseGameManagementNetworkPacket.h"

#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/Data/Playfield.h"
#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/WorldBuilderModule/ProjectWorldBuilderServerModule.h"

class ServerRequestActiveOnlineGameNetworkPacket : public BaseGameManagementNetworkPacket
{
public:

  ServerRequestActiveOnlineGameNetworkPacket();
  ServerRequestActiveOnlineGameNetworkPacket(const BinaryStream* datastream);

  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new ServerRequestActiveOnlineGameNetworkPacket(datastream); }

  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("ServerRequestActiveOnlineGame"); }
  virtual void                  Execute()         DF_OVERRIDE;

};

#endif /// PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_SERVERACTIVEGAMELISTNETWORKPACKET#pragma once
