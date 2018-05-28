#ifndef PROJECT_PROJECTGAMEENGINE_TILES_HEXTILE
#define PROJECT_PROJECTGAMEENGINE_TILES_HEXTILE 1
#pragma once

#include "BaseTile.h"

class BinaryStream;
class MultiFaceRenderObject;

class HexTile : public BaseTile
{
public:

  static float const HEIGHT;
  static float const FLAT;
  static float const DIMMED;

  enum RenderType
  {
    GroundRender  = 1,
    WallRender    = 2,
  };

  enum DOORS
  {
    DOOR_NONE       = 0,
    DOOR_NORTH      = 1<<0,
    DOOR_NORTHEAST  = 1<<1,
    DOOR_SOUTHEAST  = 1<<2,
    DOOR_SOUTH      = 1<<3,
    DOOR_SOUTHWEST  = 1<<4,
    DOOR_NORTHWEST  = 1<<5,
    DOOR_ALL        = HexTile::DOOR_NORTH | HexTile::DOOR_NORTHEAST | HexTile::DOOR_SOUTHEAST | HexTile::DOOR_SOUTH | HexTile::DOOR_SOUTHWEST| HexTile::DOOR_NORTHWEST,
  };

  HexTile();
  HexTile( uint32 resourceId, uint32 wallResourceId );
  virtual ~HexTile();

  static HexTile*                 CreateFromData( const BinaryStream* data );
  virtual void                    ReadData( const BinaryStream* data )          DF_OVERRIDE;
  virtual void                    WriteData( BinaryStream& data ) const         DF_OVERRIDE;

  virtual uint8                   GetNumRotations() const                       DF_OVERRIDE;
  virtual void                    SetNumRotations( uint8 numRotations )         DF_OVERRIDE;
  virtual int                     GetNumSides()                                 DF_OVERRIDE;
  virtual int                     GetSideLocation( const Vector2& worldGridPosition ) DF_OVERRIDE;

  virtual Vector2                 GetLocalGridPosFromSide( int side )           DF_OVERRIDE;
  virtual Vector2                 GetWorldGridPosFromSide( int side )           DF_OVERRIDE;
  virtual int                     GetConnectedDoor( uint32 door )               DF_OVERRIDE;
  virtual bool                    HasConnectedDoor( uint32 door )               DF_OVERRIDE;

  Vector3                         GetWorldPos( const Vector2& gridPos );
  Vector2                         GetCorner( int side );

#if defined CLIENT

  virtual bool                    OnDraw( const Matrix& projection )                DF_OVERRIDE;
  virtual void                    SetPosition( float x, float y, float z )          DF_OVERRIDE;
  virtual void                    SetSize( float x, float y, float z )              DF_OVERRIDE;
  virtual void                    SetRotation( float x, float y, float z )          DF_OVERRIDE;
  virtual void                    SetResourceId( uint32 resourceId )                DF_OVERRIDE;
  virtual uint32                  GetResourceId() const                             DF_OVERRIDE;
  virtual void                    SetColor( float red, float green, float blue )    DF_OVERRIDE;
  virtual void                    SetWallColor( const Color& color )                DF_OVERRIDE;

  virtual void                    ShowWalls( uint32 resourceId, const Color& color) DF_OVERRIDE;

#endif

  static bool                     s_ShowWalls;

protected:

  void                            InitRender( uint32 resourceId, uint32 wallResourceId );
};

#endif /// PROJECT_PROJECTGAMEENGINE_TILES_HEXTILE