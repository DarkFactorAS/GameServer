#pragma once

#include "PreCompile.h"

#include "EvilEngine/CoreBugReport/Common/BugReport.h"
#include "EvilEngine/CoreEngine/CoreUnittestManager/CodeUnittest/CoreAsyncUnittest.h"
#include "EvilEngine/CoreLib/Assert/DFAssert.h"
#include "EvilEngine/CoreNetwork/Common/NetworkConnection.h"
#include "EvilEngine/CoreBugReport/Client/BugReportClient.h"

class BaseBugReportUnitTest : public CoreAsyncUnittest, public SignalTarget
{

public:

  BaseBugReportUnitTest( String testName ) : CoreAsyncUnittest( testName )
  {
  }

  BugReportClient* GetClient()
  {
    BugReportClient* bugClient = BugReportClient::GetInstanceIfAny();
    dfAssertFMT(bugClient != NULL, "BugReportUnittestRunner : BugReport client is NULL!");
    return bugClient;
  }

  virtual bool Execute() DF_OVERRIDE
  {
    GetClient()->SignalConnectionChanged.Connect(this, &BaseBugReportUnitTest::ConnectionChanged);
    return CoreAsyncUnittest::Execute();
  }

  virtual bool Cleanup() DF_OVERRIDE
  {
    BugReportClient* bugClient = GetClient();
    if (bugClient != NULL)
    {
      bugClient->SignalConnectionChanged.Disconnect(this, &BaseBugReportUnitTest::ConnectionChanged);

      if (bugClient->GetNumberOfBugReports() > 0)
      {
        Failed("Bugreport did not clean up after itself!");
        return false;
      }
      return true;
    }
    return false;
  }

  virtual void ConnectionChanged(uint32 connectionStatus )
  {
    switch (connectionStatus)
    {
      case NetworkConnection::NETWORKSTATUS_FAILED:
        Failed("Connection to server failed");
        break;
      default:
        break;
    }
  }

  void FailOnDisconnect(uint32 connectionStatus)
  {
    switch (connectionStatus)
    {
    case NetworkConnection::NETWORKSTATUS_DISCONNECTED:
      Failed("Server disconnected");
      break;
    default:
      BaseBugReportUnitTest::ConnectionChanged(connectionStatus);
      break;
    }
  }
};