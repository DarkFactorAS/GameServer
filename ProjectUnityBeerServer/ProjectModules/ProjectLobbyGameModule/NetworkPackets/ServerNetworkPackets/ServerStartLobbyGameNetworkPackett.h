#ifndef PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_SERVERSTARTGAMENETWORKPACKET
#define PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_SERVERSTARTGAMENETWORKPACKET 1

#include "ProjectUnityBeerServer/ProjectModules/ProjectLobbyGameModule/NetworkPackets/BaseGameManagementNetworkPacket.h"

#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/Data/Playfield.h"
#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/WorldBuilderModule/ProjectWorldBuilderServerModule.h"

#include "ProjectUnityBeerServer/ProjectModules/ProjectOnlineGameModule/Module/ProjectOnlineGameServerModule.h"

class ServerStartLobbyGameNetworkPackett : public BaseGameManagementNetworkPacket
{
public:

  ServerStartLobbyGameNetworkPackett(uint32 lobbyGameId);
  ServerStartLobbyGameNetworkPackett(const BinaryStream* datastream);

  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new ServerStartLobbyGameNetworkPackett(datastream); }

  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("ServerStartLobbyGame"); }
  virtual void                  Execute()         DF_OVERRIDE;
  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;

private:

  ProjectOnlineGameServerModule* GetOnlineGameModule();

  uint32                        m_LobbyGameId;

};

#endif /// PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_SERVERSTARTGAMENETWORKPACKET#pragma once
