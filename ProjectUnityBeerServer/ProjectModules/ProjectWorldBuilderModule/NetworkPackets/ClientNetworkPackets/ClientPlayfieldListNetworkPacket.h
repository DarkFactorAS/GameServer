#ifndef PROJECT_WORLDBUILDERMODULE_NETWORKPACKETS_CLIENTNETWORKPACKETS_LISTPLAYFIELD
#define PROJECT_WORLDBUILDERMODULE_NETWORKPACKETS_CLIENTNETWORKPACKETS_LISTPLAYFIELD 1

#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/Packets/BaseNetworkPacket.h"

class Playfield;

class ClientPlayfieldListNetworkPacket : public BaseNetworkPacket
{
public:

  ClientPlayfieldListNetworkPacket(const BinaryStream* datastream);
  ClientPlayfieldListNetworkPacket( std::vector< Playfield* > playfieldList );

  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new ClientPlayfieldListNetworkPacket(datastream); }

  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;
  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("ClientPlayfieldList"); }
  virtual void                  Execute()         DF_OVERRIDE;

private:

  std::vector< Playfield* >     m_PlayfieldList;

};

#endif /// PROJECT_WORLDBUILDERMODULE_NETWORKPACKETS_CLIENTNETWORKPACKETS_LISTPLAYFIELD#pragma once
