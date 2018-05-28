#ifndef EVILENGINE_COREBOARDGAME_NETWORK_GAMENETWORKPACKETS_SEARCHFORPLAYERWITHNICKNETWORKPACKET
#define EVILENGINE_COREBOARDGAME_NETWORK_GAMENETWORKPACKETS_SEARCHFORPLAYERWITHNICKNETWORKPACKET 1

#include "EvilEngine/CoreLib/BasicTypes/BinaryStream/BinaryStream.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"
#include "EvilGameEngine/CoreBoardGame/GameLogic/Game/BoardGameServer.h"
#include "EvilGameEngine/CoreGameLogin/Network/BaseAuthentcatedNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/Account/Account.h"

class SearchForPlayerWithNickNetworkPacket : public BaseAuthentcatedNetworkPacket
{
public:

#ifdef CLIENT

  SearchForPlayerWithNickNetworkPacket( const String& playerNick );
  static Signal1< void, std::vector< Account* > > SignalReceivedPlayerSearchResult;

#elif defined SERVER

  SearchForPlayerWithNickNetworkPacket( std::vector< Account* > accountList );

#endif

  SearchForPlayerWithNickNetworkPacket( const BinaryStream* datastream);
  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new SearchForPlayerWithNickNetworkPacket(datastream); }
  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("SearchForPlayerWithNick"); }
  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;
  virtual void                  Execute()         DF_OVERRIDE;

private:
  String                        m_PlayerNick;
  std::vector< Account* >       m_SearchResult;
};

#endif /// EVILENGINE_COREBOARDGAME_NETWORK_GAMENETWORKPACKETS_SEARCHFORPLAYERWITHNICKNETWORKPACKET