
#ifndef EVILENGINE_CORENETWORK_EMAILNETWORKCLIENT
#define EVILENGINE_CORENETWORK_EMAILNETWORKCLIENT

#include "../Common/NetworkBase.h"
#include "../Client/NetworkClient.h"

class NetworkConnection;
class BinaryStream;

class HttpNetworkClient : public NetworkClient
{
public:

  HttpNetworkClient( const String& threadName, const String& serverAddress, uint16 serverPort );

  template <typename OBJ_CLASS, typename OBJ_POINTER>
  void LoadUrl(const String& url, OBJ_POINTER* obj, void (OBJ_CLASS::*callback)( BinaryStream* data ) )
  {
    CancelNavigation();
    SignalReveivedData.Connect( obj, callback );
    NavigateTo( url );
  }

  //virtual NetworkConnection*  CreateConnectionObject();

private:
  
  void                            CancelNavigation();
  void                            NavigateTo( const String& url );
  
  Signal1< void, BinaryStream* >  SignalReveivedData;
};

#endif /// EVILENGINE_CORENETWORK_EMAILNETWORKCLIENT