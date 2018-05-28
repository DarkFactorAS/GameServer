#ifndef EVILGAMEENGINE_COREBOARDGAME_PLAYFIELD_TILEPLAYFIELD
#define EVILGAMEENGINE_COREBOARDGAME_PLAYFIELD_TILEPLAYFIELD 1
#pragma once

#include "EvilEngine/CoreLib/BasicTypes/BinaryStream/BinaryStream.h"
#include "EvilEngine/CoreLib/BasicTypes/FixedMaps/FixedMultimap.h"
#include "EvilEngine/CoreEngine/CoreEngine/EngineModule.h"

#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"
#include "EvilGameEngine/CoreBoardGame/Playfield/PlayfieldInfo.h"

#if defined CLIENT
  #include "EvilEngine/CoreRendering/RenderModule.h"
#endif 

#if defined CLIENT
  #include "EvilEngine/CoreLib/BasicTypes/FixedMaps/FixedSizeVector.h"
#endif

class BaseTile;
class RenderObject;

#if defined CLIENT
class GridTilePlayfield : public EngineModule, public RenderModule
#else
class GridTilePlayfield : public EngineModule
#endif
{
public:
  
  enum ModuleType
  {
    RenderModule_Playfield = 2
  };

  enum PLAYFIELD_TYPE
  {
    PLAYFIELDTYPE_UNKNOWNN,
    PLAYFIELDTYPE_QUAD,
    PLAYFIELDTYPE_HEX,
  };

  GridTilePlayfield( uint32 playfieldId, const String& playfieldName, uint8 playfieldType, uint32 ownerId, uint16 sizeGridX, uint16 sizeGridY );
  ~GridTilePlayfield();

  virtual String                  GetModuleName() DF_OVERRIDE { return StaticStr("GridTilePlayfield"); }

#if defined CLIENT
  // Implementation from RenderModule ( to draw all tiles on the board )
  virtual void                    ResizeScreen( int /*width*/, int /*height*/ ) DF_OVERRIDE {}
#endif

  virtual void                    FrameProcess( float deltaTime ) DF_OVERRIDE;

  BaseTile*                       AddTile( BaseTile* tile, int gridPosX, int gridPosY, int rotation );
  BaseTile*                       AddTile( uint32 resourceId, uint32 wallResourceId, int gridPosX, int gridPosY );
  BaseTile*                       AddTile( uint32 resourceid, uint32 wallResourceId, int gridPosX, int gridPosY, int rotation );
  BaseTile*                       CreateTile( uint32 resourceId, const Vector3& position, int yRotation );

  virtual bool                    IsEmpty( const Vector2& /*gridPosition*/ ) const { return false; }
  virtual bool                    IsEmpty( const BaseTile* /*tile*/ ) const { return false; }

  void                            RemoveTile( BaseTile* tile );
  BaseTile*                       GetTile( const Vector2& gridPosition ) const;
  BaseTile*                       GetTile( int gridPosX, int gridPosY ) const;
  BaseTile*                       GetTileWithWorldPosition( float worldPosX, float worldPosY );
  Vector3                         GetTilePosition( int gridPosX, int gridPosY );

  void                            Center();
  void                            RepositionTiles();

#if defined CLIENT
  virtual void                    OnDraw( const Matrix& projection )                          DF_OVERRIDE;
  
  void                            AddRenderObject( RenderObject* renderObject );
  void                            RemoveRenderObject( RenderObject* renderObject );
  bool                            HasRenderObject( RenderObject* renderObject );
#endif

  const String&                   GetPlayfieldName() const{ return m_PlayfieldName; }

  static GridTilePlayfield*       LoadFromDatabase( uint32 playfieldId );
  static PlayfieldInfoMap         LoadPlayfieldListFromDatabase( uint32 playerId );
  static bool                     DeleteFromDatabase( uint32 ownerId, uint32 playfieldId );

  static GridTilePlayfield*       CreateFromData( const BinaryStream* data );

  bool                            SaveToDatabase();
  virtual void                    CreateGroundTiles( uint32 /*resourceId*/ );

  uint32                          GetPlayfieldId() const{ return m_PlayfieldId; }
  void                            SetPlayfieldId( uint32 playfieldId ){ m_PlayfieldId = playfieldId; }
  void                            WriteData( BinaryStream& data ) const;

  void                            SetOwnerId( uint32 ownerId ){ m_OwnerId = ownerId; }
  void                            SetName( const String& name ){ m_PlayfieldName = name; }

  uint16                          GetGridSizeX() const{ return m_GridSizeX; }
  uint16                          GetGridSizeY() const{ return m_GridSizeY; }
  float                           GetZoom() const{ return m_Zoom; }

  static void                     FlipCamera( bool flipCamera){ s_FlipCamera = flipCamera; }
  static bool                     s_FlipCamera;

  virtual void                      SetDefaultTileColor(BaseTile* /*tile*/){}


#if defined CLIENT
  virtual bool                    OnMouseButtonDown(int /*buttonId*/, float /*xPos*/, float /*yPos*/)   DF_OVERRIDE { return false; }
  virtual bool                    OnMouseButtonUp(int /*buttonId*/, float /*xPos*/, float /*yPos*/)     DF_OVERRIDE { return false; }
  virtual void                    OnMouseMove(float /*xPos*/, float /*yPos*/)                           DF_OVERRIDE {}
#endif

protected:

  static GridTilePlayfield*       CreatePlayfield( uint32 playfieldId, const String& playfieldName, uint32 ownerId, uint8 playfieldType, uint16 sizeGridX, uint16 sizeGridY );

  int                             m_PlayfieldId;
  uint8                           m_PlayfieldType;
  String                          m_PlayfieldName;

  float                           m_Zoom;
  Vector2                         m_Offset;
  Vector2                         m_ActiveCell;
  bool                            m_RepositionTiles;
 
  Vector2*                        m_LastMousePos;
  Vector2*                        m_MouseDownPos;

  FixedMultimap< BaseTile >       m_TileMap;

  bool                            m_EnableMoveBoard;
  bool                            m_EnableCamera;
  bool                            m_EnableMoveTile;

  uint16                          m_GridSizeX;
  uint16                          m_GridSizeY;

  uint32                          m_OwnerId;

#if defined CLIENT
  FixedSizeVector< RenderObject* >  m_DynamicObjects;
#endif
};

#endif /// EVILGAMEENGINE_COREBOARDGAME_PLAYFIELD_TILEPLAYFIELD
