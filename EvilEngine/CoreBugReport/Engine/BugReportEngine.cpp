

#include "Precompile.h"
#include "BugReportEngine.h"

BugReportEngine::BugReportEngine()
{
  m_Client = NULL;
  m_Server = NULL;
}

void BugReportEngine::SetClient(BugReportClient* client, bool sendOnInit)
{
  m_Client = client;
  m_SendClientBugReportsOnInit = sendOnInit;
}
void BugReportEngine::SetServer(BugReportServer* server)
{
  m_Server = server;
}

bool BugReportEngine::Init()
{
  bool initOk = true;
  if (m_Server != NULL)
  {
    initOk = m_Server->InitServer();
  }
  if (m_Client != NULL)
  {
    initOk = m_Client->InitClient(m_SendClientBugReportsOnInit) && initOk;
  }
  return initOk;
}

bool BugReportEngine::FrameProcess(float /*deltaTime*/)
{
  int successful = 0;
  if (m_Client != NULL)
  {
    if (!m_Client->Run())
    {
      return false;
    }
    successful++;
  }
  if (m_Server != NULL)
  {
    if (!m_Server->Run())
    {
      return false;
    }
    successful++;
  }
  return (successful>0);
}