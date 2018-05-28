/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : TilePlayfield
*
* Description : Holds all the elements for a playfield or board where the ground is tile based.
*               For boardgames the tiles are squared or hex shaped and tiled next to eachother
*               to make a seamless world/ground.
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "GridTilePlayfield.h"

#include <stdlib.h>

#include "EvilEngine/CoreLib/Utils/CoreUtils.h"
#include "EvilEngine/CoreIO/File/IOFile.h"
#include "EvilEngine/CoreIO/Logger/Logger.h"

#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"
#include "EvilGameEngine/CoreBoardGame/Playfield/Tiles/BaseTile.h"
#include "EvilGameEngine/CoreBoardGame/Playfield/Tiles/HexTile.h"
#include "EvilGameEngine/CoreBoardGame/Playfield/Tiles/QuadTile.h"

#ifdef SERVER
  #include "EvilEngine/CoreDatabase/CoreDatabase.h"
#else
  #include "EvilEngine/CoreRendering/RenderObjects/RenderObject.h"
  #include "EvilEngine/CoreRendering/OpenGLRender/CoreOpenGLRender.h"
  #include "EvilEngine/CoreRendering/Camera/RenderCamera.h"
  #include "EvilEngine/CoreGuiToolkit/CoreGuiToolkit.h"
  #include "EvilEngine/CoreEngine/CoreInput/CoreInputManager.h"
#endif

bool GridTilePlayfield::s_FlipCamera = false;

GridTilePlayfield::GridTilePlayfield( uint32 playfieldId,  const String& playfieldName, uint8 playfieldType, uint32 ownerId, uint16 gridSizeX, uint16 gridSizeY ) :
  EngineModule( EngineModule::COREMODULETYPE_GRIDPLAYFIELD ),
#if defined CLIENT
  RenderModule( RenderModule_Playfield ),
#endif
  m_PlayfieldId( playfieldId ),
  m_PlayfieldType( playfieldType ),
  m_PlayfieldName( playfieldName ),
  m_Zoom( 0.1f ),
  m_RepositionTiles( false ),
  m_LastMousePos( NULL ),
  m_TileMap( gridSizeX, gridSizeY, true ),
  m_EnableMoveBoard( false ),
  m_EnableCamera( false ),
  m_EnableMoveTile( false ),
  m_GridSizeX( gridSizeX ),
  m_GridSizeY( gridSizeY ),
  m_OwnerId( ownerId )

#if defined CLIENT
  , m_DynamicObjects( 100, false )
#endif
{
}

GridTilePlayfield::~GridTilePlayfield()
{
#if defined CLIENT
  //for ( uint32 index = 0; index < m_DynamicObjects.GetSize(); index++ )
  //{
  //  RenderObject* renderObj = m_DynamicObjects.Get( index );
  //  delete renderObj;
  //}
  m_DynamicObjects.Clear();
#endif
}

/************************************************************************************************
* AddTile:
* Adds a tile to the playfield at a specific position
*
* @param  (uint32)     resourceId     - ID of the resource to use on the top surface of the tile
* @param  (uint32)     wallResourceId - ID of the resource to use on the side surfaces of the tile
* @param  (int)        gridPosX       - X Position on the playfield
* @param  (int)        gridPosY       - Y Position on the playfield
* @return (BaseTile*)                 - The tile that was created/added to the playfield
*
* @author Thor Richard Hansen
*************************************************************************************************/
BaseTile* GridTilePlayfield::AddTile(uint32 resourceId, uint32 wallResourceId, int gridPosX, int gridPosY)
{
  return AddTile( resourceId, wallResourceId, gridPosX, gridPosY, 0 );
}

/************************************************************************************************
* AddTile:
* Adds a tile to the playfield at a specific position
*
* @param  (uint32)     resourceId     - ID of the resource to use on the top surface of the tile
* @param  (uint32)     wallResourceId - ID of the resource to use on the side surfaces of the tile
* @param  (int)        gridPosX       - X Position on the playfield
* @param  (int)        gridPosY       - Y Position on the playfield
* @param  (int)        rotation       - Y-Rotation of the tile
* @return (BaseTile*)                 - The tile that was created/added to the playfield
*
* @author Thor Richard Hansen
*************************************************************************************************/
BaseTile* GridTilePlayfield::AddTile(uint32 resourceId, uint32 wallResourceId, int gridPosX, int gridPosY, int rotation )
{
  switch( m_PlayfieldType )
  {
    case PLAYFIELDTYPE_QUAD:
      return GridTilePlayfield::AddTile(new SquareTile(resourceId), gridPosX, gridPosY, rotation);
    case PLAYFIELDTYPE_HEX:
      return GridTilePlayfield::AddTile(new HexTile(resourceId, wallResourceId), gridPosX, gridPosY, rotation);
    default:
      return NULL;
  }
}

/************************************************************************************************
* AddTile:
* Adds a tile to the playfield at a specific position
*
* @param  (BaseTile*)   tile        - The tile to add to the playfie;d
* @param  (int)         gridPosX    - X Position on the playfield
* @param  (int)         gridPosY    - Y Position on the playfield
*
* @author Thor Richard Hansen
*************************************************************************************************/
BaseTile* GridTilePlayfield::AddTile( BaseTile* tile, int gridPosX, int gridPosY, int rotation )
{
  if ( tile == NULL )
  {
    return tile;
  }

  BaseTile* oldTile = GetTile( gridPosX, gridPosY );
  if ( oldTile != NULL )
  {
    oldTile->SetResourceId( tile->GetResourceId() );
    oldTile->SetOriginalResourceId( tile->GetResourceId() );

    const Vector3& oldRotation = oldTile->GetRotation();
    oldTile->SetRotation( (float)rotation, oldRotation.y, oldRotation.z );
    delete tile;
    return oldTile;
  }

  tile->SetSize( m_Zoom, m_Zoom, 0.0f );    
  tile->SetPosition( m_Zoom * 2 * gridPosX, m_Zoom * 2 * gridPosY, 0.0f );
  tile->SetGridPosition( gridPosX, gridPosY );

  m_TileMap.Add( gridPosX, gridPosY, tile );

  return tile;
}

/************************************************************************************************
* CreateTile:
* Create a QUAD tile with the correct resource/position
*
* @param  (uint32)          resourceId  - ID of the texture/tiletype to use
* @param  (const Vector3&)  position    - 3D world position of the tile
* @param  (int)             yRotation   - How many rotations to rotate the tile ( quad has 4
*                                         rotations for a full rotate )
* @return (BaseTile*)                   - The new quad tile that was created
*
* @author Thor Richard Hansen
*************************************************************************************************/
BaseTile* GridTilePlayfield::CreateTile(uint32 resourceId, const Vector3& position, int yRotation)
{
  SquareTile* tile = new SquareTile(resourceId);
  tile->SetPosition(position.x, position.y, position.z);
  tile->SetRotation(0, 0, yRotation * 90.0f);
  return tile;
}


void GridTilePlayfield::RemoveTile( BaseTile* removeTile )
{
  if ( removeTile != NULL )
  {
    m_TileMap.Remove( removeTile->GetGridPosition().GetIntX(), removeTile->GetGridPosition().GetIntY(), removeTile );
  }
}

BaseTile* GridTilePlayfield::GetTileWithWorldPosition( float /*worldPosX*/, float /*worldPosY*/ )
{
  //const std::vector< BaseTile* >& tileList = m_TileMap.GetList();
  //for ( std::vector< BaseTile* >::const_iterator it = tileList.begin(); it != tileList.end(); ++it )
  //{
  //  BaseTile* tile = *it;
  //  const Vector3& tilePos  = tile->GetPosition();
  //  const Vector3& tileSize = tile->GetSize();
  //  if ( tilePos.x - tileSize.x < worldPosX && worldPosX < tilePos.x + tileSize.x && tilePos.y - tileSize.y < worldPosY && worldPosY < tilePos.y + tileSize.y )
  //  {
  //    return tile;
  //  }
  //}
  return NULL;
}

//BaseTile* TilePlayfield::GetRandomTile()
//{
//  const std::vector< BaseTile* >& tileMap = m_TileMap.GetList();
//  if ( !tileMap.empty() )
//  {
//    int index = rand() % tileMap.size();
//    return tileMap[ index ];
//  }
//  return NULL;
//}

BaseTile* GridTilePlayfield::GetTile( const Vector2& gridPosition ) const
{
  return GetTile( gridPosition.GetIntX(), gridPosition.GetIntY() );
}

BaseTile* GridTilePlayfield::GetTile( int gridPosX, int gridPosY ) const
{
  if ( gridPosX >= 0 && gridPosX < m_GridSizeX && gridPosY >= 0 && gridPosY < m_GridSizeY )
  {
    return m_TileMap.Get( gridPosX, gridPosY );
  }
  return NULL;
}

Vector3 GridTilePlayfield::GetTilePosition( int gridPosX, int gridPosY )
{
  BaseTile* tile = GetTile( gridPosX, gridPosY );
  if ( tile != NULL )
  {
    return tile->GetPosition();
  }
  return Vector3::s_Vec3Zero;
}

/************************************************************************************************
* Center:
* Centers the camera position for the playfield.
*
* @author Thor Richard Hansen
*************************************************************************************************/
void GridTilePlayfield::Center()
{
  m_Offset.Set( 0, 0 );
  m_RepositionTiles = true;

  //RenderCamera::GetInstance()->MoveTo( Vector3( 1.2f, 0.55f, 0.5f ), Vector3( 1.2f, 0.55f, 0.0f ) );
}

/************************************************************************************************
* RepositionTiles:
* Updates the position for all tiles.
*
* @author Thor Richard Hansen
*************************************************************************************************/
void GridTilePlayfield::RepositionTiles()
{
  m_RepositionTiles = false;
  uint32 maxTiles = m_TileMap.GetSize();
  for (uint32 index = 0; index < maxTiles; index++)
  {
    BaseTile* tile = m_TileMap.Get(index);
    const Vector2& tileGridPos = tile->GetGridPosition();
    tile->SetSize(m_Zoom, m_Zoom, tile->GetSize().z);

    switch( m_PlayfieldType )
    {
      case PLAYFIELDTYPE_QUAD:
        {
          float spacer = 1.01f;
          tile->SetPosition(m_Zoom * 2 * tileGridPos.x * spacer, m_Zoom * 2 * tileGridPos.y * spacer, 0.0f);
        }
        break;

      case PLAYFIELDTYPE_HEX:
        {
          float spacerX = 0.9f * 3.5f;
          float spacerY = 0.9f;
          if ((int)(tileGridPos.y) % 2 == 0)
          {
            tile->SetPosition((m_Zoom * spacerX * tileGridPos.x) + m_Offset.x + (tile->GetSize().x * spacerX * 0.5f), (spacerY * m_Zoom * tileGridPos.y) + m_Offset.y + (m_Zoom * tile->GetSize().y * 9.0f), 0.0f);
          }
          else
          {
            tile->SetPosition((m_Zoom * spacerX * tileGridPos.x) + m_Offset.x, (m_Zoom * spacerY * (tileGridPos.y + 1)) + m_Offset.y, 0.0f);
          }
        }
        break;
      default:
        break;
    }
  }
}

/************************************************************************************************
* FrameProcess:
* Run every frame in the gamecode thread. Used to update the tile positions ( if needed )
*
* @param  (float)         deltaTime    - Amount of time that has passed since last call
*
* @author Thor Richard Hansen
*************************************************************************************************/
void GridTilePlayfield::FrameProcess( float /*deltaTime*/ )
{
  if ( m_RepositionTiles )
  {
    RepositionTiles();
  }

#if !defined SERVER
  for ( uint32 index = 0; index < m_DynamicObjects.GetSize(); )
  {
    RenderObject* renderObj = m_DynamicObjects.Get( index );
    if ( renderObj->HasDestroy() )
    {
      m_DynamicObjects.Remove( index, renderObj );
    }
    else
    {
      index++;
    }
  }
#endif
}

/************************************************************************************************
* OnDraw:
* Draw all visible tiles on the playfield.
*
* @param  (const Matrix&)  projection  - What kind of projection that is used. The projection
*                                        can either be flat 2D gui or 3D world
*
* @author Thor Richard Hansen
*************************************************************************************************/
#if defined CLIENT
void GridTilePlayfield::OnDraw( const Matrix& projection )
{
  // We will probably draw this in the correct order to get the isometric view correct
  // but for now, just draw
  uint32 maxTiles = m_TileMap.GetSize();
  for ( uint32 index = 0; index < maxTiles; index++ )
  {
    BaseTile* tile = m_TileMap.Get( index );
    if ( tile != NULL && !tile->HasFlag( CoreBaseObject::FLAGS_ISINVISIBLE ) )
    {
      tile->OnDraw( projection );
    }
  }

  uint32 numElements = m_DynamicObjects.GetSize();
  for ( uint32 index = 0; index < numElements; index++ )
  {
    RenderObject* renderObj = m_DynamicObjects.Get( index );
    if ( renderObj != NULL )
    {
      renderObj->OnDraw( projection );
    }
  }
}

#endif


#if defined CLIENT
void GridTilePlayfield::AddRenderObject( RenderObject* renderObject )
{
  m_DynamicObjects.Add( renderObject );
}
void GridTilePlayfield::RemoveRenderObject( RenderObject* renderObject )
{
  uint32 numElements = m_DynamicObjects.GetSize();
  for ( uint32 index = 0; index < numElements; index++ )
  {
    RenderObject* renderObj = m_DynamicObjects.Get( index );
    if ( renderObj == renderObject )
    {
      m_DynamicObjects.Remove( index, renderObject );
      return;
    }
  }
}
bool GridTilePlayfield::HasRenderObject( RenderObject* renderObject )
{
  uint32 numElements = m_DynamicObjects.GetSize();
  for ( uint32 index = 0; index < numElements; index++ )
  {
    RenderObject* renderObj = m_DynamicObjects.Get( index );
    if ( renderObj == renderObject )
    {
      return true;
    }
  }
  return false;
}

#endif

GridTilePlayfield* GridTilePlayfield::CreateFromData( const BinaryStream* data )
{
  uint32 playfieldId        = data->ReadUInt32();
  String playfieldName      = data->ReadString();
  uint8  playfieldType      = data->ReadUInt8();
  uint32 ownerId            = data->ReadUInt32();
  uint16 gridSizeX          = data->ReadUInt16();
  uint16 gridSizeY          = data->ReadUInt16();

  GridTilePlayfield* playfield = new GridTilePlayfield( playfieldId, playfieldName, playfieldType, ownerId, gridSizeX, gridSizeY );

  if ( playfield != NULL )
  {
    uint16 numTiles = data->ReadUInt16();
    if ( numTiles > gridSizeX * gridSizeY )
    {
      return NULL;
    }

    for ( size_t index = 0; index < numTiles; index++ )
    {
      uint16 tileType = data->ReadUInt16();
      uint16 gridPosX = data->ReadUInt16();
      uint16 gridPosY = data->ReadUInt16();
      uint16 rotation = data->ReadUInt16();
      playfield->AddTile( tileType, gridPosX, gridPosY, rotation );
    }
  }

  return playfield;
}

void GridTilePlayfield::WriteData( BinaryStream& data ) const
{
  //
  // Tile dataformat ( textfile or database )
  //
  // uint32 playfieldID
  // uint8  playfieldType 
  // String playfieldName
  // uint32 gridSizeX
  // uint32 gridSizeY
  // uint16 numTiles
  // ... uint8 tileType
  // ... uint8 gridX
  // ... uint8 gridY
  // ... uint8 rotation

  uint16 numTiles = 0;
  uint32 maxTiles = m_TileMap.GetSize();
  for ( uint32 index = 0; index < maxTiles; index++ )
  {
    BaseTile* tile = m_TileMap.Get( index );
    if ( !IsEmpty( tile ) )
    {
      numTiles++;
    }
  }

  data.WriteUInt32( m_PlayfieldId );
  data.WriteString( m_PlayfieldName );
  data.WriteUInt8( m_PlayfieldType );
  data.WriteUInt32( m_OwnerId );
  data.WriteUInt16( m_GridSizeX );
  data.WriteUInt16( m_GridSizeY );
  data.WriteUInt16( numTiles );

  for ( uint32 index = 0; index < maxTiles; index++ )
  {
    BaseTile* tile = m_TileMap.Get( index );
    if ( !IsEmpty( tile ) )
    {
      data.WriteUInt16( (uint16) tile->GetResourceId() );
      data.WriteUInt16( (uint16) tile->GetGridPosition().x );
      data.WriteUInt16( (uint16) tile->GetGridPosition().y );
      data.WriteUInt16( (uint16) tile->GetRotation().y );
    }
  }
}

bool GridTilePlayfield::SaveToDatabase()
{
#ifdef DATABASE

  // Delete old instance
  if ( m_PlayfieldId != 0 )
  {
    // Make sure that this player owns the playfield
    const SQLResultSet& infoResult = CoreDatabase::GetInstance()->ExecuteSelect("Select OwnerId from playfield where Id = %d ", m_PlayfieldId );
    while ( infoResult.Next() )
    {
      uint32 pfOwnerId          = infoResult.GetInt();
      if ( pfOwnerId != m_OwnerId )
      {
        return false;
      }
    }

    CoreDatabase::GetInstance()->ExecuteDelete( "Delete from playfield where id = %d", m_PlayfieldId ); 
    CoreDatabase::GetInstance()->ExecuteDelete( "Delete from PlayfieldTiles where PlayfieldId = %d", m_PlayfieldId ); 
  }

  if ( m_PlayfieldId == 0 )
  {
    const SQLResultSet& result = CoreDatabase::GetInstance()->ExecuteSelect( "Select max( Id ) from playfield" );
    if ( result.Next() )
    {
      m_PlayfieldId = result.GetInt() + 1;
    }
    else
    {
      m_PlayfieldId = 1;
    }
  }

  if ( m_PlayfieldId == 0 )
  {
    return false;
  }

  int numTiles = m_TileMap.GetSize();

  // Insert into the DB
  bool succeeded = false;
  succeeded = CoreDatabase::GetInstance()->ExecuteInsert( "Insert into playfield "
    "( Id, Name, OwnerId, TileType, TileSet, SizeX, SizeY, NumTiles ) "
    "values( %d, '%s', %d, %d, %d, %d, %d, %d )", 
    m_PlayfieldId,
    m_PlayfieldName.c_str(),
    m_OwnerId,
    m_PlayfieldType,
    0,
    m_GridSizeX,
    m_GridSizeY,
    numTiles
    );

  // Insert into the DB
  uint32 maxTiles = m_TileMap.GetSize();
  for ( uint32 index = 0; index < maxTiles; index++ )
  {
    BaseTile* tile = m_TileMap.Get( index );
    succeeded = succeeded && CoreDatabase::GetInstance()->ExecuteInsert( "Insert into playfieldtiles "
      "( PlayfieldId, TileId, PositionX, PositionY, RotationZ ) "
      "values( %d, %d, %d, %d, %d )", 
      m_PlayfieldId,
      tile->GetOriginalResourceId(),
      tile->GetGridPosition().GetIntX(),
      tile->GetGridPosition().GetIntY(),
      tile->GetRotation().z / 10
      );
  }

  return succeeded;
#else
  return false;
#endif
}

/************************************************************************************************
* LoadFromDatabase:
* Load a playfield and all its data from the database
*
* @param  (uint32)     playfieldId  - ID of the playfield to load
*
* @author Thor Richard Hansen
*************************************************************************************************/
#ifdef DATABASE
GridTilePlayfield* GridTilePlayfield::LoadFromDatabase( uint32 playfieldId )
{
  GridTilePlayfield* playfield = NULL;

  {
    const SQLResultSet& result = CoreDatabase::GetInstance()->ExecuteSelect( "Select Name, OwnerId, TileType, SizeX, SizeY, NumTiles from playfield where Id = %d", playfieldId );
    if ( result.Next() )
    {
      String name     = result.GetString();
      uint32 ownerId  = result.GetUInt32();
      uint8  tileType = result.GetUInt8();
      uint16 sizeX    = result.GetUInt16();
      uint16 sizeY    = result.GetUInt16();
      playfield       = GridTilePlayfield::CreatePlayfield( playfieldId, name, ownerId, tileType, sizeX, sizeY );
    }
  }

  if ( playfield != NULL )
  {
    const SQLResultSet& tileResult = CoreDatabase::GetInstance()->ExecuteSelect("Select TileId, PositionX, PositionY, RotationZ from playfieldtiles where PlayfieldId =%d", playfieldId );
    while ( tileResult.Next() )
    {
      uint32 tileId     = tileResult.GetInt();
      uint32 positionX  = tileResult.GetInt();
      uint32 positionY  = tileResult.GetInt();
      uint32 rotationZ  = tileResult.GetInt();
      playfield->AddTile( tileId, positionX, positionY, rotationZ );
    }
  }
  return playfield;
}
#endif

PlayfieldInfoMap GridTilePlayfield::LoadPlayfieldListFromDatabase( uint32 playerId )
{
  PlayfieldInfoMap infoMap;

#ifdef DATABASE
  const SQLResultSet& infoResult = CoreDatabase::GetInstance()->ExecuteSelect("Select Id, Name, OwnerId, SizeX, SizeY, Tileset from playfield where OwnerId in ( 0, %d )", playerId );
  while ( infoResult.Next() )
  {
    uint32 playfieldId      = infoResult.GetInt();
    String name             = infoResult.GetString();
    uint32 ownerId          = infoResult.GetInt();
    uint16 sizeX            = infoResult.GetUInt16();
    uint16 sizeY            = infoResult.GetUInt16();
    uint32 tileSet          = infoResult.GetInt();
    infoMap[ playfieldId ]  = new PlayfieldInfo( playfieldId, name, ownerId, sizeX, sizeY, 0, tileSet );
  }
#else
  playerId = playerId;
#endif

  return infoMap;
}

bool GridTilePlayfield::DeleteFromDatabase( uint32 ownerId, uint32 playfieldId )
{
  // Make sure that this player owns the playfield
#ifdef DATABASE
  const SQLResultSet& infoResult = CoreDatabase::GetInstance()->ExecuteSelect("Select OwnerId from playfield where Id = %d ", playfieldId );
  while ( infoResult.Next() )
  {
    uint32 pfOwnerId          = infoResult.GetInt();
    if ( pfOwnerId != ownerId )
    {
      return false;
    }
  }

  CoreDatabase::GetInstance()->ExecuteDelete( "Delete from playfield where id = %d", playfieldId ); 
  CoreDatabase::GetInstance()->ExecuteDelete( "Delete from PlayfieldTiles where PlayfieldId = %d", playfieldId ); 

#else

  ownerId     = ownerId;
  playfieldId = playfieldId;

#endif

  return true;
}

GridTilePlayfield* GridTilePlayfield::CreatePlayfield( uint32 playfieldId, const String& playfieldName, uint32 ownerId, uint8 playfieldType, uint16 sizeGridX, uint16 sizeGridY )
{
  return new GridTilePlayfield( playfieldId, playfieldName, playfieldType, ownerId, sizeGridX, sizeGridY );
}

void GridTilePlayfield::CreateGroundTiles( uint32 /*resourceId*/ )
{
  //for ( int y = 0; y< m_GridSizeY; y++ )
  //{
  //  for ( int x = 0; x< m_GridSizeX; x++ )
  //  {
  //    BaseTile* tile = GetTile( x, y );
  //    if ( tile == NULL )
  //    {
  //      //int rnd = ( rand() % 20 ) + 4;
  //      //AddTile( resourceId - rnd, x, y );
  //      tile = AddTile( resourceId, x, y );
  //      tile->SetAlpha( 1.0f );
  //    }
  //  }
  //}
  //RepositionTiles();
}

