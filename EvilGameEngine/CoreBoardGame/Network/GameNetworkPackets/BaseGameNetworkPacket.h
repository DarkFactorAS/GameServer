#ifndef EVILENGINE_COREBOARDGAME_NETWORK_GAMENETWORKPACKETS_BASEGAMENETWORKPACKET
#define EVILENGINE_COREBOARDGAME_NETWORK_GAMENETWORKPACKETS_BASEGAMENETWORKPACKET 1

#include "EvilEngine/CoreLib/BasicTypes/BinaryStream/BinaryStream.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"
#include "EvilGameEngine/CoreGameLogin/Network/BaseAuthentcatedNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/Account/Account.h"

class Player;
class BoardGame;
class BoardGameServer;

class BaseGameNetworkPacket : public BaseAuthentcatedNetworkPacket
{
public:

  BaseGameNetworkPacket(uint32 packetType, const BinaryStream* datastream);
  BaseGameNetworkPacket(uint32 packetType, uint32 gameId );

  virtual String                GetPacketName() = 0;
  virtual BinaryStream*         GetDataStream()       DF_OVERRIDE;

#ifdef SERVER
  virtual bool                  CanExecute() DF_OVERRIDE;
#endif

protected:

  BoardGameServer*              GetGameServer();
  BoardGame*                    GetGame();
  Player*                       GetPlayer();

#ifdef SERVER
  bool                          IsItMyTurn();
  void                          SendNotMyTurnError();
  void                          SendToAllPlayers(BaseNetworkPacket* gameNetworkPacket);
  void                          SendToObservers(BaseNetworkPacket* gameNetworkPacket );
#endif

  uint32                        m_GameId;
  BoardGame*                    m_BoardGame;
};

#endif /// EVILENGINE_COREBOARDGAME_NETWORK_GAMENETWORKPACKETS_BASEGAMENETWORKPACKET
