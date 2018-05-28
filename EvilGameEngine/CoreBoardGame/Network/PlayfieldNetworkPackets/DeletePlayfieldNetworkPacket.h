#ifndef EVILENGINE_COREBOARDGAME_NETWORK_PLAYFIELDNETWORKPACKETS_DELETEPLAYFIELD
#define EVILENGINE_COREBOARDGAME_NETWORK_PLAYFIELDNETWORKPACKETS_DELETEPLAYFIELD 1

#include "EvilEngine/CoreLib/BasicTypes/BinaryStream/BinaryStream.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"
#include "EvilGameEngine/CoreBoardGame/Network/GameNetworkPackets/BaseGameNetworkPacket.h"
//#include "EvilGameEngine/CoreBoardGame/GameLogic/Game/BoardGame.h"
#include "EvilGameEngine/CoreBoardGame/Playfield/GridTilePlayfield.h"

class DeletePlayfieldNetworkPacket : public BaseAuthentcatedNetworkPacket
{
public:

  DeletePlayfieldNetworkPacket(uint32 playfieldId);
  DeletePlayfieldNetworkPacket(const BinaryStream* datastream);

  virtual BinaryStream*         GetDataStream()     DF_OVERRIDE;
  virtual String                GetPacketName()     DF_OVERRIDE { return StaticStr("DeletePlayfield"); }
  virtual void                  Execute()           DF_OVERRIDE;
  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new DeletePlayfieldNetworkPacket(datastream); }

private:

  uint32                        m_PlayfieldId;
};

#endif /// EVILENGINE_COREBOARDGAME_NETWORK_PLAYFIELDNETWORKPACKETS_DELETEPLAYFIELDD