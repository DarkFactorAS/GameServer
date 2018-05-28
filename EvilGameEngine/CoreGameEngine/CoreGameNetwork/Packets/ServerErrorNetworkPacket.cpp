
#include "Precompile.h"
#include "ServerErrorNetworkPacket.h"
#include "EvilEngine/CoreLib/BasicTypes/Definitions/VolatileArgs.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/GameEnginePacketData.h"

Signal2< void, uint32, uint32 > ServerErrorNetworkPacket::s_SignalPacketError;

ServerErrorNetworkPacket::ServerErrorNetworkPacket( const BinaryStream* datastream) :
  BaseNetworkPacket( GameEnginePacketData::PacketData_ErrorMessage, datastream )
{
  m_PacketTypeId = datastream->ReadUInt32();
  m_ErrorCodeId  = datastream->ReadUInt32();
}

ServerErrorNetworkPacket::ServerErrorNetworkPacket(uint32 packetTypeId, uint32 errorCodeId) :
  BaseNetworkPacket(GameEnginePacketData::PacketData_ErrorMessage),
  m_PacketTypeId( packetTypeId ),
  m_ErrorCodeId( errorCodeId )
{
}

BinaryStream* ServerErrorNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseNetworkPacket::GetDataStream();
  datastream->WriteUInt32( m_PacketTypeId );
  datastream->WriteUInt32( m_ErrorCodeId );
  return datastream;
}

void ServerErrorNetworkPacket::Execute()
{
#ifdef SERVER
  SendErrorToClient( m_ErrorCodeId );
#else
  s_SignalPacketError( m_PacketTypeId, m_ErrorCodeId );
#endif
}
