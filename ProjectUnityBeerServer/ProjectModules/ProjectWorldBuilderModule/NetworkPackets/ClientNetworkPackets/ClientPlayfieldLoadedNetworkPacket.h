#ifndef PROJECT_WORLDBUILDERMODULE_NETWORKPACKETS_CLIENTNETWORKPACKETS_LOADPLAYFIELD
#define PROJECT_WORLDBUILDERMODULE_NETWORKPACKETS_CLIENTNETWORKPACKETS_LOADPLAYFIELD 1

#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/Packets/BaseNetworkPacket.h"

class Playfield;

class ClientPlayfieldLoadedNetworkPacket : public BaseNetworkPacket
{
public:

  ClientPlayfieldLoadedNetworkPacket(const BinaryStream* datastream);
  ClientPlayfieldLoadedNetworkPacket(Playfield* playfield);

  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new ClientPlayfieldLoadedNetworkPacket(datastream); }

  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;
  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("ClientPlayfieldLoaded"); }
  virtual void                  Execute()         DF_OVERRIDE;

private:

  Playfield*                    m_Playfield;

};

#endif /// PROJECT_WORLDBUILDERMODULE_NETWORKPACKETS_CLIENTNETWORKPACKETS_LOADPLAYFIELD#pragma once
