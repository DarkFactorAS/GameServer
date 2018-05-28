#ifndef PROJECT_WORLDBUILDERMODULE_NETWORKPACKETS_SERVERNETWORKPACKETS_LOADPLAYFIELD
#define PROJECT_WORLDBUILDERMODULE_NETWORKPACKETS_SERVERNETWORKPACKETS_LOADPLAYFIELD 1

#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/NetworkPackets/BaseWorldBuilderNetworkPacket.h"

class Playfield;

class ServerLoadPlayfieldNetworkPacket : public BaseWorldBuilderNetworkPacket
{
public:

  ServerLoadPlayfieldNetworkPacket(uint32 playfieldId);
  ServerLoadPlayfieldNetworkPacket(const BinaryStream* datastream);

  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new ServerLoadPlayfieldNetworkPacket(datastream); }

  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("ServerLoadPlayfield"); }
  virtual void                  Execute()         DF_OVERRIDE;
  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;

private:

  uint32                        m_PlayfieldId;
};

#endif /// PROJECT_WORLDBUILDERMODULE_NETWORKPACKETS_SERVERNETWORKPACKETS_LOADPLAYFIELD#pragma once
