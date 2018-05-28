#ifndef EVILENGINE_COREBUGREPORT_COMMON_BUGREPORTCLIENT
#define EVILENGINE_COREBUGREPORT_COMMON_BUGREPORTCLIENT 1

#include "EvilEngine/CoreLib/Signal/SignalTarget.h"
#include "EvilEngine/CoreNetwork/Engine/CoreNetworkEngine.h"

class BugReportNetworkClient;
class BugReport;
class UserConfig;
class BinaryStream;
class NetworkClient;

class BugReportClient : public CoreNetworkEngine
{
public:
  BugReportClient( const String& appName, const String& appVersion, const String& serverHostnameAndPort );
  ~BugReportClient();

  static BugReportClient*         GetInstanceIfAny();
  static BugReportClient*         CreateInstance(const String& appName, const String& appVersion, const String& serverHostnameAndPort, bool sendReports = true);
  static BugReportClient*         CreateInstance(const String& appName, uint32 appVersion, const String& serverHostnameAndPort, bool sendReports = true);
  static void                     DestroyInstance();

  virtual void                    SetNetworkBase(NetworkBase* networkBase) DF_OVERRIDE;

  bool                            HasHostType(int networkType);
  void                            ChangeBugServer( int hostType );
  int                             GetCurrentHost();
  void                            SetBugReportHost(const String& serverHostnameAndPort);

  bool                            InitClient(bool sendAfterLoad);
  uint32                          Load(bool sendAfterLoad);
  bool                            Run();

  void                            BlockAndSendReports();
  void                            GenerateUniqueClientId(BugReport* bugReport);
  void                            SendBugReport(BugReport* bugReport);
  bool                            SaveBugReport(BugReport* bugReport);
  bool                            AddBugReport(BugReport* newBugReport);
  bool                            DeleteBugReport(BugReport* bugReport);

  Signal1< void, uint16 >         SignalUpdatedReports;
  Signal1< void, uint32 >         SignalConnectionChanged;
  Signal1< void, BugReport* >     SignalBugReportCompleted;

  virtual void                    SendAssertAsBugreport(const char* errorMessage, const char* callstack, bool isFatal);

  // Overloaded from NetworkEngine
  virtual void                    OnReceivedNetworkPacket(uint32 /*connectionInstance*/, const BinaryStream* /*dataStream*/) DF_OVERRIDE;
  virtual void                    OnConnectionChanged(uint32 /*connectionInstance*/, uint32 /*conectionStatus*/) DF_OVERRIDE;

  uint32                          GetNumberOfQueuedBugreports() const{ return m_BugReportList.size(); }
  uint32                          GetNumberOfBugReports();
  bool                            DeleteAllBugReports();

  const String&                   GetClientVersion() { return m_AppVersion; }
  void                            SetClientVersion(const String& appVersion){ m_AppVersion = appVersion; }

private:

  void                            SendFilesForBugreport(BugReport* bugReport);

  void                            SlotProcessReports();
  void                            InitSend();

  BugReport*                      GetBugReportWithClientId(uint32 clientId);
  BugReport*                      GetBugReportWithServerId(uint32 serverId);

  void                            ReceivedBugReportID(const BinaryStream* dataStream);
  void                            ServerReceivedFileOK(const BinaryStream* dataStream);

  bool                            DeleteSavedBugReport(uint32 clientId);

  String                          m_AppName;
  String                          m_AppVersion;
  static BugReportClient*         s_Instance;
  bool                            m_Isloaded;

  UserConfig*                     m_UserConfig;

  std::vector< BugReport* >       m_BugReportList;

  NetworkClient*                  m_NetworkClient;
};

#endif /// EVILENGINE_COREBUGREPORT_COMMON_BUGREPORTCLIENT
