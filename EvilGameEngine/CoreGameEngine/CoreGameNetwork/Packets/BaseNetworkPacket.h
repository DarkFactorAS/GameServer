#pragma once

#include "EvilEngine/CoreLib/BasicTypes/BinaryStream/BinaryStream.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"

class CoreGameEngineModule;

class BaseNetworkPacket
{
public:

  BaseNetworkPacket( uint32 packetType, const BinaryStream* datastream );
  BaseNetworkPacket( uint32 packetType );

  virtual ~BaseNetworkPacket(){}
      
  uint32                        GetPacketType() const { return m_PacketType; }
  virtual BinaryStream*         GetDataStream();
  virtual bool                  ReadDataStream( const BinaryStream* /*datastream*/ ){ return true; }
  virtual String                GetPacketName() = 0;
  virtual void                  Execute();
  virtual bool                  CanExecute(){ return true; }
  virtual void                  OnError(){};

  void                          SetGameEngine( CoreGameEngine* gameengine ){ m_GameEngine = gameengine; }
  CoreGameEngine*               GetGameEngine(){ return m_GameEngine; }
  EngineModule*                 GetEngineModule(int moduleId);

  void                          SetGameEngineModule( CoreGameEngineModule* module ){ m_GameEngineModule = module; }

  void                          SetConnectionId( uint32 connectionId ){ m_ConnectionId = connectionId; }

  void                          SendPacketToServer(CoreGameEngine& engine, BaseNetworkPacket* packet );
  void                          SendPacketToServer(BaseNetworkPacket* packet);
  void                          SendPacketToClient(BaseNetworkPacket* packet);
  void                          SendErrorToClient(uint32 errorId);
  String                        GetIpAddress();

protected:

  uint32                        m_PacketType;
  uint32                        m_ConnectionId;
  CoreGameEngine*               m_GameEngine;
  CoreGameEngineModule*         m_GameEngineModule;
};
