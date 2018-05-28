
#include "PreCompile.h"
#include "CoreUnittest.h"

CoreUnittest::CoreUnittest( String testName )
{
  m_StartTime = 0;
  m_Testname = testName;
  m_Status = STATUS_NONE;
  m_RunTime = 0.0f;
  m_RunCycle = 0;
}

bool CoreUnittest::Cleanup()
{
  return true;
}

void CoreUnittest::Failed( const String& errorMessage )
{
  SetStatus(STATUS_FAILED);
  m_ErrorMessage = errorMessage;
}

void CoreUnittest::Succeeded()
{
  SetStatus(STATUS_SUCCEEDED);
}
