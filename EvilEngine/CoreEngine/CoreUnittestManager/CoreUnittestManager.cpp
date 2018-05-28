
#include "PreCompile.h"
#include "CoreUnittestManager.h"
#include "CoreBaseUnittestModule.h"
#include "EvilEngine/CoreEngine/CoreUnittestManager/CodeUnittest/CoreUnittest.h"

CoreUnittestManager::CoreUnittestManager()
{
}

void CoreUnittestManager::Add(CoreBaseUnittestModule* unittest )
{
  m_NumFailed = 0;
  m_ModuleList.push_back( unittest );
}

bool CoreUnittestManager::RunAsync(float deltaTime)
{
  std::vector< CoreBaseUnittestModule* >::iterator itTest = m_ModuleList.begin();
  if ( itTest != m_ModuleList.end() )
  {
    CoreBaseUnittestModule* module = *itTest;
    switch(module->GetStatus() )
    {
      case CoreBaseUnittestModule::STATUS_NONE:
        module->SetStatus(CoreBaseUnittestModule::STATUS_RUNNING);
        if (!module->Start())
        {
          module->SetStatus(CoreBaseUnittestModule::STATUS_COMPLETED);
        }
        break;
      case CoreBaseUnittestModule::STATUS_RUNNING:
        module->RunAsync(deltaTime);
        break;
      case CoreBaseUnittestModule::STATUS_COMPLETED:
        m_NumFailed += module->GetNumFailed();
        if (module->Cleanup() )
        {
          m_ModuleList.erase(itTest);
          delete module;
        }
        break;

      case CoreBaseUnittestModule::STATUS_FAILED:
        m_NumFailed += module->GetNumFailed();
        if (module->Cleanup())
        {
          m_ModuleList.erase(itTest);
          delete module;
        }
        return false;

      default:
        // ERROR!
        return false;
    }
    return true;
  }

  // report
  return false;
}

bool CoreUnittestManager::RunSync()
{
  m_NumFailed = 0;
  for (std::vector< CoreBaseUnittestModule* >::iterator itTest = m_ModuleList.begin(); itTest != m_ModuleList.end(); ++itTest )
  {
    CoreBaseUnittestModule* module = *itTest;

    module->RunSync();
    m_NumFailed += module->GetNumFailed();
  }

  return (m_NumFailed==0);
}

void CoreUnittestManager::Cleanup()
{
  for (std::vector< CoreBaseUnittestModule* >::iterator itTest = m_ModuleList.begin(); itTest != m_ModuleList.end(); ++itTest)
  {
    CoreBaseUnittestModule* module = *itTest;
    delete module;
  }
  m_ModuleList.clear();
}

uint32 CoreUnittestManager::GetNumTests() const
{
  uint32 numTests = 0;
  for (std::vector< CoreBaseUnittestModule* >::const_iterator itTest = m_ModuleList.begin(); itTest != m_ModuleList.end(); ++itTest)
  {
    CoreBaseUnittestModule* module = *itTest;
    numTests += module->GetNumTests();
  }
  return numTests;
}
