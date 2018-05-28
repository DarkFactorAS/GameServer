
#include "PreCompile.h"

#include "BasePlayfieldUnittest.hpp"
#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/NetworkPackets/ServerNetworkPackets/ServerSavePlayfieldNetworkPacket.h"

class SavePlayfieldUnittest : public BasePlayfieldUnittest, public SignalTarget
{
public:

  SavePlayfieldUnittest() : BasePlayfieldUnittest("SavePlayfield")
  {
  }

  virtual bool Execute() DF_OVERRIDE
  {
    if (!LoginAutenticatedAccount())
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

    worldbuilderModule->SignalSavedPlayfield.Connect(this, &SavePlayfieldUnittest::SlotSavedPlayfield);
    worldbuilderModule->SignalPlayfieldNetworkError.Connect(this, &SavePlayfieldUnittest::SlotPlayfieldMetworkError);

    gameClient->SendPacketToServer(new ServerSavePlayfieldNetworkPacket(m_Playfield));

    return true;
  }

  void SlotSavedPlayfield(PlayfieldSource playfieldSource)
  {
    if (playfieldSource.m_SourceType == Playfield::SourceType_Network && playfieldSource.m_SourceId == m_Playfield->GetPlayfieldId() )
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
