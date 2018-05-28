#ifndef EVILENGINE_COREBOARDGAME_NETWORK_PLAYFIELDNETWORKPACKETS_SAVEPLAYFIELD
#define EVILENGINE_COREBOARDGAME_NETWORK_PLAYFIELDNETWORKPACKETS_SAVEPLAYFIELD 1

#include "EvilEngine/CoreLib/BasicTypes/BinaryStream/BinaryStream.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"
#include "EvilGameEngine/CoreBoardGame/Network/GameNetworkPackets/BaseGameNetworkPacket.h"
//#include "EvilGameEngine/CoreBoardGame/GameLogic/Game/BoardGame.h"
#include "EvilGameEngine/CoreBoardGame/Playfield/GridTilePlayfield.h"

class SavePlayfieldNetworkPacket : public BaseAuthentcatedNetworkPacket
{
public:

#ifdef CLIENT

  SavePlayfieldNetworkPacket(GridTilePlayfield* playfield);

#else

  SavePlayfieldNetworkPacket(uint32 playfieldId);

#endif

  SavePlayfieldNetworkPacket(const BinaryStream* datastream);
  virtual BinaryStream*         GetDataStream()     DF_OVERRIDE;
  virtual String                GetPacketName()     DF_OVERRIDE { return StaticStr("SavePlayfield"); }
  virtual void                  Execute()           DF_OVERRIDE;
  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new SavePlayfieldNetworkPacket(datastream); }

private:

  uint32                        m_PlayfieldId;
  GridTilePlayfield*            m_Playfield;
};

#endif /// EVILENGINE_COREBOARDGAME_NETWORK_PLAYFIELDNETWORKPACKETS_SAVEPLAYFIELD