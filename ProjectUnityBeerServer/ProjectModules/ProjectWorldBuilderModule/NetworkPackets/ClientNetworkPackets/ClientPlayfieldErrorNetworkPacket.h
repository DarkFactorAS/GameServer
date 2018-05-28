#ifndef PROJECT_WORLDBUILDERMODULE_NETWORKPACKETS_CLIENTNETWORKPACKETS_PLAYFIELDERROR
#define PROJECT_WORLDBUILDERMODULE_NETWORKPACKETS_CLIENTNETWORKPACKETS_PLAYFIELDERROR 1

#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/Packets/BaseNetworkPacket.h"

class Account;

class ClientPlayfieldErrorNetworkPacket : public BaseNetworkPacket
{
public:

  ClientPlayfieldErrorNetworkPacket(const BinaryStream* datastream);
  ClientPlayfieldErrorNetworkPacket(uint32 packetType, uint32 errorCode, const String& message);

  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new ClientPlayfieldErrorNetworkPacket(datastream); }

  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;
  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("ClientPlayfieldError"); }
  virtual void                  Execute()         DF_OVERRIDE;

private:

  uint32                        m_PacketType;
  uint32                        m_ErrorCode;
  String                        m_Message;

};

#endif /// PROJECT_WORLDBUILDERMODULE_NETWORKPACKETS_CLIENTNETWORKPACKETS_PLAYFIELDERROR#pragma once
