#ifndef PROJECT_WORLDBUILDERMODULE_NETWORKPACKETS_SERVERNETWORKPACKETS_DELETEPLAYFIELD
#define PROJECT_WORLDBUILDERMODULE_NETWORKPACKETS_SERVERNETWORKPACKETS_DELETEPLAYFIELD 1

#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/NetworkPackets/BaseWorldBuilderNetworkPacket.h"

class ServerDeletePlayfieldNetworkPacket : public BaseWorldBuilderNetworkPacket
{
public:

  ServerDeletePlayfieldNetworkPacket(uint32 playfieldId);
  ServerDeletePlayfieldNetworkPacket(const BinaryStream* datastream);

  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new ServerDeletePlayfieldNetworkPacket(datastream); }

  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("ServerDeletePlayfield"); }
  virtual void                  Execute()         DF_OVERRIDE;
  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;

private:

  uint32                        m_PlayfieldId;
};

#endif /// PROJECT_WORLDBUILDERMODULE_NETWORKPACKETS_SERVERNETWORKPACKETS_DELETEPLAYFIELD#pragma once
