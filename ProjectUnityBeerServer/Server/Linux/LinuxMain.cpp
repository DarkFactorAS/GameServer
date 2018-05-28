/*************************************************************************************
* Project     : Valyrian Adventures Server 2014
* File        : LinuxMain
* Description : Server for the Valyrian Adventures multiplatform game.
*               
* Author      : Thor Richard Hansen
*************************************************************************************/

#include "PreCompile.h"
#include "EvilEngine/CoreLib/Assert/DFAssert.h"
#include "EvilEngine/CoreLib/Assert/DFCallstack.h"
#include "EvilEngine/CoreLib/Utils/CoreUtils.h"
#include "EvilEngine/CoreIO/Logger/Logger.h"
#include "EvilEngine/CoreIO/ConfigReader/ConfigReader.h"
#include "EvilEngine/CoreDatabase/CoreDatabase.h"
#include "../../Common/ProjectGameEngine/GameEngine/ProjectGameEngine.h"
#include "../../Common/ProjectGameEngine/GameEngine/ProjectGameEngineServer.h"

#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

static long           s_LastTime      = 0;

int main(int argc, char **argv, char** envp )
{
  // Initialize the callstack system
  dfAssertHandler::GetInstance();
  DFCallstack::GetInstance();
  dfAssertHandler::SetupSignalHandlers();

  Log::Initialize( "", "server.log" );
  LogInfo("Main","Started log");

  if ( argc > 1 && strcmp(argv[1], "-nodb") == 0 )
  {
    CoreDatabase::SetOfflineMode();
  }

  ConfigReader config( "ServerConfig.xml" );
  if ( !config.IsValid() )
  {
    LogFatal("Main","Error in config file");
    return -1;
  }

  uint16 serverPort     = (uint16)config.GetIntKey("serverport");
  if ( serverPort == 0 )
  {
    LogError("Main","Missing serverport setting in teh config file!. Has to be greater than 0.");
    return -1;
  }

  String dbAddress      = config.GetStringKey("dbhost");
  String dbUsername     = config.GetStringKey("dbusername");
  String dbPassword     = config.GetStringKey("dbpassword");
  String dbSchema       = config.GetStringKey("dbschema");

  CoreDatabase* database = CoreDatabase::GetInstance();
  bool hasdbserver = database->Connect( dbAddress, dbUsername, dbPassword, dbSchema );
  if ( !hasdbserver )
  {
#if defined(WINDOWS) && defined(DEBUG)
    bool runOffline = true;
    if ( runOffline )
    {
      CoreDatabase::SetOfflineMode();
      LogError("Main","could not connect to db. Setting the DB in offline mode while in debugger");
    }
    else
#endif
    {
      LogFatal("Main","could not connect to db");
      return -1;
    }
  }

  // Start the server
  ProjectGameEngineServer* gameServer = new ProjectGameEngineServer( serverPort );
  if ( !gameServer->Init() )
  {
    LogFatal("Main","Could not start gameserver");
    return -1;
  }

  // Just halt here while the server is running
  bool runEngine = true;
  while( runEngine )
  {
    long nowTime    = CoreUtils::GetTime();
    if ( s_LastTime == 0 )
    {
      s_LastTime = nowTime;
    }
    long deltaTime  = nowTime - s_LastTime;
    float fDelta    = deltaTime / 1000.0f;

    if ( fDelta < 0.0f )
    {
      LogError( "Main", String::FormatString("Deltatime is less than 0! %d, %d, %f", nowTime, s_LastTime, fDelta ) );
    }
    s_LastTime      = nowTime;

    runEngine = gameServer->FrameProcesss( fDelta );

    boost::this_thread::sleep(boost::posix_time::microseconds(5000)); 
  }

  //CoreDatabase::Destroy();
  LogInfo("Main","Shutting down server");
  delete gameServer;
  Log::Shutdown();

  return 0;
}
