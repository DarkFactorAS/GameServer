
#include "EvilEngine/CoreBugReport/Server/BugReportServer.h"

class UnittestBugReportServer : public BugReportServer
{
private:

  static UnittestBugReportServer* s_Instance;

public:

  UnittestBugReportServer(const String& appName, uint32 appVersion) :
    BugReportServer(appName, appVersion, 0, String::zero)
  {
  }

  static UnittestBugReportServer* GetInstanceIfAny()
  {
    return s_Instance;
  }

  static UnittestBugReportServer* CreateInstance(const String& appName, uint32 appVersion)
  {
    if (s_Instance == NULL)
    {
      s_Instance = new UnittestBugReportServer(appName, appVersion);
    }
    return s_Instance;
  }

  static void                     DestroyInstance()
  {
    delete s_Instance;
    s_Instance = NULL;
  }

  virtual bool                    InsertInDatabase(BugReport* bugReport) DF_OVERRIDE
  {
    if (bugReport != NULL)
    {
      uint32 serverId = bugReport->GetClientId() * 10;
      bugReport->SetServerId(serverId);
      return true;
    }
    return false;
  }

  virtual BugReportFile*          GetPartialFile(uint32 /*serverId*/) DF_OVERRIDE
  {
    return NULL;
  }

};

UnittestBugReportServer* UnittestBugReportServer::s_Instance = NULL;
