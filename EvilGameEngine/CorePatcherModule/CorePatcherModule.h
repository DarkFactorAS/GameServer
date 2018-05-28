#ifndef EVILENGINE_COREPATCHERMODULE_COREPATCHERMODULE
#define EVILENGINE_COREPATCHERMODULE_COREPATCHERMODULE 1

#include "EvilEngine/CoreEngine/CoreEngine/EngineModule.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"

class HttpNetworkClient;

class CorePatcherModule : public EngineModule, public SignalTarget
{
public:
  
  CorePatcherModule();
  
  virtual void                FrameProcess( float /*deltaTime*/ ) DF_OVERRIDE;
  virtual String              GetModuleName() DF_OVERRIDE { return StaticStr("CorePatcherModule"); }

private:

  void                        RemoveMe();
  void                        OnConnected(bool isConnected);
  void                        OnDownloadedConfig(BinaryStream* data);

  String                      m_BaseUrlPath;
  HttpNetworkClient*          m_HttpClient;
};

#endif /// EVILENGINE_COREPATCHERMODULE_COREPATCHERMODULE