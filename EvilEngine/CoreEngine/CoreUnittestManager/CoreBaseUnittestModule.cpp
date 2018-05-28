
#include "PreCompile.h"
#include "CoreBaseUnittestModule.h"
#include "CoreUnittestManager.h"
#include "CodeUnittest/CoreUnittest.h"
#include "CoreIO/Logger/Logger.h"
#include "CoreLib/Utils/CoreUtils.h"

uint32 CoreBaseUnittestModule::s_Id = 0;

CoreBaseUnittestModule::CoreBaseUnittestModule(const String& moduleName) :
  m_Status(0),
  m_ModuleName(moduleName)
{
}

void CoreBaseUnittestModule::Add(CoreUnittest* unittest)
{
  unittest->SetID(++s_Id);
  m_UnitTestList.push_back(unittest);
}

bool CoreBaseUnittestModule::Start()
{
  return true;
}
bool CoreBaseUnittestModule::Cleanup()
{
  for (std::vector< CoreUnittest* >::iterator itTest = m_UnitTestList.begin(); itTest != m_UnitTestList.end(); )
  {
    CoreUnittest* asyncUnittest = *itTest;
    if (asyncUnittest->GetUnittestType() != CoreUnittest::e_AsyncUnitTest)
    {
      delete asyncUnittest;
      itTest = m_UnitTestList.erase(itTest);
    }
    else
    {
      ++itTest;
    }
  }

  return true;
}

CoreUnittest* CoreBaseUnittestModule::GetNextAsyncTest()
{
  for (std::vector< CoreUnittest* >::iterator itTest = m_UnitTestList.begin(); itTest != m_UnitTestList.end(); ++itTest)
  {
    CoreUnittest* asyncUnittest = *itTest;
    if (asyncUnittest != NULL && asyncUnittest->GetUnittestType() == CoreUnittest::e_AsyncUnitTest && asyncUnittest->GetStatus() == CoreUnittest::STATUS_NONE)
    {
      return asyncUnittest;
    }
  }
  return NULL;
}

void CoreBaseUnittestModule::RunAsync(float deltaTime)
{
  if (m_RunningUnitTest == NULL)
  {
    m_RunningUnitTest = GetNextAsyncTest();
  }

  // Skip to next module
  if (m_RunningUnitTest == NULL )
  {
    SetStatus(STATUS_COMPLETED);
    return;
  }

  switch (m_RunningUnitTest->GetStatus())
  {
    case CoreUnittest::STATUS_NONE:
      m_RunningUnitTest->SetStartTime( CoreUtils::GetTimestamp() );
      m_RunningUnitTest->SetStatus(CoreUnittest::STATUS_RUNNING);
      //LogUnittestFMT("UnitTest", "Test %d [%s:%s] ...", m_RunningUnitTest->GetID(), m_ModuleName.c_str(), m_RunningUnitTest->GetName().c_str());
      if (!m_RunningUnitTest->Execute())
      {
        m_RunningUnitTest->SetStatus(CoreUnittest::STATUS_FAILED);
      }
      break;

    case CoreUnittest::STATUS_RUNNING:
      {
        m_RunningUnitTest->AddRunTime(deltaTime);
        m_RunningUnitTest->AddRunCycle();

        float totalRunTime = m_RunningUnitTest->GetRunTime();
        float maxTimeout   = m_RunningUnitTest->GetMaxRunTime();

        if (dfAssertHandler::AmIBeingDebugged())
        {
          maxTimeout = std::max( maxTimeout, 60.0f );
        }

        if (totalRunTime > maxTimeout)
        {
          LogUnittestFMT("UnitTest", "Test %d [%s:%s] Timed out after %0.2f seconds (%d cycles) [UNITTESTFAILED] ThisDelta : %0.2f",
            m_RunningUnitTest->GetID(),
            m_ModuleName.c_str(),
            m_RunningUnitTest->GetName().c_str(),
            m_RunningUnitTest->GetRunTime(),
            m_RunningUnitTest->GetRunCycles(),
            deltaTime);

          m_RunningUnitTest->SetStatus(CoreUnittest::STATUS_FAILED);
        }
      }
      break;

    case CoreUnittest::STATUS_FAILED:
      {
        m_NumFailed++;

        int ms = (int)(m_RunningUnitTest->GetRunTime() * 1000.0f);
        LogUnittestFMT("UnitTest", "Test %d [%s:%s] FAILED. (%d ms) [UNITTESTFAILED] :: %s", 
          m_RunningUnitTest->GetID(), 
          m_ModuleName.c_str(), 
          m_RunningUnitTest->GetName().c_str(), 
          ms,
          m_RunningUnitTest->GetErrorMessage().c_str() );
        m_RunningUnitTest->Cleanup();
        m_RunningUnitTest = NULL;
      }
      break;

    case CoreUnittest::STATUS_SUCCEEDED:
      {
        int ms = (int)( m_RunningUnitTest->GetRunTime() * 1000.0f );
        LogUnittestFMT("UnitTest", "Test %d [%s:%s] SUCCEEDED. (%d ms)", m_RunningUnitTest->GetID(), m_ModuleName.c_str(), m_RunningUnitTest->GetName().c_str(), ms);
        m_RunningUnitTest->Cleanup();
        m_RunningUnitTest = NULL;
      }
      break;
  }
}

int CoreBaseUnittestModule::RunSync()
{
  m_NumFailed = 0;

  for (std::vector< CoreUnittest* >::iterator itTest = m_UnitTestList.begin(); itTest != m_UnitTestList.end(); ++itTest )
  {
    CoreUnittest* syncUnittest = *itTest;
    if (syncUnittest == NULL || syncUnittest->GetUnittestType() != CoreUnittest::e_SyncUnitTest)
    {
      continue;
    }

    //LogUnittestFMT("UnitTest", "Test %d [%s:%s] ...", syncUnittest->GetID(), m_ModuleName.c_str(), syncUnittest->GetName().c_str());
    syncUnittest->SetStartTime(CoreUtils::GetTime());

    if (!syncUnittest->Execute())
    {
      uint32 duration = (uint32)(CoreUtils::GetTime() - syncUnittest->GetStartTime());
      LogUnittestFMT("UnitTest", "Test %d [%s:%s] FAILED. (%s) (%d ms) [ ERROR]", syncUnittest->GetID(), m_ModuleName.c_str(), syncUnittest->GetName().c_str(), syncUnittest->GetErrorMessage().c_str(), duration);
      m_NumFailed++;
    }
    else
    {
      uint32 duration = (uint32)(CoreUtils::GetTime() - syncUnittest->GetStartTime());
      LogUnittestFMT("UnitTest", "Test %d [%s:%s] SUCCEEDED. (%d ms)", syncUnittest->GetID(), m_ModuleName.c_str(), syncUnittest->GetName().c_str(), duration);
    }
    syncUnittest->Cleanup();
  }
  return m_NumFailed;
}

uint32 CoreBaseUnittestModule::GetNumTests() const
{
  return (uint32)m_UnitTestList.size();
}
 