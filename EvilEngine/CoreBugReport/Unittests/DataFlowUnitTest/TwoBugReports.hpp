
#include "BaseBugReportUnitTest.hpp"

class TwoBugReports : public BaseBugReportUnitTest
{
private:

  std::vector< BugReport* > m_BugList;

public:

  TwoBugReports() : BaseBugReportUnitTest("TwoBugReports")
  {
  }

  virtual bool Execute() DF_OVERRIDE
  {
    bool started = BaseBugReportUnitTest::Execute();
    BugReportClient* bugClient = GetClient();
    if (bugClient != NULL)
    {
      bugClient->SignalBugReportCompleted.Connect(this, &TwoBugReports::BugReportSent);
      m_BugList.push_back(new BugReport("TwoBugReports #1", "This is test report 1.", "test@darkfactor.net"));
      m_BugList.push_back(new BugReport("TwoBugReports #2", "This is test report 2.", "test@darkfactor.net"));

      for (uint32 index = 0; index < m_BugList.size(); index++)
      {
        bugClient->SendBugReport(m_BugList[index]);
      }

      return started;
    }
    return false;
  }

  void BugReportSent(BugReport* report)
  {
    for ( std::vector< BugReport* >::iterator itReport = m_BugList.begin(); itReport != m_BugList.end(); ++itReport )
    {
      BugReport* bugReport = *itReport;
      if (bugReport == report)
      {
        m_BugList.erase( itReport );
        if (m_BugList.empty())
        {
          Succeeded();
        }
        return;
      }
    }

    Failed("Received status on unknown bugreport");
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
      bugClient->SignalBugReportCompleted.Disconnect(this, &TwoBugReports::BugReportSent);
      return BaseBugReportUnitTest::Cleanup();
    }
    return false;
  }
};
