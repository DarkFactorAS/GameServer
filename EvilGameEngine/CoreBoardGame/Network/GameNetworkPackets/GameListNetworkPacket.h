#ifndef EVILENGINE_COREBOARDGAME_NETWORK_GAMENETWORKPACKETS_GAMELIST_NETWORKPACKET
#define EVILENGINE_COREBOARDGAME_NETWORK_GAMENETWORKPACKETS_GAMELIST_NETWORKPACKET 1

#include "EvilEngine/CoreLib/BasicTypes/BinaryStream/BinaryStream.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"
#include "EvilGameEngine/CoreBoardGame/Network/GameNetworkPackets/BaseGameNetworkPacket.h"
#include "EvilGameEngine/CoreBoardGame/GameLogic/Game/BoardGameServer.h"
#include <vector>

class BoardGameServer;
class BoardGame;

class GameListNetworkPacket : public BaseAuthentcatedNetworkPacket
{
public:

  GameListNetworkPacket();
  GameListNetworkPacket( const BinaryStream* datastream);
  GameListNetworkPacket( GameList& gameList );
  
  static  BaseNetworkPacket*    Create( const BinaryStream* datastream ){ return new GameListNetworkPacket( datastream ); }

  virtual BinaryStream*         GetDataStream()     DF_OVERRIDE;
  virtual String                GetPacketName()     DF_OVERRIDE { return StaticStr("GameList"); }
  virtual void                  Execute()           DF_OVERRIDE;
  virtual BoardGame*            CreateGameClass(const BinaryStream* datastream);
  virtual bool                  ReadDataStream(const BinaryStream* datastream) DF_OVERRIDE;

  static Signal1< void, std::vector< BoardGame* >& > s_SignalGameList;
  
private:

  BoardGameServer*              GetGameServer(CoreGameEngine& engine);

  GameList                      m_GameList;
};

#endif /// EVILENGINE_COREBOARDGAME_NETWORK_GAMENETWORKPACKETS_GAMELIST_NETWORKPACKET
