#ifndef EVILENGINE_CORENETWORK_UNITTESTFRAMEWORK_UNITTESTNETWORKCLIENT
#define EVILENGINE_CORENETWORK_UNITTESTFRAMEWORK_UNITTESTNETWORKCLIENT

#include "../Common/NetworkBase.h"
#include "EvilEngine/CoreLib/Signal/SignalTarget.h"
#include "EvilEngine/CoreNetwork/Client/NetworkClient.h"
#include "EvilEngine/CoreNetwork/Common/NetworkConnection.h"

#include "UnittestNetworkConnection.hpp"

#include <stdlib.h>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

class BinaryStream;

class UnitTestNetworkClient : public NetworkClient
{
public:

  Signal1<void, const BinaryStream* /*dataStream*/> SignalSendData;

  UnitTestNetworkClient(const String& threadName, bool usePackets) : NetworkClient(threadName, usePackets)
  {
  }

  virtual NetworkConnection* CreateConnectionObject() DF_OVERRIDE
  {
    UnittestNetworkConnection* newConnection = new UnittestNetworkConnection(this);
    return newConnection;
  }

  virtual bool                    ConnectAsync() DF_OVERRIDE
  {
    LogDebug("UT-NetworkClient", "ConnectAsync");

    m_Connection = CreateConnectionObject();
    m_Connection->SetNetworkStatus(NetworkConnection::NETWORKSTATUS_CONNECTED);
    m_Connection->OnConnected();
    return true;
  }

  void QueueData(const BinaryStream* data)
  {
    LogDebug("UT-NetworkClient", "QueueData");
    NetworkClient::QueueData(m_Connection, data);
  }

  virtual bool SendData(NetworkConnection* /*connection*/, BinaryStream* data) DF_OVERRIDE
  {
    LogDebug("UT-NetworkClient", "SendData");
    if (m_UsePackets)
    {
      data->WriteUInt32(0, data->GetSize() - 4);
    }
    else
    {
      return false;
    }

    SignalSendData(data);
    return true;
  }
};

#endif /// EVILENGINE_CORENETWORK_UNITTESTFRAMEWORK_UNITTESTNETWORKCLIENT