
#include "BaseBugReportUnitTest.hpp"

class DuplicateReportWithNewVersion : public BaseBugReportUnitTest
{
private:

  BugReport* m_ReportOldVersion;
  BugReport* m_ReportNewVersion;

  uint32     m_OldVersion;
  uint32     m_NewVersion;

  const String MINOR_VERSION = ".1";

public:

  DuplicateReportWithNewVersion() : BaseBugReportUnitTest("DuplicateReportWithNewVersion")
  {
  }

  virtual bool Execute() DF_OVERRIDE
  {
    bool started = BaseBugReportUnitTest::Execute();
    BugReportClient* bugClient = GetClient();
    if (bugClient != NULL)
    {
      m_OldVersion = 0;
      m_NewVersion = 0;

      bugClient->SignalBugReportCompleted.Connect(this, &DuplicateReportWithNewVersion::BugReportSent);
      m_ReportOldVersion = new BugReport("DuplicateReportWithNewVersion", "This is duplicate report except app version", "test@darkfactor.net");
      m_ReportNewVersion = new BugReport("DuplicateReportWithNewVersion", "This is duplicate report except app version", "test@darkfactor.net");

      String oldVersion = bugClient->GetClientVersion();
      bugClient->SendBugReport( m_ReportOldVersion );
      bugClient->SetClientVersion( oldVersion + MINOR_VERSION);
      bugClient->SendBugReport(m_ReportNewVersion);
      bugClient->SetClientVersion(oldVersion);

      return started;
    }
    return false;
  }

  void BugReportSent(BugReport* report)
  {
    if (m_ReportOldVersion == report)
    {
      m_OldVersion = report->GetServerId();
    }
    else if (m_ReportNewVersion == report)
    {
      m_NewVersion = report->GetServerId();
    }
    else
    {
      Failed("Unknown bureport received");
      return;
    }

    if (m_OldVersion != 0 && m_NewVersion != 0)
    {
      if (m_OldVersion == m_NewVersion)
      {
        Failed("The duplicate bugreports resolved to the same server ID!");
      }
      else
      {
        Succeeded();
      }
    }
  }

  virtual void ConnectionChanged(uint32 connectionStatus) DF_OVERRIDE
  {
    FailOnDisconnect(connectionStatus);
  }

  virtual bool Cleanup() DF_OVERRIDE
  {
    BugReportClient* bugClient = GetClient();
    if (bugClient != NULL)
    {
      bugClient->SignalBugReportCompleted.Disconnect(this, &DuplicateReportWithNewVersion::BugReportSent);
      return BaseBugReportUnitTest::Cleanup();
    }
    return false;
  }
};
