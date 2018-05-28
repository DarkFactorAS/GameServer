#ifndef EVILENGINE_COREUNITTEST_CORESYNCUNITTEST
#define EVILENGINE_COREUNITTEST_CORESYNCUNITTEST 1

#include "CoreUnittest.h"

class CoreSyncUnittest : public CoreUnittest
{
public:

  CoreSyncUnittest( String testName );

  virtual bool   Execute() DF_OVERRIDE { return false; };

  virtual CoreUnittest::UnitTestType GetUnittestType() DF_OVERRIDE DF_FINAL
  {
    return CoreUnittest::e_SyncUnitTest;
  }

};

#endif /// EVILENGINE_COREUNITTEST_CORESYNCUNITTEST