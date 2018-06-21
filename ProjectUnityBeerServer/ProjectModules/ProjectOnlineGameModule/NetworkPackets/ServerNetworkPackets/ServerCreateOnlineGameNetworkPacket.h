#ifndef PROJECT_ONLINEGAME_NETWORKPACKETS_SERVERCREATEONLINEGAMENETWORKPACKET
#define PROJECT_ONLINEGAME_NETWORKPACKETS_SERVERCREATEONLINEGAMENETWORKPACKET 1

#include "ProjectUnityBeerServer/ProjectModules/ProjectLobbyGameModule/NetworkPackets/BaseGameManagementNetworkPacket.h"

#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/Data/Playfield.h"
#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/WorldBuilderModule/ProjectWorldBuilderServerModule.h"

#include "ProjectUnityBeerServer/ProjectModules/ProjectOnlineGameModule/Module/ProjectOnlineGameServerModule.h"

class ServerCreateOnlineGameNetworkPacket : public BaseGameManagementNetworkPacket
{
public:

  ServerCreateOnlineGameNetworkPacket(uint32 lobbyGameId);
  ServerCreateOnlineGameNetworkPacket(const BinaryStream* datastream);

  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new ServerCreateOnlineGameNetworkPacket(datastream); }

  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("ServerCreateOnlineGame"); }
  virtual void                  Execute()         DF_OVERRIDE;
  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;

private:

  ProjectOnlineGameServerModule* GetOnlineGameModule();

  uint32                        m_LobbyGameId;

};

#endif /// PROJECT_ONLINEGAME_NETWORKPACKETS_SERVERCREATEONLINEGAMENETWORKPACKET#pragma once
