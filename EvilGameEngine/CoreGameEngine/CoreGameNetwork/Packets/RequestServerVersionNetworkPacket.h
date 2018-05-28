#pragma once

#include "BaseNetworkPacket.h"

class RequestServerVersionNetworkPacket : public BaseNetworkPacket
{
public:

  RequestServerVersionNetworkPacket( const BinaryStream* datastream);
  RequestServerVersionNetworkPacket();

  static  BaseNetworkPacket*    Create( const BinaryStream* datastream ){ return new RequestServerVersionNetworkPacket(datastream); }

  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("RequestServerVersion"); }
  virtual void                  Execute()         DF_OVERRIDE;

private:

  String m_ServerVersion;

};
