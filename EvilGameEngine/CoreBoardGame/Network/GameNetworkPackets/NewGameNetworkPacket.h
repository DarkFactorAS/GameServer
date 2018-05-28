#ifndef EVILENGINE_COREBOARDGAME_NETWORK_GAMENETWORKPACKETS_NEWGAMENETWORKPACKET
#define EVILENGINE_COREBOARDGAME_NETWORK_GAMENETWORKPACKETS_NEWGAMENETWORKPACKET 1

#include "EvilEngine/CoreLib/BasicTypes/BinaryStream/BinaryStream.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"
#include "EvilGameEngine/CoreBoardGame/Network/GameNetworkPackets/BaseGameNetworkPacket.h"
#include "EvilGameEngine/CoreBoardGame/GameLogic/Game/BoardGame.h"

class NewGameNetworkPacket : public BaseAuthentcatedNetworkPacket
{
public:

#ifdef CLIENT

  virtual void                  Execute()               DF_OVERRIDE;

#else

  NewGameNetworkPacket(BoardGame* game);
  virtual BinaryStream*         GetDataStream()        DF_OVERRIDE;

#endif

  NewGameNetworkPacket(const BinaryStream* datastream);
  virtual String                GetPacketName()        DF_OVERRIDE { return StaticStr("ReceiveGame"); }
  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new NewGameNetworkPacket(datastream); }
  BoardGameServer*              GetGameServer(CoreGameEngine& engine);
  virtual BoardGame*            CreateGameClass(const BinaryStream* datastream);
  virtual bool                  ReadDataStream(const BinaryStream* datastream) DF_OVERRIDE;

private:

  BoardGame*                    m_NewGame;
};

#endif /// EVILENGINE_COREBOARDGAME_NETWORK_GAMENETWORKPACKETS_NEWGAMENETWORKPACKET