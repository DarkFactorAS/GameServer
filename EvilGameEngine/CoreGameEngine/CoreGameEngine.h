#ifndef EVILENGINE_COREGAMEENGINE_COREGAMEENGINE
#define EVILENGINE_COREGAMEENGINE_COREGAMEENGINE 1
#pragma once

#include "EvilEngine/CoreLib/CoreMath/Vector2.h"
#include "EvilEngine/CoreLib/BasicTypes/Bundle/Bundle.h"
#include "EvilEngine/CoreEngine/CoreEngine/CoreEngine.h"
#include "EvilEngine/CoreLib/Signal/SignalTarget.h"
#include "EvilEngine/CoreLib/Signal/Signal.h"
#include "EvilEngine/CoreNetwork/Engine/CoreNetworkEngine.h"

#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"

#if defined CLIENT
  #include "EvilEngine/CoreEngine/CoreInput/Keyboard.h"
#endif

#include <map>
#include <vector>
#include <list>

class EngineModule;
class Vector2;
class CoreOpenGLRender;
class CoreResourceManager;
class NetworkBase;
class CoreScreenModule;
class CoreGuiToolkit;
class AccountManager;
class CoreGuiBase;
class String;
class Variant;
class CoreScreenModule;
class CoreEffectSystem;
class BaseNetworkPacket;
class NetworkConnection;
class BinaryStream;

class CoreGameEngine : public CoreNetworkEngine
{
public:

  enum MESSAGE_TYPE
  {
    MESSAGE_PURCHASE_REFRESH      = 10,
    MESSAGE_PURCHASE_MEMBERSHIP   = 11,
    MESSAGE_PURCHASE_PLAYFIELD1   = 12,
    MESSAGE_AMAZONLOGIN           = 20,
    MESSAGE_PLATFORM_LOGIN        = 21,
  };

  CoreGameEngine();
  virtual ~CoreGameEngine();
//  static CoreGameEngine*            GetInstance();
  virtual bool                      Init() DF_OVERRIDE;
  virtual void                      Shutdown() DF_OVERRIDE;

  virtual bool                      FrameProcess( float deltaTime ) DF_OVERRIDE;

  virtual void                      OnReceivedNetworkData(uint32 /*connectionInstance*/, const BinaryStream* /*dataStream*/) DF_FINAL {};
  virtual void                      OnReceivedNetworkPacket(uint32 /*connectionInstance*/, const BinaryStream* /*dataStream*/) DF_OVERRIDE;
  virtual void                      OnConnectionChanged(uint32 /*connectionInstance*/, uint32 /*connectionStatus*/) DF_OVERRIDE;
  void                              DisconnectAccountId(uint32 accountId);

  virtual const String              GetGameKey();
  void                              SetGameKey(const String& gameKey);

  CoreScreenModule*                 GetActiveScreenModule(){ return m_ScreenModule; }

  CoreResourceManager*              GetResourceManager(){ return m_ResourceManager; }
  NetworkBase*                      GetNetwork() const { return m_NetworkBase; }

  void                              ChangeScreenModule( int newScreenModule );
  void                              AddModuleParameter( const String& name, const Variant& val );

  bool                              SendPacketToClient(uint32 connectionId, BaseNetworkPacket* packet);
  bool                              SendPacketToServer( BaseNetworkPacket* packet);
  bool                              SendBinaryStreamToServer(BinaryStream* data);
  bool                              SendBinaryStreamToEndpoint(uint32 connectionId, BinaryStream* data);
  // Depricated
  bool                              SendPacketToEndpoint(uint32 connectionId, BaseNetworkPacket* packet);

  void                              AddErrorPacket(uint32 connectionId, uint32 packetTypeId, uint32 errorCodeId );
  void                              RemoveConnection( uint32 connectionId );

  typedef BaseNetworkPacket*        NetworkPacket_fp( const BinaryStream* );
  void                              RegisterNetworkPacketCreator(uint32 packetTypeId, NetworkPacket_fp* createInstance );
  void                              RegisterPacketType(uint32 packetTypeId, NetworkPacket_fp* createInstance );
  bool                              ReceivePacket( uint32 packetTypeId, uint32 connectionInstance, const BinaryStream* dataStream );
  
#ifdef CLIENT
  
  CoreOpenGLRender*                 GetRender() const{ return m_Render; }
  virtual void                      SetRender(CoreOpenGLRender* render);

  // TODO : Make these more abstract since they are EngineModules
  CoreGuiToolkit*                   GetGuiToolkit() const{ return m_GuiToolkit; }
  virtual CoreGuiToolkit*           CreateGuiToolkit();
  CoreEffectSystem*                 GetEffectSystem() { return m_EffectSystem; }

  // Gui input
  CoreGuiBase*                      AddGuiObject( CoreGuiBase* guiObject );
  void                              RemoveGuiObject( CoreGuiBase* guiObject );
  void                              RemoveAllGuiObjects();


  // Item Purchase
  Signal1< void, uint32 >           SignalPurchasedItem;
  Signal1<void, int>                SignalChangeServer;

  static Keyboard::Key              KeyboardNo(){ return s_KeyboardNo; }
  static Keyboard::Key              KeyboardYes(){ return s_KeyboardYes; }

  virtual void                      ChangeServer( int universeType );  

  Signal2< void, uint32, uint32 >   SignalServerError;  /// PacketTypeId, ErrorCodeId

#endif

  const char*                       GetBuildVersion();

protected:

  // Holder classes
  CoreResourceManager*              m_ResourceManager;

  int                               m_RequestedScreenModuleId;
  Bundle                            m_ModuleParameters;
  CoreScreenModule*                 m_ScreenModule;

#ifdef CLIENT
  CoreGuiToolkit*                   m_GuiToolkit;
  CoreOpenGLRender*                 m_Render;
  CoreEffectSystem*                 m_EffectSystem;

  float                             m_Framerate;
  float                             m_UpdateFPS;
  float                             m_EngineDelta;
  int                               m_NumFrames;

  static Keyboard::Key              s_KeyboardNo;
  static Keyboard::Key              s_KeyboardYes;

#endif

  std::map< uint32, NetworkPacket_fp* > m_PacketCreator;
  std::map< uint32, const char* >   m_NetworkErrorMessages;

  bool                              m_Shutdown;
  String                            m_GameKey;
};

#endif /// EVILENGINE_COREGAMEENGINE_COREGAMEENGINE
