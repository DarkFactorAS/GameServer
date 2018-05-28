#ifndef EVILENGINE_COREBOARDGAME_NETWORK_PLAYFIELDNETWORKPACKETS_PLAYFIELDLIST
#define EVILENGINE_COREBOARDGAME_NETWORK_PLAYFIELDNETWORKPACKETS_PLAYFIELDLIST 1

#include "EvilEngine/CoreLib/BasicTypes/BinaryStream/BinaryStream.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"
#include "EvilGameEngine/CoreBoardGame/Network/GameNetworkPackets/BaseGameNetworkPacket.h"
#include "EvilGameEngine/CoreBoardGame/Playfield/GridTilePlayfield.h"

class PlayfieldListNetworkPacket : public BaseAuthentcatedNetworkPacket
{
public:

#ifdef CLIENT
  PlayfieldListNetworkPacket();
  PlayfieldListNetworkPacket(const BinaryStream* datastream);
#else
  PlayfieldListNetworkPacket(const BinaryStream* datastream);
  PlayfieldListNetworkPacket(PlayfieldInfoMap playfieldList);
  virtual BinaryStream*         GetDataStream()                                         DF_OVERRIDE;
#endif

  virtual void                  Execute()           DF_OVERRIDE;
  virtual String                GetPacketName()     DF_OVERRIDE { return StaticStr("PlayfieldList"); }
  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new PlayfieldListNetworkPacket(datastream); }

  static Signal1< void, PlayfieldInfoMap /* plauyfieldList */ > s_SignalPlayfieldList;
  
private:

  PlayfieldInfoMap              m_PlayfieldList;
};

#endif /// EVILENGINE_COREBOARDGAME_NETWORK_PLAYFIELDNETWORKPACKETS_PLAYFIELDLIST
