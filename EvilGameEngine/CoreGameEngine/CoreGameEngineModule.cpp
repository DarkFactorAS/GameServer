#include "Precompile.h"
#include "EvilEngine/CoreIO/Logger/Logger.h"
#include "CoreGameEngineModule.h"
#include "EvilEngine/CoreNetwork/Common/NetworkConnection.h"

void CoreGameEngineModule::RegisterPacketType(uint32 packetTypeId, NetworkPacket_fp* createInstance)
{
  m_PacketCreator[packetTypeId] = createInstance;
}

void CoreGameEngineModule::DisconnectConnectionId(uint32 connectionId)
{
  CoreGameEngine* gameEngine = safe_cast<CoreGameEngine*> (GetEngine());
  if (gameEngine != NULL)
  {
    NetworkBase* network = gameEngine->GetNetwork();
    if (network != NULL)
    {
      NetworkConnection* connection = network->GetConnection(connectionId);
      if (connection != NULL)
      {
        connection->Disconnect();
      }
    }
  }
}

bool CoreGameEngineModule::SendPacketToClientConnection(uint32 connectionId, BaseNetworkPacket* packet)
{
  CoreGameEngine* gameEngine = safe_cast<CoreGameEngine*> (GetEngine());
  if (gameEngine != NULL)
  {
    return gameEngine->SendPacketToEndpoint(connectionId, packet);
  }
  return false;
}

bool CoreGameEngineModule::ReceivePacket(uint32 packetTypeId, uint32 connectionInstance, const BinaryStream* dataStream)
{
  std::map< uint32, NetworkPacket_fp* >::iterator itFind = m_PacketCreator.find(packetTypeId);
  if (itFind != m_PacketCreator.end())
  {
    BaseNetworkPacket* packet = itFind->second(dataStream);
    if (packet != NULL)
    {
      if (packet->ReadDataStream(dataStream))
      {
        // TODO : Change this to module instead
        CoreGameEngine* gameEngine = safe_cast< CoreGameEngine* > ( GetEngine() );
        packet->SetGameEngine(gameEngine);

        packet->SetGameEngineModule( this );
        packet->SetConnectionId(connectionInstance);

        // Handle packet right away. TODO. queue it for the right thread.
        if (packet->CanExecute())
        {
          packet->Execute();
        }
      }
      else
      {
        LogFatalFMT("CoreGameEngineModule", "Failed to read binarystream for packet %d/%s", packetTypeId, packet->GetPacketName().c_str());
      }

      delete packet;
      return true;
    }
  }
  return false;
}
