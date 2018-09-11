#ifndef PROJECT_ONLINEGAME_NETWORKPACKETS_BASE
#define PROJECT_ONLINEGAME_NETWORKPACKETS_BASE 1

#include "ProjectUnityBeerServer/ProjectModules/ProjectOnlineGameModule/Module/ProjectOnlineGameServerModule.h"
#include "ProjectUnityBeerServer/ProjectModules/ProjectOnlineGameModule/Data/OnlineGamePacketData.h"
#include "ProjectUnityBeerServer/ProjectModules/ProjectOnlineGameModule/NetworkPackets/ServerNetworkPackets/ServerBaseOnlineGameNetworkPacket.h"

#include "EvilGameEngine/CoreGameLogin/Network/BaseAuthentcatedNetworkPacket.h"

//#include "ProjectUnityBeerServer/ProjectModules/ProjectGameManagementModule/NetworkPackets/BaseGameManagementNetworkPacket.h"
//#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/Data/Playfield.h"

class ServerBaseOnlineGameNetworkPacket : public BaseAuthentcatedNetworkPacket
{
public:

  ServerBaseOnlineGameNetworkPacket(OnlineGamePacketData::PacketData packetType, const BinaryStream* datastream);
  ServerBaseOnlineGameNetworkPacket(OnlineGamePacketData::PacketData packetType);

  void                            SendGameErrorToClient(uint32 errorId);
  void                            SendGameErrorToClient(OnlineGamePacketData::PacketError errorId);

  void                            SendPacketToOnlineGamePlayers(OnlineGameData* lobbyGame, BaseNetworkPacket* packet);

  ProjectOnlineGameServerModule*  GetModule();

protected:

  bool                            CanExecuteAsAccount(uint32 accountId);
};

#endif /// PROJECT_ONLINEGAME_NETWORKPACKETS_BASE
