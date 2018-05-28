
#include "BaseBugReportUnitTest.hpp"

class SingleBugReport : public BaseBugReportUnitTest
{
private:

  BugReport* m_Report;

public:

  SingleBugReport() : BaseBugReportUnitTest("SingleBugReport")
  {
    m_Report = NULL;
  }

  virtual bool Execute() DF_OVERRIDE
  {
    bool started = BaseBugReportUnitTest::Execute();
    BugReportClient* bugClient = GetClient();
    if (bugClient != NULL)
    {
      bugClient->SignalBugReportCompleted.Connect(this, &SingleBugReport::BugReportSent);
      m_Report = new BugReport("SingleBugReport #1", "This is test report 1.", "test@darkfactor.net");
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
      bugClient->SignalBugReportCompleted.Disconnect(this, &SingleBugReport::BugReportSent);
    }
    return BaseBugReportUnitTest::Cleanup();
  }
};
