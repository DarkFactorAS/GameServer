#ifndef PROJECT_WORLDBUILDERMODULE_NETWORKPACKETS_SERVERNETWORKPACKETS_SAVEPLAYFIELD
#define PROJECT_WORLDBUILDERMODULE_NETWORKPACKETS_SERVERNETWORKPACKETS_SAVEPLAYFIELD 1

#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/NetworkPackets/BaseWorldBuilderNetworkPacket.h"

class Playfield;

class ServerSavePlayfieldNetworkPacket : public BaseWorldBuilderNetworkPacket
{
public:

  ServerSavePlayfieldNetworkPacket(Playfield* playfield);
  ServerSavePlayfieldNetworkPacket(const BinaryStream* datastream);

  static  BaseNetworkPacket*    Create(const BinaryStream* datastream) { return new ServerSavePlayfieldNetworkPacket(datastream); }

  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("ServerSavePlayfield"); }
  virtual void                  Execute()         DF_OVERRIDE;
  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;

private:

  BinaryStream*                 m_Data;
  Playfield*                    m_Playfield;
};

#endif /// PROJECT_WORLDBUILDERMODULE_NETWORKPACKETS_SERVERNETWORKPACKETS_SAVEPLAYFIELD#pragma once
