#ifndef PROJECT_WORLDBUILDERMODULE_NETWORKPACKETS_SERVERNETWORKPACKETS_SENDPLAYFIELDLIST
#define PROJECT_WORLDBUILDERMODULE_NETWORKPACKETS_SERVERNETWORKPACKETS_SENDPLAYFIELDLIST 1

#include "ProjectWorldBuilderModule/NetworkPackets/BaseWorldBuilderNetworkPacket.h"

class Playfield;

class ServerSendPlayfieldListNetworkPacket : public BaseWorldBuilderNetworkPacket
{
public:

  ServerSendPlayfieldListNetworkPacket(const BinaryStream* datastream);

  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new ServerSendPlayfieldListNetworkPacket(datastream); }

  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("ServerSendPlayfieldList"); }
  virtual void                  Execute()         DF_OVERRIDE;

private:

  uint32                        m_Flags;
};

#endif /// PROJECT_WORLDBUILDERMODULE_NETWORKPACKETS_SERVERNETWORKPACKETS_SENDPLAYFIELDLIST#pragma once
