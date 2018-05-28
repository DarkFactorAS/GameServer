/*************************************************************************************
 * Project     : EvilEngine Multiplatform 2014
 * File        : EngineModule
 * Description : Abstract interface class for CoreGameEngine modules. This interface
 *               has to be implemented to be able to run as a sub component in the
 *               CoreGameEngine.
 *
 * Author      : Thor Richard Hansen
 *************************************************************************************/

#include "Precompile.h"
#include "CorePatcherModule.h"

#include "EvilEngine/CoreIO/ConfigReader/ConfigReader.h"
#include "EvilEngine/CoreIO/Logger/Logger.h"
#include "EvilEngine/CoreLib/BasicTypes/String/StringList.h"
#include "EvilEngine/CoreNetwork/HttpNetworkClient/HttpNetworkClient.h"
#include "EvilEngine/CoreEngine/CoreEngine/EngineModule.h"

CorePatcherModule::CorePatcherModule() :
  EngineModule( EngineModule::COREMODULETYPE_PATCHER),
  m_HttpClient( NULL )
{
  ConfigReader* config = ConfigReader::GetInstanceIfAny();
  if ( config != NULL && config->IsValid())
  {
    // Format : http://home.darkfactor.net:1080/Downloads/VA/
    String fullUrlName = config->GetStringKey("patchserver");
    fullUrlName = fullUrlName.Replace( "http://", "" );

    StringList urlList = StringList::Split( fullUrlName, "/");
    if ( !fullUrlName.empty() )
    {
      String serverName = "home.darkfactor.net";
      uint16 serverPort = 1080;
      m_BaseUrlPath     = "Downloads/VA/";

      m_HttpClient = new HttpNetworkClient( "CorePatcher", serverName, serverPort );
      m_HttpClient->SignalConnectChanged.Connect( this, &CorePatcherModule::OnConnected );
      m_HttpClient->ConnectAsync();
    }
  }
}

void CorePatcherModule::OnConnected( bool didConnect )
{
  if ( didConnect )
  {
    m_HttpClient->LoadUrl(m_BaseUrlPath + "StableVersion.xml", this, &CorePatcherModule::OnDownloadedConfig);
  }
}

void CorePatcherModule::OnDownloadedConfig( BinaryStream* /*data*/ )
{
  LogInfo("CorePatcherModule", "Received Config?" );
}

void CorePatcherModule::RemoveMe()
{
  CoreEngine* parent = GetEngine();
  if ( parent != NULL )
  {
    parent->RemoveEngineModule(this);
  }
}

void CorePatcherModule::FrameProcess(float /*deltaTime*/)
{
  if ( m_HttpClient != NULL )
  {
    m_HttpClient->Run();
  }
}
  
