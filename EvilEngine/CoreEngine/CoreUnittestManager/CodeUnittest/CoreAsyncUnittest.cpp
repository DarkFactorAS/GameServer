
#include "PreCompile.h"
#include "CoreUnittest.h"
#include "CoreAsyncUnittest.h"
#include "CoreIO/Logger/Logger.h"

CoreAsyncUnittest::CoreAsyncUnittest( String testName ) : CoreUnittest(testName)
{
}

bool CoreAsyncUnittest::Execute()
{
  LogInfoFMT("UnitTest", "Test %d [%s] STARTED.", GetID(), m_Testname.c_str());
  return true;
}

void CoreAsyncUnittest::Failed( const String& errorMessage )
{
  CoreUnittest::Failed(errorMessage);
}
