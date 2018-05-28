#ifndef EVILENGINE_COREBOARDGAME_GAMELOGIC_CHARACTER
#define EVILENGINE_COREBOARDGAME_GAMELOGIC_CHARACTER 1
#pragma once

#include "EvilGameEngine/CoreBoardGame/GameEngine/CoreBaseRenderObject.h"
#include "EvilGameEngine/CoreBoardGame/GameLogic/GameObject/GameObject.h"

class RenderObject;
class BinaryStream;

class Character : public GameObject
{
public:

  enum CharacterType
  {
    CharacterType_None = 0,
    CharacterType_Monster,
    CharacterType_MissionNpc,
    CharacterType_Player,
    CharacterType_Computer,
  };

  Character();
  Character( String characterName, uint32 characterType, uint32 iconId, const Color& color );
  Character( String characterName, uint32 characterType, uint32 iconId, const Color& color, const Vector3& position, const Vector3& size );
  virtual ~Character(){};

  virtual GameObject*             Clone() const DF_OVERRIDE;

  uint32                          GetCharacterId() const{ return m_CharacterId; }
  void                            SetCharacterId( uint32 characterId ){ m_CharacterId = characterId; }

  virtual void                    ReadData( const BinaryStream* data )    DF_OVERRIDE;
  virtual void                    WriteData( BinaryStream& data ) const   DF_OVERRIDE;

  const Color&                    GetCharacterColor() const{ return GetColor(); }
  bool                            IsPlayerCharacter() const{ return m_CharacterType == CharacterType_Player; }
  bool                            IsPlayerComputer() const{ return m_CharacterType == CharacterType_Computer; }
  bool                            IsMonster() const{ return m_CharacterType == CharacterType_Monster; }
  bool                            IsMissionNpc() const{ return m_CharacterType == CharacterType_MissionNpc; }
  void                            SetMonster(){ m_CharacterType = CharacterType_Monster; }

  uint32                          GetCharacterType() const{ return m_CharacterType; }
  virtual uint32                  GetWalkDistance() const{ return m_WalkDistance; }

  void                            SetOpponent( uint32 opponentId ){ m_Opponent = opponentId; }
  uint32                          GetOpponent() const{ return m_Opponent; }

protected:

  Character( const Character* other );
  uint32                          m_WalkDistance;

private:

  uint32                          m_CharacterId;
  uint32                          m_CharacterType;
  uint32                          m_Opponent;

  static uint32                   s_CharacterId;
};

#endif /// EVILENGINE_COREBOARDGAME_GAMELOGIC_CHARACTER
