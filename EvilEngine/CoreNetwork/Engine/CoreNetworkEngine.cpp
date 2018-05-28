
#include "Precompile.h"
#include "CoreNetworkEngine.h"
#include "../Common/NetworkConnection.h"

CoreNetworkEngine::CoreNetworkEngine() :
  m_NetworkBase(NULL)
{
}

bool CoreNetworkEngine::Run()
{
  if ( m_NetworkBase == NULL )
  {
    LogFatal("CoreNetworkEngine", "Networkbase not set!");
    return false;
  }

  std::vector< NetworkBase::MessageData > networkPackets = m_NetworkBase->PopQueuedPackets();
  for (size_t index = 0; index < networkPackets.size(); index++)
  {
    NetworkBase::MessageData& data = networkPackets[index];
    switch (data.m_MessageId)
    {
      case NetworkBase::NETWORKPACKETTYPE_DATA:
        {
          if (data.m_Data != NULL && data.m_Data->IsOK())
          {
            OnReceivedNetworkData(data.m_ConnectionId, data.m_Data);
          }
          else
          {
            LogError("PacketNetworkBase", "Datastream packet was not OK!");
          }
        }
        break;

      case NetworkBase::NETWORKPACKETTYPE_DATAPACKET:
        {
          if (data.m_Data != NULL && data.m_Data->IsOK())
          {
            OnReceivedNetworkPacket(data.m_ConnectionId, data.m_Data);
          }
          else
          {
            LogError("PacketNetworkBase", "Datastream packet was not OK!");
          }
        }
        break;

      case NetworkBase::NETWORKPACKETTYPE_DISCONNECTED:
        OnConnectionChanged( data.m_ConnectionId, NetworkConnection::NETWORKSTATUS_DISCONNECTED );
        break;
      case NetworkBase::NETWORKPACKETTYPE_CONNECTED:
        OnConnectionChanged( data.m_ConnectionId, NetworkConnection::NETWORKSTATUS_CONNECTED );
        break;
      case NetworkBase::NETWORKPACKETTYPE_FAILED:
        OnConnectionChanged( data.m_ConnectionId, NetworkConnection::NETWORKSTATUS_FAILED );
        break;
      case NetworkBase::NETWORKPACKETTYPE_CONNECTING:
        OnConnectionChanged( data.m_ConnectionId, NetworkConnection::NETWORKSTATUS_CONNECTING );
        break;
      case NetworkBase::NETWORKPACKETTYPE_RESOLVINGHOSTNAME:
        OnConnectionChanged( data.m_ConnectionId, NetworkConnection::NETWORKSTATUS_RESOLVINGHOSTNAME );
        break;
      default:
        break;
    }

    delete data.m_Data;
    data.m_Data = NULL;
  }

  SendNetworkPackets();

  return true;
}

bool CoreNetworkEngine::SendPacket(uint32 connectionId, BinaryStream* data)
{
  if (data != NULL)
  {
    m_SendQueue.push_back(std::pair< uint32, BinaryStream* >(connectionId, data));
    return true;
  }
  return false;
}

void CoreNetworkEngine::SendNetworkPackets()
{
  if ( !m_SendQueue.empty() )
  {
    std::vector < std::pair< uint32, BinaryStream* > > sendList;
    std::swap(sendList, m_SendQueue);

    //m_WriteMutex.Aquire();
    for (std::vector< std::pair< uint32, BinaryStream* > >::const_iterator itData = sendList.begin(); itData != sendList.end(); itData++)
    {
      uint32 receiverId = itData->first;
      BinaryStream* data = itData->second;

      BinaryStream* fullStream = m_NetworkBase->CreateBinaryStream();
      fullStream->WriteRaw( data );

      NetworkConnection* connection = m_NetworkBase->GetConnection(receiverId);
      if (connection != NULL)
      {
        m_NetworkBase->SendData( connection, fullStream );
      }
      else
      {
        LogInfoFMT("NetworkEngine", "FrameProcess: Could not send packet[%d bytes] to reciever[%d] - No connection to client", data->GetSize(), receiverId);
      }
    }
    m_SendQueue.clear();
    //m_WriteMutex.Release();
  }
}