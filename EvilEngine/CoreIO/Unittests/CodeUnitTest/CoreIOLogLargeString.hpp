
#include "PreCompile.h"

#include "EvilEngine/CoreEngine/CoreUnittestManager/CodeUnittest/CoreSyncUnittest.h"
#include "EvilEngine/CoreIO/Logger/Logger.h"

class CoreIOLogLargeString : public CoreSyncUnittest
{
public:

  const String mediumString = "1234567890";

  CoreIOLogLargeString() : CoreSyncUnittest("CoreIOLogLargeString")
  {
  }

  virtual bool Execute() DF_OVERRIDE
  {
    String longString = mediumString;

    for ( uint32 numRuns = 0; numRuns < 10; numRuns++ )
    {
      longString = String::FormatString("%s%s", longString.c_str(), longString.c_str() );
    }

    if ( !Log::LogStaticMsg( Log::e_LogDebug, "Unittest", longString.c_str() ) )
    {
      Failed("Could not create long string!");
      return false;
    }

    return true;
  }
};
