#ifndef EVILENGINE_COREBOARDGAME_NETWORK_GAMENETWORKPACKETS_LEAVEGAMENETWORKPACKET
#define EVILENGINE_COREBOARDGAME_NETWORK_GAMENETWORKPACKETS_LEAVEGAMENETWORKPACKET 1

#include "EvilEngine/CoreLib/BasicTypes/BinaryStream/BinaryStream.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"
#include "EvilGameEngine/CoreBoardGame/Network/GameNetworkPackets/BaseGameNetworkPacket.h"
#include "EvilGameEngine/CoreBoardGame/GameLogic/Game/BoardGameServer.h"

class DeleteGameNetworkPacket : public BaseGameNetworkPacket
{
public:

  DeleteGameNetworkPacket(uint32 gameId);

  DeleteGameNetworkPacket( const BinaryStream* datastream);
  
  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new DeleteGameNetworkPacket(datastream); }
  virtual String                GetPacketName()     DF_OVERRIDE { return StaticStr("DeleteGame"); }
  virtual void                  Execute()           DF_OVERRIDE;
};

#endif /// EVILENGINE_COREBOARDGAME_NETWORK_GAMENETWORKPACKETS_LEAVEGAMENETWORKPACKET