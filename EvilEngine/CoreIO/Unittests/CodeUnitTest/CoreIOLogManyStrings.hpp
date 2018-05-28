
#include "PreCompile.h"

#include "EvilEngine/CoreEngine/CoreUnittestManager/CodeUnittest/CoreSyncUnittest.h"
#include "EvilEngine/CoreIO/Logger/Logger.h"

class CoreIOLogManyStrings : public CoreSyncUnittest
{
public:

  const String mediumString = "1234567890";

  CoreIOLogManyStrings() : CoreSyncUnittest("CoreIOLogManyStrings")
  {
  }

  virtual bool Execute() DF_OVERRIDE
  {
    String longString = mediumString;

    // Check memory here.

    for ( uint32 numRuns = 0; numRuns < 250000; numRuns++ )
    {
      if (!Log::LogStaticMsg(Log::e_LogDevNull, "Unittest", "This is a message that will be logged many many times"))
      {
        Failed("Could not log many many strings!");
        return false;
      }
    }

    // Check memory here

    return true;
  }
};
