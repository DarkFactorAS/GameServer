#ifndef PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_CLIENTGAMEERRORNETWORKPACKET
#define PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_CLIENTGAMEERRORNETWORKPACKET 1

#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/Packets/BaseNetworkPacket.h"

class Account;

class ClientGameErrorNetworkPacket : public BaseNetworkPacket
{
public:

  ClientGameErrorNetworkPacket(const BinaryStream* datastream);
  ClientGameErrorNetworkPacket(uint32 packetType, uint32 errorCode, const String& message);

  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new ClientGameErrorNetworkPacket(datastream); }

  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;
  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("ClientGameErrorNetwork"); }
  virtual void                  Execute()         DF_OVERRIDE;

private:

  uint32                        m_PacketType;
  uint32                        m_ErrorCode;
  String                        m_Message;

};

#endif /// PROJECT_GAMEMANAMANGEMENT_NETWORKPACKETS_CLIENTGAMEERRORNETWORKPACKET#pragma once
