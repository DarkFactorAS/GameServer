
#include "Precompile.h"
#include "BaseNetworkPacket.h"

#include "EvilEngine/CoreVersion/version.h"
#include "EvilEngine/CoreNetwork/Common/NetworkConnection.h"
#include "EvilEngine/CoreIO/Logger/Logger.h"

#include "EvilGameEngine/CoreGameEngine/CoreGameEngineModule.h"

BaseNetworkPacket::BaseNetworkPacket( uint32 packetType, const BinaryStream* /*datastream*/ ) :
  m_PacketType(packetType),
  m_ConnectionId(0),
  m_GameEngine(NULL)
{
}

BaseNetworkPacket::BaseNetworkPacket( uint32 packetType) :
  m_PacketType( packetType ),
  m_ConnectionId(0),
  m_GameEngine(NULL)
{
}

BinaryStream* BaseNetworkPacket::GetDataStream()
{
  BinaryStream* binaryStream = new BinaryStream();
  binaryStream->WriteUInt32( m_PacketType );
  return binaryStream;
}

void BaseNetworkPacket::Execute()
{
  dfBugreportFMT( false, "BaseNetworkPacket::Execute called but not implemented for packetType %d!", m_PacketType );
}
//void BaseNetworkPacket::Execute(CoreGameEngine& /*engine*/, uint32 /*networkInstance*/)
//{
//  dfBugreportFMT(false, "BaseNetworkPacket::Execute called but not implemented for packetType %d!", m_PacketType);
//}

EngineModule* BaseNetworkPacket::GetEngineModule(int moduleId)
{
  if ( m_GameEngine != NULL )
  {
    return m_GameEngine->GetEngineModule(moduleId);
  }
  return NULL;
}


void BaseNetworkPacket::SendErrorToClient(uint32 errorId )
{
  CoreGameEngine* gameEngine = GetGameEngine();
  if (gameEngine != NULL )
  {
    gameEngine->AddErrorPacket(m_ConnectionId, m_PacketType, errorId);
  }
}

void BaseNetworkPacket::SendPacketToClient(BaseNetworkPacket* packet)
{
  if (m_GameEngineModule != NULL && packet != NULL )
  {
    if (!m_GameEngineModule->SendPacketToClient(m_ConnectionId, packet))
    {
      LogFatalFMT("BaseNetworkPacket", "Failed to send packet [%s] to client. Code error", packet->GetPacketName().c_str());
    }
  }
}

void BaseNetworkPacket::SendPacketToServer(CoreGameEngine& /*engine*/, BaseNetworkPacket* packet)
{
  SendPacketToServer(packet);
}

void BaseNetworkPacket::SendPacketToServer(BaseNetworkPacket* packet)
{
  CoreGameEngine* gameEngine = GetGameEngine();
  if (gameEngine != NULL && packet != NULL )
  {
    if (!gameEngine->SendPacketToServer(packet))
    {
      LogFatalFMT("BaseNetworkPacket", "Failed to send packet [%s] to endpoint. Code error", packet->GetPacketName().c_str());
    }
  }
}

String BaseNetworkPacket::GetIpAddress()
{
  CoreGameEngine* gameEngine = GetGameEngine();
  if (gameEngine != NULL)
  {
    NetworkBase* networkBase = gameEngine->GetNetwork();
    if (networkBase != NULL)
    {
      NetworkConnection* connection = networkBase->GetConnection(m_ConnectionId);
      if (connection != NULL)
      {
        return connection->IpAddress();
      }
    }
  }
  return String::zero;
}
