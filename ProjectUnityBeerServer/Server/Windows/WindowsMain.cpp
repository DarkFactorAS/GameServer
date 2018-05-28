/*************************************************************************************
* Project     : Valyrian Adventures 2014
* File        : main
* Description : Multi-platform board game that is built for iOS/Android/Windows/Mac.
*               
* Author      : Thor Richard Hansen
*************************************************************************************/

#include "PreCompile.h"
#include "EvilEngine/CoreIO/Logger/Logger.h"
#include "EvilEngine/CoreLib/Utils/CoreUtils.h"
#include "EvilEngine/CoreDatabase/CoreDatabase.h"
#include "EvilEngine/CoreIO/ConfigReader/ConfigReader.h"
#include "EvilEngine/CoreEngine/CoreResourceManager/CoreResourceManager.h"
#include "EvilEngine/CoreEngine/CoreFramework/Windows/CoreWindowsServer.h"
#include "EvilEngine/CoreLib/Assert/DFAssert.h"
#include "EvilEngine/CoreLib/Assert/DFCallstack.h"
#include "EvilEngine/CoreNetwork/Server/NetworkServer.h"

#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"
#include "EvilGameEngine/CoreGameLogin/LoginModule/CoreGameServerLoginModule.h"

#include "ProjectModules/ProjectWorldBuilderModule/WorldBuilderModule/ProjectWorldBuilderServerModule.h"
#include "ProjectModules/ProjectGameManagementModule/Logic/GameManagementModule/ProjectGameManagementServerModule.h"
#include "ProjectModules/ProjectOnlineGameModule/Module/ProjectOnlineGameServerModule.h"
#include "EvilEngine/CoreEngine/CoreEventLogger/CoreEventLogger.h"
#include "EvilEngine/CoreVersion/version.h"


#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

#include "resource.h"
//#include "windows.h"

#ifndef SERVER
  #error "SERVER build not defined! Make sure you have Server-Debug or Server-Release selected"
#endif

int main(int argc, char* argv[] )
{
  // Initialize the callstack system
  dfAssertHandler::GetInstance();
  DFCallstack::GetInstance();
  dfAssertHandler::SetupSignalHandlers();

  CoreWindowsServer* windowServer = new CoreWindowsServer( "UnityBEERServer", "Unity BEER Server" );
  windowServer->SetFramerate(10);

  if (argc == 1/*dfAssertHandler::AmIBeingDebugged()*/)
  {
    if (!windowServer->CreateAsConsoleApp(IDI_ICON1, IDI_ICON3))
    {
      return -1;
    }
  }
  else
  {
    if (!windowServer->CreateAsService(argc,argv))
    {
      return 0;
    }
  }

  Log::Initialize( "", String::FormatString("%sserver.log", windowServer->GetDefaultPath()));
  LogInfoFMT("Main", "Started server. Version %d (%s)", BUILD_REVISION, BUILD_DATE);

  String configFilename = String::FormatString("%sServerConfig.xml", windowServer->GetDefaultPath());
  ConfigReader* config = ConfigReader::CreateInstance(configFilename);
  if ( !config->IsValid() )
  {
    LogFatalFMT("Main","Error in config file (Filename: %s", configFilename.c_str());
    return -1;
  }

  uint16 serverPort     = (uint16)config->GetIntKey("serverport");
  if ( serverPort == 0 )
  {
    LogError("Main","Missing serverport setting in the config file!. Has to be greater than 0.");
    return -1;
  }

  String dbAddress      = config->GetStringKey("dbhost");
  String dbUsername     = config->GetStringKey("dbusername");
  String dbPassword     = config->GetStringKey("dbpassword");
  String dbSchema       = config->GetStringKey("dbschema");
  CoreDatabase* database = CoreDatabase::GetInstance();
  //database->SetDatabaseDisplayFlags(CoreDatabase::DISPLAYFLAG_ALL);
  bool hasdbserver = database->Connect( dbAddress, dbUsername, dbPassword, dbSchema );
  if ( !hasdbserver )
  {
    LogFatal("Main","could not connect to db");
    return -1;
  }

  // Create the engine with the wanted modules
  CoreGameEngine* gameEngine = new CoreGameEngine();
  gameEngine->SetGameKey("UnityBeer");
  gameEngine->SetNetworkBase( new NetworkServer("GameNetwork", serverPort, true ) );
  gameEngine->AddEngineModule( new CoreGameServerLoginModule() );
  gameEngine->AddEngineModule( new ProjectWorldBuilderServerModule() );
  gameEngine->AddEngineModule( new ProjectGameManagementServerModule() );
  gameEngine->AddEngineModule( new ProjectOnlineGameServerModule());

  windowServer->SetEngine(gameEngine);

  int ret = windowServer->Run();

  //CoreDatabase::Destroy();
  LogInfo("Main","Shutting down server");
  delete config;
  delete gameEngine;
  delete windowServer;
  LogInfo("Main","Server successfully shut down");
  Log::Shutdown();
  
  return ret;
}
