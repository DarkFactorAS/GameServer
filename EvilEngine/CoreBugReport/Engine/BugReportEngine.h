#pragma once

#include "EvilEngine/CoreBugReport/Client/BugReportClient.h"
#include "EvilEngine/CoreBugReport/Server/BugReportServer.h"

class BugReportEngine : public CoreNetworkEngine
{
public:
  BugReportEngine();

  // TODO change these two to modules
  void SetClient(BugReportClient* client, bool sendOnInit);
  void SetServer(BugReportServer* server);

  virtual bool                      Init() DF_OVERRIDE;
  virtual bool                      FrameProcess(float deltaTime) DF_OVERRIDE;

private:

  bool                              m_SendClientBugReportsOnInit;
  BugReportClient*                  m_Client;
  BugReportServer*                  m_Server;
};