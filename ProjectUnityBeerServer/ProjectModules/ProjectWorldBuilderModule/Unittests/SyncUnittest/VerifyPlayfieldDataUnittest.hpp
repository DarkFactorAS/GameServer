
#include "PreCompile.h"

#include "EvilEngine/CoreEngine/CoreUnittestManager/CodeUnittest/CoreSyncUnittest.h"
#include "EvilEngine/CoreIO/Logger/Logger.h"

#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/Data/WorldBuilderPacketData.h"
#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/Data/Playfield.h"
#include "ProjectUnityBeerServer/ProjectModules/ProjectWorldBuilderModule/WorldBuilderModule/ProjectWorldBuilderServerModule.h"

class VerifyPlayfieldDataUnittest : public CoreSyncUnittest
{
public:

  VerifyPlayfieldDataUnittest() : CoreSyncUnittest("VerifyPlayfieldData")
  {
  }

  virtual bool Execute() DF_OVERRIDE
  {
    Playfield* testPlayfield = new Playfield();
    testPlayfield->SetPlayfieldName("BrokenSize");
    testPlayfield->SetBoardSizeX(12);
    testPlayfield->SetBoardSizeY(12);

    uint32 errorCode = ProjectWorldBuilderServerModule::VerifyPlayfieldData(testPlayfield);
    if (errorCode != WorldBuilderPacketData::ErrorCode_InvalidPlayfieldSize)
    {
      Failed("InvalidPlayfieldSize");
      return false;
    }

    testPlayfield->SetBoardSizeX(12);
    testPlayfield->SetBoardSizeY(16);
    testPlayfield->SetPlayfieldName("AA");

    errorCode = ProjectWorldBuilderServerModule::VerifyPlayfieldData(testPlayfield);
    if (errorCode != WorldBuilderPacketData::ErrorCode_PlayfieldnameTooShort)
    {
      Failed("PlayfieldnameTooShort");
      return false;
    }

    testPlayfield->SetPlayfieldName("A Very very very very Very very very very long playfieldname");
    errorCode = ProjectWorldBuilderServerModule::VerifyPlayfieldData(testPlayfield);
    if (errorCode != WorldBuilderPacketData::ErrorCode_PlayfieldnameTooLong)
    {
      Failed("PlayfieldnameTooLong");
      return false;
    }

    testPlayfield->SetPlayfieldName("Normal playfieldname");
    errorCode = ProjectWorldBuilderServerModule::VerifyPlayfieldData(testPlayfield);
    if (errorCode != WorldBuilderPacketData::ErrorCode_NoError)
    {
      Failed("Unknown playfield error");
      return false;
    }

    delete testPlayfield;
    return true;
  }
};
