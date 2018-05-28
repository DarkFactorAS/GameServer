#ifndef EVILENGINE_CORENETWORK_UNITTESTFRAMEWORK_UNITTESTNETWORKCONNECTION
#define EVILENGINE_CORENETWORK_UNITTESTFRAMEWORK_UNITTESTNETWORKCONNECTION

#include "../Common/NetworkBase.h"
#include "../Common/NetworkConnection.h"

class UnittestNetworkConnection : public NetworkConnection
{
public:
  UnittestNetworkConnection(NetworkBase* parent) : NetworkConnection(parent)
  {
  }

protected:
  virtual void HandleRead(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/, NetworkConnection* /*connection*/) DF_OVERRIDE
  {
  }

  virtual void HandleWrite(const boost::system::error_code& /*errorCode*/, size_t /*bytesWritten*/, NetworkConnection* /*connection*/) DF_OVERRIDE
  {
  }

};

#endif /// EVILENGINE_CORENETWORK_UNITTESTFRAMEWORK_UNITTESTNETWORKCONNECTION
