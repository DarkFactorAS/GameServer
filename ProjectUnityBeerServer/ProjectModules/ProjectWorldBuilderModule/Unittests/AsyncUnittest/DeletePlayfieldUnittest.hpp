
#include "PreCompile.h"

#include "BasePlayfieldUnittest.hpp"
#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/NetworkPackets/ServerNetworkPackets/ServerDeletePlayfieldNetworkPacket.h"

class DeletePlayfieldUnittest : public BasePlayfieldUnittest, public SignalTarget
{
public:

  DeletePlayfieldUnittest() : BasePlayfieldUnittest("DeletePlayfield")
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

    worldbuilderModule->SignalDeletedPlayfield.Connect(this, &DeletePlayfieldUnittest::SlotDeletedPlayfield);
    worldbuilderModule->SignalPlayfieldNetworkError.Connect(this, &DeletePlayfieldUnittest::SlotPlayfieldMetworkError);

    gameClient->SendPacketToServer(new ServerDeletePlayfieldNetworkPacket(m_PlayfieldId));

    return true;
  }

  void SlotDeletedPlayfield(PlayfieldSource playfieldSource)
  {
    if (playfieldSource.m_SourceType == Playfield::SourceType_Network && playfieldSource.m_SourceId == m_PlayfieldId)
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
