#ifndef EVILENGINE_COREUNITTEST_COREUNITTESTMANAGER
#define EVILENGINE_COREUNITTEST_COREUNITTESTMANAGER 1

#include "CoreBaseUnittestModule.h"

class CoreUnittest;
///class CoreBaseUnittestModule;

class CoreUnittestManager
{

private:

  uint32                                 m_NumFailed;
  std::vector< CoreBaseUnittestModule* > m_ModuleList;

public:

  CoreUnittestManager();

  void Add(CoreBaseUnittestModule* module);

  bool RunSync();
  bool RunAsync(float deltaTime);
  void Cleanup();

  uint32 GetNumFailed() const{ return m_NumFailed; }
  uint32 GetNumTests() const;
};

#endif