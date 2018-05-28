#ifndef PROJECT_PROJECTGAMEENGINE_TILES_BASETILE
#define PROJECT_PROJECTGAMEENGINE_TILES_BASETILE 1
#pragma once

#include "EvilEngine/CoreLib/CoreMath/CoreMath.h"
#include "EvilGameEngine/CoreBoardGame/GameEngine/CoreBaseRenderObject.h"

#include <map>

class BinaryStream;

class BaseTile : public CoreBaseRenderObject
{
public:

  enum TILE_TYPE
  {
    TILETYPE_UNKNOWN,
    TILETYPE_QUAD,
    TILETYPE_HEX,
  };

  BaseTile();
  BaseTile( uint8 tileType, uint32 resourceId );
  virtual ~BaseTile();

  uint32                          GetWallResourceId(){ return 0; }

  void                            SetGridPosition( int x, int y ){ m_GridPosition.Set( (float)x, (float)y ); }
  void                            SetGridPosition( const Vector2& other ){ m_GridPosition.Set( other.x, other.y ); }
  const Vector2&                  GetGridPosition() const { return m_GridPosition; }
  Vector2                         GetGridPosition(){ return m_GridPosition; }

  uint8                           GetType(){ return m_TileType; }
  uint32                          GetOriginalResourceId() const { return m_OriginalResourceId; }
  void                            SetOriginalResourceId( uint32 resourceId ){ m_OriginalResourceId = resourceId; }

  virtual uint8                   GetNumRotations() const{ return 0; }
  virtual void                    SetNumRotations( uint8 /*numRotations*/ ){};
  virtual int                     GetNumSides(){ return 0; }
  virtual Vector2                 GetLocalGridPosFromSide( int /*side*/ ){ return Vector2::s_Vec2Minus; }
  virtual int                     GetSideLocation( const Vector2& /*gridPosition*/ ){ return 0; }

  virtual Vector2                 GetWorldGridPosFromSide( int /*side*/ ){ return Vector2::s_Vec2Minus; }

  uint32                          GetDoors() const{ return m_Doors;}
  uint32                          GetRotatedDoors() const{ return m_RotatedDoors;}
  void                            SetDoors( uint32 doors ){ m_Doors = doors; m_RotatedDoors = doors; }
  void                            SetRotatedDoors( uint32 doors ){ m_RotatedDoors = doors; }
  bool                            HasDoor( uint32 door ){ return ( m_RotatedDoors & door ) == door; }
  void                            RotateDoors( uint8 rotation );

  virtual int                     GetConnectedDoor( uint32 /*door*/ ){ return 0; }
  virtual bool                    HasConnectedDoor( uint32 /*door*/ ){ return false; }

  virtual void                    ReadData( const BinaryStream* data );
  virtual void                    WriteData( BinaryStream& data ) const;
  static BaseTile*                CreateFromData( const BinaryStream* data );

//#if defined CLIENT
  virtual void                    ShowWalls( uint32 /*resourceId*/, const Color& /*color*/ ){}
  virtual void                    SetWallColor( const Color& /*color*/ ){}
//#endif

protected:

  uint8                           m_TileType;
  uint32                          m_OriginalResourceId;
  Vector2                         m_GridPosition;
  uint32                          m_Doors;
  uint32                          m_RotatedDoors;

#if defined CLIENT
  void                            RemoveWalls();
  std::map< int, RenderObject* >  m_Walls;
#endif
};

#endif /// PROJECT_PROJECTGAMEENGINE_TILES_BASETILE