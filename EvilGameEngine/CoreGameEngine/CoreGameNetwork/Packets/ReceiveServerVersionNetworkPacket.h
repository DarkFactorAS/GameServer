#ifndef EVILGAMEENGINE_COREGAMEENGINE_NETWORKPACKETS_RECEIVEDSERVERVERSION_NETWORKPACKET
#define EVILGAMEENGINE_COREGAMEENGINE_NETWORKPACKETS_RECEIVEDSERVERVERSION_NETWORKPACKET

#pragma once

#include "BaseNetworkPacket.h"

class ReceivedServerVersionNetworkPacket : public BaseNetworkPacket
{
public:

  ReceivedServerVersionNetworkPacket(uint32 buildVersion, String buildDate);
  ReceivedServerVersionNetworkPacket( const BinaryStream* datastream);

  static  BaseNetworkPacket*    Create( const BinaryStream* datastream ){ return new ReceivedServerVersionNetworkPacket(datastream); }

  virtual BinaryStream*         GetDataStream()     DF_OVERRIDE;
  virtual String                GetPacketName()     DF_OVERRIDE { return StaticStr("ReceivedServerVersion"); }
  virtual void                  Execute()           DF_OVERRIDE;

private:

  uint32  m_BuildVersion;
  String  m_BuildDate;

};

#endif /// EVILGAMEENGINE_COREGAMEENGINE_NETWORKPACKETS_RECEIVEDSERVERVERSION_NETWORKPACKET