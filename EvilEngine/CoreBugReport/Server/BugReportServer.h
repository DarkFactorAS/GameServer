#ifndef EVILENGINE_COREBUGREPORT_BUGREPORTSERVER
#define EVILENGINE_COREBUGREPORT_BUGREPORTSERVER 1

//#ifdef SERVER

#include "EvilEngine/CoreNetwork/Common/NetworkConnection.h"
#include "EvilEngine/CoreNetwork/Engine/CoreNetworkEngine.h"
#include "EvilEngine/CoreNetwork/Server/NetworkServer.h"

class BugReport;
class BugReportFile;
class BugReportNetworkServer;

class BugReportServer : public CoreNetworkEngine
{
public:

  BugReportServer( const String& appName, uint32 appVersion, uint16 serverPort, const String& datafolder );
  ~BugReportServer();

  bool                            InitServer();
  bool                            Run();
  virtual void                    OnReceivedNetworkPacket(uint32 /*connectionInstance*/, const BinaryStream* /*dataStream*/) DF_OVERRIDE;

  virtual void                    SetNetworkBase(NetworkBase* networkBase) DF_OVERRIDE;

private:

  String                          GetFileExtension(const String& fileName);

  void                            HandleBugReport(NetworkConnection* connection, const BinaryStream* dataStream);
  void                            HandleFile(NetworkConnection* connection, const BinaryStream* dataStream);
  void                            HandleStatusFile(NetworkConnection* connection, const BinaryStream* dataStream);
  void                            HandlePartialFile(NetworkConnection* connection, const BinaryStream* dataStream);

  virtual bool                    InsertInDatabase(BugReport* report);
  virtual BugReportFile*          GetPartialFile(uint32 serverId);

  String                          m_AppName;
  String                          m_AppVersion;

  /// Location of the files on disk
  String                          m_Datafolder;

  /// Network server
  NetworkServer*                  m_NetworkServer;
};

//#endif /// SERVER
#endif /// EVILENGINE_COREBUGREPORT_BUGREPORTSERVER
