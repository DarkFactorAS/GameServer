/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : CoreGameEngine
*
* Description : Main loop for all game engine objects. This class also forwards input calls
*               to the GUI Toolkit.
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"

#include "EvilEngine/CoreLib/Utils/CoreUtils.h"
#include "EvilEngine/CoreLib/BasicTypes/Definitions/VolatileArgs.h"
#include "EvilEngine/CoreNetwork/Client/NetworkClient.h"
#include "EvilEngine/CoreNetwork/Common/NetworkBase.h"
#include "EvilEngine/CoreNetwork/Common/NetworkConnection.h"
#include "EvilEngine/CoreBugReport/Common/BugReport.h"
#include "EvilEngine/CoreEngine/CoreResourceManager/CoreResourceManager.h"
#include "EvilEngine/CoreEngine/CoreInput/CoreInputManager.h"
#include "EvilEngine/CoreEngine/CoreBaseObject/CoreBaseObject.h"
#include "EvilEngine/CoreEngine/CoreEngine/EngineModule.h"
#include "EvilEngine/CoreGuiToolkit/CoreGuiToolkit.h"
#include "EvilEngine/CoreVersion/version.h"

#ifdef CLIENT
  #include "EvilEngine/CoreRendering/OpenGLRender/CoreOpenGLRender.h"
#endif

#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/Packets/ServerErrorNetworkPacket.h"
#include "EvilGameEngine/CoreGameEngine/CoreEffectSystem/CoreEffectSystem.h"
#include "EvilGameEngine/CoreGameEngine/CoreScreenModule/CoreScreenModule.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/GameEnginePacketData.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/Packets/BaseNetworkPacket.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/Packets/ServerErrorNetworkPacket.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/Packets/RequestServerVersionNetworkPacket.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/Packets/ReceiveServerVersionNetworkPacket.h"

#include "EvilGameEngine/CoreGameEngine/CoreGameEngineModule.h"

#include "CoreGameEngine.h"

#include <time.h>       /* time */

#if defined CLIENT
  Keyboard::Key CoreGameEngine::s_KeyboardNo = Keyboard::KEY_ESCAPE;
  Keyboard::Key CoreGameEngine::s_KeyboardYes = Keyboard::KEY_RETURN;
#endif

CoreGameEngine::CoreGameEngine() :
  m_ResourceManager( NULL ),
  m_RequestedScreenModuleId( 0 ),
  m_ScreenModule( NULL )
#ifdef CLIENT
  , m_GuiToolkit( NULL )
  , m_Render( NULL )
  , m_EffectSystem( NULL )
  , m_Framerate( 0.0f )
  , m_UpdateFPS( 0.0f )
  , m_EngineDelta( 0.0f )
  , m_NumFrames( 0 )
#endif
  , m_Shutdown( false )
{
#ifdef DEBUG
  // Guard to make sure that we are adding to the correct thread
  CoreBaseObject::s_ThreadId  = CoreUtils::GetThreadId();
#endif

  //SetInstance( this );

  // Initialize the randomizer to be fully random
  srand ( (uint32) time(NULL) );

  // Registrer all packet types for the engine
  RegisterPacketType(GameEnginePacketData::PacketData_ReceiveServerVersion, ReceivedServerVersionNetworkPacket::Create);
  RegisterPacketType(GameEnginePacketData::PacketData_RequestServerVersion, RequestServerVersionNetworkPacket::Create);
  RegisterPacketType( GameEnginePacketData::PacketData_ErrorMessage, ServerErrorNetworkPacket::Create );

  // Network error messages
  NetworkBase::RegisterNetworkErrorMessage(GameEnginePacketData::ErrorCode_Version_ServerIsTooOld, "Cannot connect to server.\nServer is too old.");
  NetworkBase::RegisterNetworkErrorMessage(GameEnginePacketData::ErrorCode_Version_ClientIsTooOld, "Cannot connect to server.\nClient is too old.");
  NetworkBase::RegisterNetworkErrorMessage(GameEnginePacketData::ErrorCode_WrongGameKey, "Cannot connect to server.\nWrong game key");
  NetworkBase::RegisterNetworkErrorMessage(GameEnginePacketData::ErrorCode_ConnectionFailed, "Cannot connect to server.\nConnection failed.");
  NetworkBase::RegisterNetworkErrorMessage(GameEnginePacketData::ErrorCode_CodeError, "Code error");
}

CoreGameEngine::~CoreGameEngine()
{
  delete m_ScreenModule;  
  m_ScreenModule = NULL;
  delete m_ResourceManager;
  m_ResourceManager = NULL;
#ifdef CLIENT
  delete m_GuiToolkit;
  m_GuiToolkit = NULL;
  delete m_EffectSystem;
  m_EffectSystem = NULL;
#endif
}

//CoreGameEngine* CoreGameEngine::GetInstance()
//{
//  return safe_cast< CoreGameEngine* > ( CoreEngine::GetInstance() );
//}

void CoreGameEngine::Shutdown()
{
  m_Shutdown = true;
}

bool CoreGameEngine::Init()
{
  if (m_NetworkBase != NULL )
  {
    return m_NetworkBase->Start();
  }
  return false;
}

const String CoreGameEngine::GetGameKey()
{
  dfBugreport(!m_GameKey.empty(),"CoreGameEngine::GetGameKey. Must be overloaded to get proper key!");
  return m_GameKey;
}

void CoreGameEngine::SetGameKey(const String& gameKey)
{
  m_GameKey = gameKey; 
}

const char* CoreGameEngine::GetBuildVersion()
{
  static const char* buildVersion = NULL;
  if ( buildVersion == NULL )
  {
    const char* ch          = BUILD_DATE;
    uint32 num              = BUILD_REVISION;
    String strBuildVersion  = String::FormatString("%s - %d", ch, num );
    return strBuildVersion.ToCharPointer();
  }
  return buildVersion;
}

/************************************************************************************************
* ChangeScreenModule:
* Request a change to switch the active screen module. Each screen module handles its own logic
* and what is rendered on screen.
*
* @param  (int)   newScreenModule - The screen module we want to switch to
*
* @author Thor Richard Hansen
*************************************************************************************************/
void CoreGameEngine::ChangeScreenModule( int newScreenModule )
{
  m_RequestedScreenModuleId = newScreenModule;
}

void CoreGameEngine::AddModuleParameter( const String& name, const Variant& val )
{
  m_ModuleParameters.AddData( name, val );
}

/************************************************************************************************
* SetRender:
* Sets the render engine in the game engine.
*
* @param  (CoreOpenGLRender)   render    - The render engine
*
* @author Thor Richard Hansen
*************************************************************************************************/
#ifdef CLIENT
void CoreGameEngine::SetRender(CoreOpenGLRender* render)
{
  if (m_Render != render)
  {
    // recreate the gui toolkit
    if (m_GuiToolkit != NULL)
    {
      RemoveEngineModule(m_GuiToolkit);
      render->RemoveRenderModule(m_GuiToolkit);
    }
  }

  if (render)
  {
    if (m_GuiToolkit == NULL)
    {
      m_GuiToolkit = CreateGuiToolkit();
      m_GuiToolkit->ResizeScreen(render->GetWidth(), render->GetHeight());
    }
    AddEngineModule(m_GuiToolkit);
    render->AddRenderModule(m_GuiToolkit);
  }

  m_Render = render;
}
#endif

/************************************************************************************************
* CreateGuiToolkit:
* Virtual function to create the correct instance of the gui toolkit
*
* @author Thor Richard Hansen
*************************************************************************************************/
#ifdef CLIENT
CoreGuiToolkit* CoreGameEngine::CreateGuiToolkit()
{
  return CoreGuiToolkit::CreateInstance(); 
}
#endif

/************************************************************************************************
* OnReceivedNetworkData:
* Received data from the network thread. Executed in the game thread
*
* @param (uint32)       connectionInstance  - ID of the connection that received the data
* @param (BinaryStream) dataStream          - The data received from the network
*
* @author Thor Richard Hansen
*************************************************************************************************/
void CoreGameEngine::OnReceivedNetworkPacket(uint32 connectionInstance, const BinaryStream* dataStream)
{
  if ( m_NetworkBase == NULL )
  {
    return;
  }

  NetworkConnection* gameConnection = m_NetworkBase->GetConnection( connectionInstance );
  if (gameConnection == NULL)
  {
    LogError("CoreGameEngine", "Connection is not a coregame network connection!");
    return;
  }

  uint32 readPtr  = dataStream->GetReadPos();
  uint32 methodId = dataStream->ReadUInt32();

  // Check if any of the modules will handle this packet.
  for (std::vector< EngineModule* >::const_iterator itModule = m_EngineModules.begin(); itModule != m_EngineModules.end(); ++itModule)
  {
    CoreGameEngineModule* module = static_cast< CoreGameEngineModule* > ( *itModule );
    if ( module != NULL )
    {
      if ( module->ReceivePacket(methodId,connectionInstance,dataStream) )
      {
        return;
      }
    }
  }

  if (!ReceivePacket(methodId, connectionInstance, dataStream) )
  {
    LogFatalFMT( "CoreGameEngine", "Calling depricated network handling for packet : %d", methodId );

    // Hack while converting the handling of network packets in gamecode instead of the network code
    dataStream->SetReadPos(readPtr);
    m_NetworkBase->OnReceivedNetworkData(connectionInstance, dataStream);

    // Hack to send to the screen module to handle network packet
    if ( m_ScreenModule != NULL )
    {
      dataStream->SetReadPos(readPtr);
      m_ScreenModule->SlotReceivedNetworkData(methodId, dataStream);
    }
  }
}

void CoreGameEngine::OnConnectionChanged(uint32 connectionInstance, uint32 conectionStatus)
{
  //for (std::vector<EngineModule*>::iterator itModule = m_EngineModules.begin(); itModule != m_EngineModules.end(); itModule++)
  //{
  //  EngineModule* engineModule = *itModule;
  //  engineModule->OnConnectionChanged(connectionInstance, conectionStatus);
  //}

  if (m_ScreenModule != NULL)
  {
    m_ScreenModule->OnConnectionChanged(connectionInstance, conectionStatus);
  }

  switch (conectionStatus)
  {
    case NetworkConnection::NETWORKSTATUS_FAILED:
    case NetworkConnection::NETWORKSTATUS_DISCONNECTED:
      RemoveConnection(connectionInstance);
      break;
    default:
      break;
  }
}

/************************************************************************************************
* FrameProcess:
* Main run loop for the game engine
*
* @param  (float)   deltaTime    - Amount of time that passed since last call.
*
* @author Thor Richard Hansen
*************************************************************************************************/
bool CoreGameEngine::FrameProcess( float deltaTime )
{
  if ( m_Shutdown )
  {
    return false;
  }

#ifdef CLIENT
  long preTime    = CoreUtils::GetTime();
#endif

  // Process network
  CoreNetworkEngine::Run();

  if (m_NetworkBase != NULL )
  {
    m_NetworkBase->Run();
  }

  // In case our parent is not listening for messages
  CoreResourceManager::GetInstance()->LoadResources();

#ifdef CLIENT

  long postTime   = CoreUtils::GetTime();

  float renderDelta = ( postTime - preTime ) / 1000.0f;

  m_NumFrames++;
  m_EngineDelta += renderDelta;
  m_UpdateFPS += deltaTime;

  if ( m_UpdateFPS > 5.0f )
  {
    m_Framerate = m_NumFrames / m_UpdateFPS;
    float percentInRender = ( 100.0f * m_EngineDelta ) / m_UpdateFPS;

    if ( m_Framerate < 50 )
    {
      LogInfoFMT("GameEngine","FPS: %0.2f [Frames: %d] [EngineTime : %0.2f - TotalTime: %0.2f ] %0.0f percent spent in rendering!", m_Framerate, m_NumFrames, m_EngineDelta, m_UpdateFPS, percentInRender );
    }

    CoreScreenModule* screenModule = GetActiveScreenModule();
    if ( screenModule != NULL )
    {
      screenModule->UpdateFPS( m_Framerate );
    }

    m_NumFrames   = 0;
    m_UpdateFPS   = 0.0f;
    m_EngineDelta = 0.0f;
  }

#endif

  return CoreEngine::FrameProcess( deltaTime );
}


#ifdef CLIENT
void CoreGameEngine::ChangeServer( int universeType )
{
  // Reonnect the main connection
  NetworkClient* networkClient = safe_cast< NetworkClient* > ( GetNetwork() );
  if ( networkClient != NULL && networkClient->HasHostType( universeType ) )
  {
    if ( m_ScreenModule != NULL )
    {
      m_ScreenModule->ClearServerVersion();
    }

    networkClient->SetCurrentHost( universeType );
    networkClient->Disconnect();
    SignalChangeServer(universeType);

    SendPacketToServer( new RequestServerVersionNetworkPacket() );
  }

  // Change the bugserver connection
}
#endif

/************************************************************************************************
* AddGuiObject:
* Adds a gui object to the gui toolkit. The gui toolkit will handle behavior and rendering
*
* @param  (CoreGuiBase*)   guiObject - The gui object to add
* @return (CoreGuiBase*)             - Returns the object that was added
*
* @author Thor Richard Hansen
*************************************************************************************************/
#ifdef CLIENT
CoreGuiBase* CoreGameEngine::AddGuiObject( CoreGuiBase* guiObject )
{
  if (m_GuiToolkit != NULL)
  {
    m_GuiToolkit->AddGuiObject( guiObject );
  }
  return guiObject;
}
#endif

/************************************************************************************************
* RemoveGuiObject:
* Remove a gui object from the gui toolkit. The gui toolkit will handle behavior and rendering
*
* @param  (CoreGuiBase*)   guiObject - The gui object to remove
*
* @author Thor Richard Hansen
*************************************************************************************************/
#ifdef CLIENT
void CoreGameEngine::RemoveGuiObject( CoreGuiBase* guiObject )
{
  if ( m_GuiToolkit != NULL )
  {
    m_GuiToolkit->RemoveGuiObject(guiObject);
  }
}
#endif

/************************************************************************************************
* RemoveAllGuiObjects:
* Removes all gui objects from the list
*
* @author Thor Richard Hansen
*************************************************************************************************/
#ifdef CLIENT
void CoreGameEngine::RemoveAllGuiObjects()
{
  if (m_GuiToolkit != NULL)
  {
    m_GuiToolkit->RemoveAllGuiObjects();
  }
}
#endif

bool CoreGameEngine::SendPacketToServer(BaseNetworkPacket* packet)
{
  NetworkClient* networkClient = safe_cast<NetworkClient*> (m_NetworkBase);
  if (networkClient != NULL)
  {
    uint32 connectionId = (networkClient->GetConnection()) ? networkClient->GetConnection()->GetConnectionId() : 0;
    return SendPacketToEndpoint(connectionId,packet);
  }
  return false;
}
bool CoreGameEngine::SendBinaryStreamToServer(BinaryStream* data)
{
  NetworkClient* networkClient = safe_cast<NetworkClient*> (m_NetworkBase);
  if (networkClient != NULL)
  {
    if (networkClient->MustReconnect())
    {
      networkClient->ConnectAsync();
    }

    uint32 connectionId = (networkClient->GetConnection()) ? networkClient->GetConnection()->GetConnectionId() : 0;
    return SendBinaryStreamToEndpoint(connectionId, data);
  }
  return false;
}
bool CoreGameEngine::SendPacketToEndpoint(uint32 connectionId, BaseNetworkPacket* packet)
{
  if (packet != NULL)
  {
#ifdef DEBUG
    LogDebugFMT("CoreGameEngine", "SendPacketToEndpoint: %s", packet->GetPacketName().c_str());
#endif

    return CoreNetworkEngine::SendPacket(connectionId, packet->GetDataStream());
  }
  return false;
}
bool CoreGameEngine::SendBinaryStreamToEndpoint(uint32 connectionId, BinaryStream* data)
{
  return CoreNetworkEngine::SendPacket(connectionId, data);
}

void CoreGameEngine::DisconnectAccountId(uint32 accountId)
{
  for (std::vector<EngineModule*>::iterator itModule = m_EngineModules.begin(); itModule != m_EngineModules.end(); itModule++)
  {
    EngineModule* engineModule = *itModule;
    if (engineModule != NULL)
    {
      engineModule->OnAccountDisconnected(accountId);
    }
  }
}

/************************************************************************************************
 * AddErrorPacket:
 * Send an error from the server to the client and display it to the user
 *
 * @param  (uint32)              connectionId   - The ID of the connection for the receiver
 * @param  (uint32)              errorCodeId    - ID of the error to send to the client or display
 *                                                to the user.
 *
 * @author Thor Richard Hansen
 *************************************************************************************************/
void CoreGameEngine::AddErrorPacket(uint32 connectionId, uint32 packetTypeId, uint32 errorCodeId)
{
  LogInfoFMT( "CoreGameEngine", "AddErrorPacket::Received error => %d/%d", packetTypeId, errorCodeId );
  SendPacketToEndpoint( connectionId, new ServerErrorNetworkPacket( packetTypeId, errorCodeId ) );
  //SignalServerError( packetTypeId, errorCodeId );
}

void CoreGameEngine::RemoveConnection( uint32 connectionId )
{
  for (std::vector<EngineModule*>::iterator itModule = m_EngineModules.begin(); itModule != m_EngineModules.end(); itModule++)
  {
    EngineModule* engineModule = *itModule;
    if ( engineModule != NULL )
    {
      engineModule->OnConnectionDisconnected( connectionId );
    }
  }
}

void CoreGameEngine::RegisterNetworkPacketCreator(uint32 packetTypeId, NetworkPacket_fp* createInstance )
{
  m_PacketCreator[ packetTypeId ] = createInstance;
}

void CoreGameEngine::RegisterPacketType(uint32 packetTypeId, NetworkPacket_fp* createInstance )
{
  m_PacketCreator[ packetTypeId ] = createInstance;
}

bool CoreGameEngine::ReceivePacket( uint32 packetTypeId, uint32 connectionInstance, const BinaryStream* dataStream )
{
  std::map< uint32, NetworkPacket_fp* >::iterator itFind = m_PacketCreator.find(packetTypeId);
  if (itFind != m_PacketCreator.end())
  {
    BaseNetworkPacket* packet = itFind->second( dataStream );
    if (packet != NULL)
    {
#ifdef DEBUG
      LogDebugFMT("CoreGameEngine", "ReceivePacket: %s", packet->GetPacketName().c_str());
#endif
      if ( packet->ReadDataStream(dataStream) )
      {
        packet->SetGameEngine( this );
        packet->SetConnectionId(connectionInstance);

        dfBugreportFMT(packet->GetPacketType() == packetTypeId, "Packet %s is spawned with type %d but is type %d", packet->GetPacketName().c_str(), packetTypeId, packet->GetPacketType());

        // Handle packet right away. TODO. queue it for the right thread.
        if (packet->CanExecute())
        {
          packet->Execute();
        }
      }
      else
      {
        dfBugreportFMT( false, "Failed to read binarystream for packet %d/%s", packetTypeId, packet->GetPacketName().c_str() );
      }

      delete packet;
      return true;
    }
  }
  return false;
}

