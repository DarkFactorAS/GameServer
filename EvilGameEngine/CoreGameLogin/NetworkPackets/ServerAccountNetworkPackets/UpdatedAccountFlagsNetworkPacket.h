#pragma once

#include "EvilGameEngine/CoreGameLogin/Network/BaseAuthentcatedNetworkPacket.h"

class UpdatedAccountFlagsNetworkPacket : public BaseAuthentcatedNetworkPacket
{
public:

  UpdatedAccountFlagsNetworkPacket( const BinaryStream* datastream);
  UpdatedAccountFlagsNetworkPacket( uint32 accountFlags);

  static  BaseNetworkPacket*    Create( const BinaryStream* datastream ){ return new UpdatedAccountFlagsNetworkPacket( datastream ); }

  virtual BinaryStream*         GetDataStream()       DF_OVERRIDE;
  virtual String                GetPacketName()       DF_OVERRIDE { return StaticStr("UpdatedAccountFlags"); }
  virtual void                  Execute()             DF_OVERRIDE;

private:

  uint32 m_AccountFlags;

};
