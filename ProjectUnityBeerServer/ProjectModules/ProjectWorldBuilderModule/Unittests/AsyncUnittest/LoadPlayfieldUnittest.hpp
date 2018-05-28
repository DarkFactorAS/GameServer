
#include "PreCompile.h"

#include "BasePlayfieldUnittest.hpp"
#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/NetworkPackets/ServerNetworkPackets/ServerLoadPlayfieldNetworkPacket.h"

class LoadPlayfieldUnittest : public BasePlayfieldUnittest, public SignalTarget
{
public:

  LoadPlayfieldUnittest() : BasePlayfieldUnittest("LoadPlayfield")
  {
  }

  virtual bool Execute() DF_OVERRIDE
  {
    if (!LoginAutenticatedAccount())
    {
      return false;
    }

    if (!AddPlayfields())
    {
      return false;
    }

    UnittestGameEngineClient* gameClient = UnittestGameEngineClient::GetInstanceIfAny();
    if (gameClient == NULL)
    {
      return false;
    }

    ProjectWorldBuilderClientModule* worldbuilderModule = safe_cast<ProjectWorldBuilderClientModule*> (gameClient->GetEngineModule(ProjectWorldBuilderClientModule::PROJECT_MODULETYPE_CLIENTWORLDBUILDER));
    if (worldbuilderModule == NULL)
    {
      Failed("ProjectWorldBuilderClientModule is NULL");
      return false;
    }

    worldbuilderModule->SignalLoadedPlayfield.Connect(this, &LoadPlayfieldUnittest::SlotLoadedPlayfield);
    worldbuilderModule->SignalPlayfieldNetworkError.Connect(this, &LoadPlayfieldUnittest::SlotPlayfieldMetworkError);

    gameClient->SendPacketToServer(new ServerLoadPlayfieldNetworkPacket(m_Playfield->GetPlayfieldId()));

    return true;
  }

  void SlotLoadedPlayfield(Playfield* playfield)
  {
    if (playfield->GetPlayfieldSource() == Playfield::SourceType_Network && playfield->GetPlayfieldId() == m_Playfield->GetPlayfieldId())
    {
      Succeeded();
    }
    else
    {
      Failed("Wrong playfieldid");
    }
  }

  void SlotPlayfieldMetworkError(uint32 /*packetType*/, uint32 errorCode, const String& errorMessage)
  {
    Failed(String::FormatString("Received playfield network error : (%d/%s)", errorCode, errorMessage.c_str()));
  }

  virtual bool Cleanup() DF_OVERRIDE
  {
    //UnittestGameEngineServer* gameServer = UnittestGameEngineServer::GetInstanceIfAny();
    //if (gameServer != NULL)
    //{
    //  ProjectWorldBuilderServerModule* serverLoginModule = safe_cast<ProjectWorldBuilderServerModule*> (gameServer->GetEngineModule(EngineModule::COREMODULETYPE_SERVERLOGIN));
    //  if (serverLoginModule != NULL)
    //  {
    //    serverLoginModule->ClearAccountList();
    //  }
    //}

    return CoreAsyncUnittest::Cleanup();
  }
};
