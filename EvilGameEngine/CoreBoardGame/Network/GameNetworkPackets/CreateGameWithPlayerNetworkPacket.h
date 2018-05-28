#ifndef EVILENGINE_COREBOARDGAME_NETWORK_GAMENETWORKPACKETS_CREATEGAMEWITHPLAYER_NEWORKPACKET
#define EVILENGINE_COREBOARDGAME_NETWORK_GAMENETWORKPACKETS_CREATEGAMEWITHPLAYER_NEWORKPACKET 1

#include "EvilEngine/CoreLib/BasicTypes/BinaryStream/BinaryStream.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"
#include "EvilGameEngine/CoreBoardGame/Network/GameNetworkPackets/BaseGameNetworkPacket.h"
#include "EvilGameEngine/CoreBoardGame/GameLogic/Game/BoardGameServer.h"

class CreateGameWithPlayerNetworkPacket : public BaseAuthentcatedNetworkPacket
{
public:

#ifdef CLIENT
  CreateGameWithPlayerNetworkPacket( AccountID opponentId );
  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;
#else
  virtual void                  Execute()         DF_OVERRIDE;
#endif

  CreateGameWithPlayerNetworkPacket(const BinaryStream* datastream);
  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("CreateGameWithPlayer"); }
  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new CreateGameWithPlayerNetworkPacket(datastream); }

private:

  AccountID                     m_OpponentId;

};

#endif /// EVILENGINE_COREBOARDGAME_NETWORK_GAMENETWORKPACKETS_CREATEGAMEWITHPLAYER_NEWORKPACKET