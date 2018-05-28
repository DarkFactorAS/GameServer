
#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"

class UnittestGameEngineClient : public CoreGameEngine
{
private:

  static UnittestGameEngineClient* s_Instance;

public:

  UnittestGameEngineClient(const String& /*appName*/, uint32 /*appVersion*/) :
    CoreGameEngine()
  {
  }

  static UnittestGameEngineClient* GetInstanceIfAny()
  {
    return s_Instance;
  }

  static UnittestGameEngineClient* CreateInstance(const String& appName, uint32 appVersion)
  {
    dfAssert(s_Instance == NULL, "UnittestGameEngineClient: Instance is not null!" );
    if (s_Instance == NULL)
    {
      s_Instance = new UnittestGameEngineClient(appName, appVersion);
    }
    return s_Instance;
  }

  static void                     DestroyInstance()
  {
    delete s_Instance;
    s_Instance = NULL;
  }

};

UnittestGameEngineClient* UnittestGameEngineClient::s_Instance = NULL;
