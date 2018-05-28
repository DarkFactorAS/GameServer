
#ifndef EVILENGINE_CORENETWORK_EMAILNETWORKCLIENT
#define EVILENGINE_CORENETWORK_EMAILNETWORKCLIENT

#include "../Common/NetworkBase.h"
#include "../Client/NetworkClient.h"

class NetworkConnection;
class BinaryStream;

class EmailNetworkClient : public NetworkClient
{
public:

  EmailNetworkClient( const String& threadName, const String& serverAddress, uint16 serverPort );

  void SendEmail( String receiver, String sender, String subject, String message );

  virtual NetworkConnection*  CreateConnectionObject();
};

#endif /// EVILENGINE_CORENETWORK_EMAILNETWORKCLIENT