
#include "PreCompile.h"

#include "EvilEngine/CoreEngine/CoreUnittestManager/CodeUnittest/CoreSyncUnittest.h"
#include "EvilEngine/CoreIO/Logger/Logger.h"
#include "EvilEngine/CoreBugReport/Client/BugReportClient.h"
#include "EvilEngine/CoreBugReport/Common/BugReport.h"

class DeleteSavedBugreportUnitTest : public CoreSyncUnittest
{
public:

  DeleteSavedBugreportUnitTest() : CoreSyncUnittest("DeleteSaveBugreportUnitTest")
  {
  }

  virtual bool Execute() DF_OVERRIDE
  {
    BugReportClient* client = new BugReportClient("BugReportUnitTest", "1.0", "localhost" );
    BugReport* savedReport = new BugReport("Saved Report", "Bonus Message", "dummy@bla.com");

    if (!client->DeleteAllBugReports())
    {
      Failed("Failed to delete old bugreports");
      return false;
    }

    if (!client->SaveBugReport(savedReport))
    {
      Failed("Failed to save bugreport");
      return false;
    }

    if(!client->DeleteBugReport(savedReport))
    {
      Failed("Failed to delete new bugreports");
      return false;
    }

    if (client->GetNumberOfBugReports() != 0 )
    {
      Failed("Failed to delete bugreport from memory");
      return false;
    }

    delete client;

    return true;
  }
};
