#ifndef PROJECT_WORLDBUILDERMODULE_NETWORKPACKETS_CLIENTNETWORKPACKETS_SAVEPLAYFIELD
#define PROJECT_WORLDBUILDERMODULE_NETWORKPACKETS_CLIENTNETWORKPACKETS_SAVEPLAYFIELD 1

#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/Packets/BaseNetworkPacket.h"

class Playfield;

class ClientPlayfieldSavedNetworkPacket : public BaseNetworkPacket
{
public:

  ClientPlayfieldSavedNetworkPacket(const BinaryStream* datastream);
  ClientPlayfieldSavedNetworkPacket(uint32 playfieldSource, uint32 playfieldId, const String& playfieldTokenId);

  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new ClientPlayfieldSavedNetworkPacket(datastream); }

  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;
  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("ClientPlayfieldSaved"); }
  virtual void                  Execute()         DF_OVERRIDE;

private:

  uint32                        m_PlayfieldSource;
  uint32                        m_PlayfieldId;
  String                        m_PlayfieldTokenId;

};

#endif /// PROJECT_WORLDBUILDERMODULE_NETWORKPACKETS_CLIENTNETWORKPACKETS_SAVEPLAYFIELD#pragma once
