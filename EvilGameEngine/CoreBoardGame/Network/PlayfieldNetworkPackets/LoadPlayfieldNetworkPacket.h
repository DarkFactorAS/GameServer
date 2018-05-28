#ifndef EVILENGINE_COREBOARDGAME_NETWORK_PLAYFIELDNETWORKPACKETS_LOADPLAYFIELD
#define EVILENGINE_COREBOARDGAME_NETWORK_PLAYFIELDNETWORKPACKETS_LOADPLAYFIELD 1

#include "EvilEngine/CoreLib/BasicTypes/BinaryStream/BinaryStream.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"
#include "EvilGameEngine/CoreBoardGame/Network/GameNetworkPackets/BaseGameNetworkPacket.h"
//#include "EvilGameEngine/CoreBoardGame/GameLogic/Game/BoardGame.h"
#include "EvilGameEngine/CoreBoardGame/Playfield/GridTilePlayfield.h"

class LoadPlayfieldNetworkPacket : public BaseAuthentcatedNetworkPacket
{
public:

#ifdef CLIENT

  LoadPlayfieldNetworkPacket(uint32 playfieldId);

#else

  LoadPlayfieldNetworkPacket(GridTilePlayfield* playfield);

#endif

  LoadPlayfieldNetworkPacket(const BinaryStream* datastream);
  virtual BinaryStream*         GetDataStream()     DF_OVERRIDE;
  virtual String                GetPacketName()     DF_OVERRIDE { return StaticStr("LoadPlayfield"); }
  virtual void                  Execute()           DF_OVERRIDE;
  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new LoadPlayfieldNetworkPacket(datastream); }

private:

  uint32                        m_PlayfieldId;
  GridTilePlayfield*            m_Playfield;
};

#endif /// EVILENGINE_COREBOARDGAME_NETWORK_PLAYFIELDNETWORKPACKETS_LOADPLAYFIELD