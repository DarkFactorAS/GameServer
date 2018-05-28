
#include "PreCompile.h"

#include "EvilEngine/CoreEngine/CoreUnittestManager/CodeUnittest/CoreSyncUnittest.h"
#include "EvilEngine/CoreIO/Logger/Logger.h"
#include "EvilEngine/CoreIO/UserConfig/UserConfig.h"
#include "EvilEngine/CoreLib/BasicTypes/Variant/Variant.h"
#include "EvilEngine/CoreLib/BasicTypes/Variant/VariantTree.h"

class CoreIOUserConfigTest : public CoreSyncUnittest
{
public:

  CoreIOUserConfigTest() : CoreSyncUnittest("CoreIOUserConfigTest")
  {
  }

  virtual bool Execute() DF_OVERRIDE
  {
    UserConfig* userConfig = new UserConfig("TestConfig");
    VariantTree* testNode = new VariantTree();
    testNode->SetName("TestName");
    testNode->SetValue("TestKey1", new Variant("TestVal1") );

    userConfig->AddChild(testNode);
    if (!userConfig->Save())
    {
      Failed("Failed to save config");
      return false;
    }

    return true;
  }
};
