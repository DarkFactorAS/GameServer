#ifndef EVILENGINE_COREBOARDGAME_GAMELOGIC_GAMEOBJECT
#define EVILENGINE_COREBOARDGAME_GAMELOGIC_GAMEOBJECT 1
#pragma once

#include "EvilGameEngine/CoreBoardGame/GameEngine/CoreBaseRenderObject.h"

class RenderObject;
class BinaryStream;
class GridTilePlayfield;

class GameObject : public CoreBaseRenderObject
{
public:

  enum GameObjectType
  {
    e_Character,
    e_Item,
  };

  GameObject();
  GameObject( uint32 id, uint32 type, String name, uint32 iconId, const Color& color );
  GameObject( uint32 id, uint32 type, String name, uint32 iconId, const Color& color, const Vector3& position, const Vector3& size );
  virtual ~GameObject();

  virtual GameObject*             Clone() const;

  void                            CreateRenderObject();

  const Vector2&                  GetGridPosition() const { return m_GridPosition; }
  void                            SetGridPosition( const Vector2& position ){ m_GridPosition = position; }
  void                            MoveCameraToMe() const;
  void                            SlideCameraToMe() const;

  uint32                          GetId() const{ return m_Id; }
  void                            SetId(uint32 id) { m_Id = id; }
  uint32                          GetType() const{ return m_Type; }
  const String&                   GetName() const{ return m_Name; }
  uint32                          GetIconId() const{ return m_IconId; }
  void                            SetIconId( uint32 iconId );
  const Color&                    GetColor() const{ return m_Color; }

  bool                            IsCharacter() const{ return m_Type == e_Character; }
  bool                            IsItem() const{ return m_Type == e_Item; }

  virtual void                    ReadData( const BinaryStream* data );
  virtual void                    WriteData( BinaryStream& data ) const;

  static Vector3                  GameObjectSize;

  void                            SetDiscovered( bool isDiscovered ){ m_IsDiscovered = isDiscovered; }
  bool                            IsDiscovered() const{ return m_IsDiscovered; }

  GridTilePlayfield*              GetPlayfield() const{ return m_Playfield;}
  void                            SetPlayfield( GridTilePlayfield* playfield ){ m_Playfield = playfield; }

protected:

  GameObject( const GameObject* otherObj );
  static uint32                   s_Id;

private:

  uint32                          m_Id;
  uint32                          m_Type;
  String                          m_Name;
  uint32                          m_IconId;
  Color                           m_Color;
  Vector2                         m_GridPosition;
  bool                            m_IsDiscovered;
  GridTilePlayfield*                  m_Playfield;
};

#endif /// EVILENGINE_COREBOARDGAME_GAMELOGIC_GAMEOBJECT
