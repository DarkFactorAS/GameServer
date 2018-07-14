#pragma once

#include "BaseNetworkPacket.h"

class ServerErrorNetworkPacket : public BaseNetworkPacket
{
public:

  ServerErrorNetworkPacket( const BinaryStream* datastream);
  ServerErrorNetworkPacket( uint32 packetTypeId, uint32 errorCodeId, const String errorMessage);
    
  static  BaseNetworkPacket*    Create( const BinaryStream* datastream ){ return new ServerErrorNetworkPacket( datastream ); }
    
  virtual BinaryStream*         GetDataStream()       DF_OVERRIDE;
  virtual String                GetPacketName()       DF_OVERRIDE { return StaticStr("ServerError"); }
  virtual void                  Execute()             DF_OVERRIDE;
  
  static Signal2< void, uint32, uint32 > s_SignalPacketError;

private:

  uint32 m_PacketTypeId;
  uint32 m_ErrorCodeId;
  String m_ErrorMessage;

};
