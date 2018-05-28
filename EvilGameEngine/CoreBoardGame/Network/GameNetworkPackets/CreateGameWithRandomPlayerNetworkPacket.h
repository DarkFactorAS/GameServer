#ifndef EVILENGINE_COREBOARDGAME_NETWORK_GAMENETWORKPACKETS_CREATEGAMEWITHRANDOMPLAYER_NEWORKPACKET
#define EVILENGINE_COREBOARDGAME_NETWORK_GAMENETWORKPACKETS_CREATEGAMEWITHRANDOMPLAYER_NEWORKPACKET 1

#include "EvilEngine/CoreLib/BasicTypes/BinaryStream/BinaryStream.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"
#include "EvilGameEngine/CoreBoardGame/Network/GameNetworkPackets/BaseGameNetworkPacket.h"
#include "EvilGameEngine/CoreBoardGame/GameLogic/Game/BoardGameServer.h"

class CreateGameWithRandomPlayerNetworkPacket : public BaseAuthentcatedNetworkPacket
{
public:

#ifdef CLIENT
  CreateGameWithRandomPlayerNetworkPacket();
#else
  virtual void                  Execute()         DF_OVERRIDE;
#endif

  CreateGameWithRandomPlayerNetworkPacket(const BinaryStream* datastream);

  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new CreateGameWithRandomPlayerNetworkPacket(datastream); }
  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("CreateGameWithRandomPlayer"); }
  BoardGameServer*              GetGameServer(CoreGameEngine& engine);

};

#endif /// EVILENGINE_COREBOARDGAME_NETWORK_GAMENETWORKPACKETS_CREATEGAMEWITHRANDOMPLAYER_NEWORKPACKET