#ifndef EVILENGINE_CORENETWORK_UNITTESTFRAMEWORK_UNITTESTNETWORKSERVER
#define EVILENGINE_CORENETWORK_UNITTESTFRAMEWORK_UNITTESTNETWORKSERVER

#include "../Common/NetworkBase.h"
#include "EvilEngine/CoreLib/Signal/SignalTarget.h"
#include "EvilEngine/CoreNetwork/Server/NetworkServer.h"
#include "EvilEngine/CoreNetwork/Common/NetworkConnection.h"

#include "UnittestNetworkConnection.hpp"

#include <stdlib.h>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

class BinaryStream;

class UnitTestNetworkServer : public NetworkServer
{
public:

  uint32 m_NetworkInstance;

  Signal1<void, const BinaryStream* /*dataStream*/> SignalSendData;

  UnitTestNetworkServer(const String& threadName, bool usePackets) : NetworkServer(threadName, 0, usePackets)
  {
    m_NetworkInstance = 0;
  }

  virtual NetworkConnection* CreateConnectionObject() DF_OVERRIDE
  {
    UnittestNetworkConnection* newConnection = new UnittestNetworkConnection(this);
    return newConnection;
  }

  bool IsRunning() const DF_OVERRIDE
  {
    return true;
  }

  uint32 GetConnectionInstance()
  {
    return m_NetworkInstance;
  }

  virtual bool Start() DF_OVERRIDE
  {
    LogDebug("UT-NetworkServer", "Start");

    NetworkConnection* connection = CreateConnectionObject();
    m_NetworkInstance = connection->GetInstance();
    boost::system::error_code ec;
    HandleAccept(ec, connection);
    Run();

    return true;
  }

  virtual void                    StartAccept() DF_OVERRIDE
  {
    LogDebug("UT-NetworkServer", "StartAccept");
  }

  void QueueData(const BinaryStream* data)
  {
    LogDebugFMT("UT-NetworkServer", "Queue data for instance %d", m_NetworkInstance);
    NetworkConnection* connection = GetConnection(m_NetworkInstance);
    NetworkServer::QueueData(connection,data);
  }

  virtual bool                    SendData(NetworkConnection* /*connection*/, BinaryStream* data) DF_OVERRIDE
  {
    if (m_UsePackets)
    {
      LogDebug("UT-NetworkServer", "SendData");
      data->WriteUInt32(0, data->GetSize() - 4);
    }
    else
    {
      LogDebug("UT-NetworkServer", "SendData failed => Not packet");
      return false;
    }

    SignalSendData(data);
    return true;
  }


};

#endif /// EVILENGINE_CORENETWORK_UNITTESTFRAMEWORK_UNITTESTNETWORKSERVER