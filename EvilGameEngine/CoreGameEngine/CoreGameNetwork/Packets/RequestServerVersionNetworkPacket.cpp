
#include "Precompile.h"
#include "RequestServerVersionNetworkPacket.h"
#include "ReceiveServerVersionNetworkPacket.h"
#include "EvilEngine/CoreVersion/version.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/GameEnginePacketData.h"

RequestServerVersionNetworkPacket::RequestServerVersionNetworkPacket( const BinaryStream* datastream) :
  BaseNetworkPacket( GameEnginePacketData::PacketData_RequestServerVersion, datastream )
{
}

RequestServerVersionNetworkPacket::RequestServerVersionNetworkPacket() :
  BaseNetworkPacket(GameEnginePacketData::PacketData_RequestServerVersion)
{
}

void RequestServerVersionNetworkPacket::Execute()
{
#ifdef SERVER
  SendPacketToClient( new ReceivedServerVersionNetworkPacket( BUILD_REVISION, BUILD_DATE ) );
#endif
}
