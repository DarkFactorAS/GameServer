#ifndef PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_SERVERCREATEGAMENETWORKPACKET
#define PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_SERVERCREATEGAMENETWORKPACKET 1

#include "ProjectUnityBeerServer/ProjectModules/ProjectLobbyGameModule/NetworkPackets/BaseGameManagementNetworkPacket.h"

#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/Data/Playfield.h"
#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/WorldBuilderModule/ProjectWorldBuilderServerModule.h"

class ServerCreateGameNetworkPacket : public BaseGameManagementNetworkPacket
{
public:

  ServerCreateGameNetworkPacket(uint32 playfieldId, uint32 robotId);
  ServerCreateGameNetworkPacket(const BinaryStream* datastream);

  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new ServerCreateGameNetworkPacket(datastream); }

  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("ServerCreateGame"); }
  virtual void                  Execute()         DF_OVERRIDE;
  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;

private:

  ProjectWorldBuilderServerModule* GetWorldBuilderModule();

  uint32                        m_PlayfieldId;
  uint32                        m_RobotId;

};

#endif /// PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_SERVERCREATEGAMENETWORKPACKET#pragma once
