
#include "BaseBugReportUnitTest.hpp"

class BugReportWithFileOnDisk : public BaseBugReportUnitTest
{
private:

  BugReport* m_Report;
  String      m_Filename;

public:

  BugReportWithFileOnDisk() : BaseBugReportUnitTest("BugReportWithFileOnDisk")
  {
    m_Report = NULL;
    m_Filename = "fileondisk.txt";
  }

  virtual bool Execute() DF_OVERRIDE
  {
    bool started = BaseBugReportUnitTest::Execute();
    BugReportClient* bugClient = GetClient();
    if (bugClient != NULL)
    {
      bugClient->SignalBugReportCompleted.Connect(this, &BugReportWithFileOnDisk::BugReportSent);

      // Generate the file if we have to
      if (!CreateFile(m_Filename, "This is just a test file used for bugreport unittest."))
      {
        Failed(String::FormatString("Could not create file '%s'", m_Filename.c_str()));
        return false;
      }

      m_Report = new BugReport("BugReportWithEmbeddedFile", "BugReport with embedded files", "test@darkfactor.net");
      if (m_Report->AddFile(m_Filename))
      {
        bugClient->SendBugReport(m_Report);
        return started;
      }
    }
    return false;
  }

  bool CreateFile(const String& fileName, const String& content)
  {
    if (!IOFile::Exists(fileName))
    {
      IOFile* newFile = new IOFile(fileName);
      bool wasCreated = newFile->OpenWrite(fileName, true);
      if ( wasCreated )
      {
        newFile->WriteString(content);
      }
      newFile->Close();
      delete newFile;
      return wasCreated;
    }
    return true;
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
      bugClient->SignalBugReportCompleted.Disconnect(this, &BugReportWithFileOnDisk::BugReportSent);
    }

    if (IOFile::Exists(m_Filename))
    {
      if (!IOFile::DeleteFileWithName(m_Filename))
      {
        Failed(String::FormatString("Cleanup : Failed to delete file '%s'", m_Filename.c_str()));
      }
    }

    return BaseBugReportUnitTest::Cleanup();
  }
};
