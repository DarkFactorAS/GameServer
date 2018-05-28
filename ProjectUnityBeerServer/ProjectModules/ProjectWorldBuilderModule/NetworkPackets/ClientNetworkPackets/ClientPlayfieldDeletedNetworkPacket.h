#ifndef PROJECT_WORLDBUILDERMODULE_NETWORKPACKETS_CLIENTNETWORKPACKETS_PLAYFIELDDELETED
#define PROJECT_WORLDBUILDERMODULE_NETWORKPACKETS_CLIENTNETWORKPACKETS_PLAYFIELDDELETED 1

#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/Packets/BaseNetworkPacket.h"

class Playfield;

class ClientPlayfieldDeletedNetworkPacket : public BaseNetworkPacket
{
public:

  ClientPlayfieldDeletedNetworkPacket(const BinaryStream* datastream);
  ClientPlayfieldDeletedNetworkPacket(uint32 playfieldSource, uint32 playfieldId, const String& playfieldTokenId);

  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new ClientPlayfieldDeletedNetworkPacket(datastream); }

  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;
  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("ClientPlayfieldDeleted"); }
  virtual void                  Execute()         DF_OVERRIDE;

private:

  uint32                        m_PlayfieldSource;
  uint32                        m_PlayfieldId;
  String                        m_PlayfieldTokenId;

};

#endif /// PROJECT_WORLDBUILDERMODULE_NETWORKPACKETS_CLIENTNETWORKPACKETS_PLAYFIELDDELETED#pragma once
