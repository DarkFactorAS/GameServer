
#include "EvilEngine/CoreNetwork/Client/NetworkClient.h"

class StringList;

class SSLNetworkClient : public NetworkClient
{
public:
  SSLNetworkClient();

  bool SendRequest( const String& url );
  int VerifyKeyList( const unsigned char* ca, long length, unsigned char* data, unsigned int dataLength, const StringList& keyList );

};
