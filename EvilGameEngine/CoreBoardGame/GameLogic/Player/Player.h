#ifndef EVILENGINE_COREBOARDGAME_GAMELOGIC_PLAYER
#define EVILENGINE_COREBOARDGAME_GAMELOGIC_PLAYER 1
#pragma once

#include "EvilGameEngine/CoreBoardGame/GameEngine/CoreBaseRenderObject.h"
#include "EvilGameEngine/CoreBoardGame/GameLogic/Character/Character.h"

class RenderObject;
class BinaryStream;

class Player : public Character
{
public:
  Player();
  Player( String playerName, uint32 characterType, uint32 iconId, const Color& color );
  Player( String playerName, uint32 characterType, uint32 iconId, const Color& color, Vector3 position, Vector3 size, uint32 resourceId );

  virtual ~Player(){};

  static Player*                  CreateFromData( const BinaryStream* data );

  virtual void                    ReadData( const BinaryStream* data )    DF_OVERRIDE;
  virtual void                    WriteData( BinaryStream& data ) const   DF_OVERRIDE;

  void                            SetAccountId( uint32 accountId ){ m_AccountId = accountId; }
  uint32                          GetAccountId() const{ return m_AccountId; }

  uint32                          GetAllPlayerFlags(){ return m_PlayerFlags;}
  bool                            HasPlayerFlag(int flag) const { return (m_PlayerFlags & flag) != 0; }
  void                            SetPlayerFlag(int flag) { m_PlayerFlags |= flag; }
  void                            ClearPlayerFlag(int flag) { m_PlayerFlags &= ~flag; }
  uint32                          GetAllPlayerGuiFlags() { return m_PlayerGuiFlags; }
  bool                            HasPlayerGuiFlag(int flag) const { return (m_PlayerGuiFlags & flag) != 0; }
  void                            SetPlayerGuiFlag(int flag) { m_PlayerGuiFlags |= flag; }
  void                            ClearPlayerGuiFlag(int flag) { m_PlayerGuiFlags &= ~flag; }

  bool                            IsPlayer() const{ return m_IsPlayer; }
  bool                            IsNpc() const{ return !m_IsPlayer; }
  void                            SetPlayer( bool isPlayer ){ m_IsPlayer = isPlayer; }

  bool                            IsClient() const{ return m_IsClient; }
  void                            SetClient( bool isClient ){ m_IsClient = isClient; }

  static Color                    Player1Color;
  static Color                    Player2Color;
  static Vector3                  PlayerSize;
  static const float              Jumpspeed;
  static const float              Teleportspeed;
  static const float              JumpWaitspeed;

private:

  uint32                          m_AccountId;
  bool                            m_IsPlayer;
  bool                            m_IsClient;
  uint32                          m_PlayerFlags;
  uint32                          m_PlayerGuiFlags;
};

#endif /// EVILENGINE_COREBOARDGAME_GAMELOGIC_PLAYER
