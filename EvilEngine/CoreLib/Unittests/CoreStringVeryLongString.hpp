
#include "PreCompile.h"

#include "EvilEngine/CoreEngine/CoreUnittestManager/CodeUnittest/CoreSyncUnittest.h"

class CoreStringVeryLongString : public CoreSyncUnittest
{
public:

  const String mediumString = "1234567890";

  CoreStringVeryLongString() : CoreSyncUnittest("CoreStringVeryLongString")
  {
  }

  virtual bool Execute() DF_OVERRIDE
  {
    String longString = mediumString;

    for ( uint32 numRuns = 0; numRuns < 10; numRuns++ )
    {
      longString = String::FormatString("%s%s", longString.c_str(), longString.c_str() );
    }

    if ( longString.Length() != 10240 )
    {
      Failed("Could not create long string!");
      return false;
    }

    return true;
  }
};
