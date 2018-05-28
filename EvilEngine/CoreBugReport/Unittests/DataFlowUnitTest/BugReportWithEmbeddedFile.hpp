
#include "BaseBugReportUnitTest.hpp"

class BugReportWithEmbeddedFile : public BaseBugReportUnitTest
{
private:

  BugReport* m_Report;

public:

  BugReportWithEmbeddedFile() : BaseBugReportUnitTest("BugReportWithEmbeddedFile")
  {
    m_Report = NULL;
  }

  virtual bool Execute() DF_OVERRIDE
  {
    bool started = BaseBugReportUnitTest::Execute();
    BugReportClient* bugClient = GetClient();
    if (bugClient != NULL)
    {
      bugClient->SignalBugReportCompleted.Connect(this, &BugReportWithEmbeddedFile::BugReportSent);

      m_Report = new BugReport("BugReportWithEmbeddedFile", "BugReport with embedded files", "test@darkfactor.net");
      m_Report->AddFileData("testfile.txt", "BWAHAHA\0", 8);

      bugClient->SendBugReport(m_Report);
      return started;
    }
    return false;
  }

  void BugReportSent(BugReport* report)
  {
    if (report->GetClientId() == m_Report->GetClientId())
    {
      Succeeded();
    }
    else
    {
      Failed("Received unknown bugreport");
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
      bugClient->SignalBugReportCompleted.Disconnect(this, &BugReportWithEmbeddedFile::BugReportSent);
    }
    return BaseBugReportUnitTest::Cleanup();
  }
};
