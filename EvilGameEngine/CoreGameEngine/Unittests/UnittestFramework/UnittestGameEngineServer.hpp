
#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"

class UnittestGameEngineServer : public CoreGameEngine
{
private:

  static UnittestGameEngineServer* s_Instance;

public:

  UnittestGameEngineServer(const String& /*appName*/, uint32 /*appVersion*/) :
    CoreGameEngine()
  {
  }

  static UnittestGameEngineServer* GetInstanceIfAny()
  {
    return s_Instance;
  }

  static UnittestGameEngineServer* CreateInstance(const String& appName, uint32 appVersion)
  {
    dfAssert(s_Instance == NULL, "UnittestGameEngineServer: Instance is not null!");
    if (s_Instance == NULL)
    {
      s_Instance = new UnittestGameEngineServer(appName, appVersion);
    }
    return s_Instance;
  }

  static void                     DestroyInstance()
  {
    delete s_Instance;
    s_Instance = NULL;
  }

};

UnittestGameEngineServer* UnittestGameEngineServer::s_Instance = NULL;
