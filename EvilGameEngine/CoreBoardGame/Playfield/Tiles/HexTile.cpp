/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : HexTile
*
* Description : Overloaded from BaseTile. This class will handle and draw a tile shaped as a 
*               hexagon.
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "HexTile.h"

#include "EvilEngine/CoreRendering/RenderObjects/MultiFaceRenderObject.h"
#include "EvilEngine/CoreRendering/RenderObjects/CubeRenderObject.h"
#include "EvilEngine/CoreRendering/RenderObjects/QuadRenderObject.h"
#include "EvilEngine/CoreRendering/RenderObjects/RenderListObject.h"
#include "EvilEngine/CoreRendering/RenderObjects/HexagonRenderObject.h"
#include "EvilEngine/CoreGuiToolkit/CoreGuiResources.h"
#include "EvilEngine/CoreLib/BasicTypes/BinaryStream/BinaryStream.h"
#include "EvilEngine/CoreLib/CoreMath/Quaternion.h"

float const HexTile::HEIGHT = 0.05f;
float const HexTile::FLAT   = 0.0f;
float const HexTile::DIMMED = 0.5f;

bool HexTile::s_ShowWalls = false;

HexTile::HexTile() :
  BaseTile( TILETYPE_HEX, 0 )
{
  InitRender( 0, 0 );
}

HexTile::HexTile( uint32 resourceId, uint32 wallResourceId ) :
  BaseTile( TILETYPE_HEX, resourceId )
{
  InitRender( resourceId, wallResourceId );
}

HexTile::~HexTile()
{
}

void HexTile::InitRender( uint32 resourceId, uint32 wallResourceId )
{
#if defined CLIENT
  delete m_RenderObject;
  HexagonRenderObject* hexRenderObject = new HexagonRenderObject( resourceId, wallResourceId );
  hexRenderObject->SetWallColor( Color::White );
  m_RenderObject = hexRenderObject;
#else
  VOLATILE_ARG( resourceId );
  VOLATILE_ARG( wallResourceId );
#endif
}

#if defined CLIENT
void HexTile::SetColor( float red, float green, float blue )
{
  RenderObject* renderObj = m_RenderObject->GetRenderObject( GroundRender );
  if ( renderObj != NULL )
  {
    renderObj->SetColor( red, green, blue );
  }
}
void HexTile::SetWallColor( const Color& color )
{
  FixedSizeVector< RenderObject* >& renderList = m_RenderObject->GetRenderListObject();
  for ( uint32 index = 0; index < renderList.GetSize(); index++ )
  {
    RenderObject* renderObject = renderList.Get( index );
    if ( renderObject->GetInstance() == WallRender )
    {
      renderObject->SetColor( color );
    }
  }
}
#endif


/************************************************************************************************
* CreateFromData:
* Read the data from a binary blob and then create an instance of HexTile with that data.
*
* @param  (BinaryStream)   data       - The binarystream we want to write to
* @return (HexTile*)                  - The newly created BaseTile with the blob data
*
* @author Thor Richard Hansen
*************************************************************************************************/
HexTile* HexTile::CreateFromData( const BinaryStream* data )
{
  HexTile* info = new HexTile();
  info->ReadData( data );
  return info;
}

/************************************************************************************************
* ReadData:
* Read the data from a binary blob and initialize all members with it
*
* @param  (BinaryStream)   data       - The binarystream we want to write to
*
* @author Thor Richard Hansen
*************************************************************************************************/
void HexTile::ReadData( const BinaryStream* data )
{
  m_TileType          = data->ReadUInt8();
  uint32 resourceId   = data->ReadUInt32();
  m_Doors             = data->ReadUInt32();
  uint8 numRotations  = data->ReadUInt8();
  uint16 x            = data->ReadUInt16();
  uint16 y            = data->ReadUInt16();

  SetNumRotations( numRotations );
  SetGridPosition( x, y );
  SetResourceId( resourceId );
}

void HexTile::WriteData( BinaryStream& data ) const
{
  data.WriteUInt8( m_TileType );
  data.WriteUInt32( m_ResourceId );
  data.WriteUInt32( m_Doors );
  data.WriteUInt8( GetNumRotations() );
  data.WriteUInt16( (uint16)m_GridPosition.GetIntX() );
  data.WriteUInt16( (uint16)m_GridPosition.GetIntY() );
}

/************************************************************************************************
* SetPosition:
* Set the world position of the tile.
*
* @param (float) x - X position in the world
* @param (float) y - Y position in the world
* @param (float) z - Z position in the world
*
* @author Thor Richard Hansen
*************************************************************************************************/
#if defined CLIENT
void HexTile::SetPosition( float x, float y, float z )
{
  const Vector3& oldPos = GetPosition();
  BaseTile::SetPosition( x, y, z );

  for ( std::map< int, RenderObject* >::const_iterator itWall = m_Walls.begin(); itWall != m_Walls.end(); ++itWall )
  {
    RenderObject* object = itWall->second;
    const Vector3& objPos = object->GetPosition();
    object->SetPosition( objPos.x + ( x - oldPos.x ), objPos.y + ( y - oldPos.y ), objPos.z + ( z - oldPos.z ) );
  }
}

/************************************************************************************************
* SetSize:
* Set the size of the tile.
*
* @param (float) x - How wide the tile is
* @param (float) y - How deep the tile is
* @param (float) z - How high the tile is
*
* @author Thor Richard Hansen
*************************************************************************************************/
void HexTile::SetSize( float x, float y, float z )
{
  BaseTile::SetSize( x, y, z );

  const Vector3& tilePos = GetPosition();
  for ( std::map< int, RenderObject* >::const_iterator itWall = m_Walls.begin(); itWall != m_Walls.end(); ++itWall )
  {
    RenderObject* object = itWall->second;
    Vector3 dist = object->GetPosition() - tilePos;
    dist.Normalize();

    object->SetPosition( tilePos + ( dist * 0.5f ) );
    object->SetSize( m_Size.x * 0.1f, m_Size.y * 0.1f, m_Size.z * 2.0f );
  }
}

void HexTile::SetRotation( float x, float y, float z )
{
  BaseTile::SetRotation( x, y, z );
}

/************************************************************************************************
* OnDraw:
* Draw the ground of the tile
*
* @param  (const Matrix&)  projection  - What kind of projection that is used. The projection
*                                        can either be flat 2D gui or 3D world
*
* @author Thor Richard Hansen
*************************************************************************************************/
bool HexTile::OnDraw( const Matrix& projection )
{
  bool success = BaseTile::OnDraw( projection );

  if ( s_ShowWalls )
  {
    for ( std::map< int, RenderObject* >::const_iterator itWall = m_Walls.begin(); itWall != m_Walls.end(); ++itWall )
    {
      RenderObject* object = itWall->second;
      object->OnDraw( projection );
    }
  }
  return success;
}

#endif

uint8 HexTile::GetNumRotations() const
{
  int rotX = m_Rotation.GetIntX();
  rotX %= 360;
  int numRot = ( 6 - ( rotX / 60) ) % 6;
  return (uint8) numRot; 
}

void HexTile::SetNumRotations( uint8 numRotations)
{
  SetRotation( numRotations * -60.0f, m_Rotation.y, m_Rotation.z );
  m_RotatedDoors = m_Doors;
  RotateDoors( numRotations );
}

int HexTile::GetNumSides()
{
  return 6; 
}

int HexTile::GetSideLocation( const Vector2& worldGridPosition )
{
  //Vector2 localGridPos = m_GridPosition - gridPosition;
  for ( int side = 0; side < GetNumSides(); side++ )
  {
    uint32 door = 1<<side;
    const Vector2& doorPos = GetWorldGridPosFromSide( door );
    if ( doorPos == worldGridPosition )
    {
      return side;
    }
  }
  return 0;
}

Vector2 HexTile::GetLocalGridPosFromSide( int door )
{
  if ( m_GridPosition.GetIntY() % 2 == 0 )
  {
    switch( door )
    {
      case DOOR_NORTH:        return Vector2( 0.0f, 2.0f );
      case DOOR_NORTHEAST:    return Vector2( 1.0f, 1.0f );
      case DOOR_SOUTHEAST:    return Vector2( 1.0f, -1.0f );
      case DOOR_SOUTH:        return Vector2( 0.0f, -2.0f );
      case DOOR_SOUTHWEST:    return Vector2( 0.0f, -1.0f );
      case DOOR_NORTHWEST:    return Vector2( 0.0f, 1.0f );
      default:                break;
    }
  }
  else
  {
    switch( door )
    {
      case DOOR_NORTH:        return Vector2( 0.0f, 2.0f );
      case DOOR_NORTHEAST:    return Vector2( 0.0f, 1.0f );
      case DOOR_SOUTHEAST:    return Vector2( 0.0f, -1.0f );
      case DOOR_SOUTH:        return Vector2( 0.0f, -2.0f );
      case DOOR_SOUTHWEST:    return Vector2( -1.0f, -1.0f );
      case DOOR_NORTHWEST:    return Vector2( -1.0f, 1.0f );
      default:                break;
    }
  }

  return BaseTile::GetLocalGridPosFromSide( door );
}

Vector2 HexTile::GetCorner( int side )
{
  switch( side )
  {
    case DOOR_NORTH:        return Vector2( 0.0f, 1.0f );
    case DOOR_NORTHEAST:    return Vector2( 0.75f, 0.5f );
    case DOOR_SOUTHEAST:    return Vector2( 0.75f, -0.5f );
    case DOOR_SOUTH:        return Vector2( 0.0f, -1.0f );
    case DOOR_SOUTHWEST:    return Vector2( -0.75f, -0.5f );
    case DOOR_NORTHWEST:    return Vector2( -0.75f, 0.5f );
    default:                return Vector2::s_Vec2Zero;
  }
}

Vector2 HexTile::GetWorldGridPosFromSide( int side )
{
  const Vector2& gridPos = GetLocalGridPosFromSide( side );
  if ( gridPos != Vector2::s_Vec2Minus )
  {
    return m_GridPosition + gridPos;
  }
  return Vector2::s_Vec2Minus;
}

int HexTile::GetConnectedDoor( uint32 door )
{
  switch( door )
  {
    case DOOR_NORTH:        return DOOR_SOUTH;
    case DOOR_NORTHEAST:    return DOOR_SOUTHWEST;
    case DOOR_SOUTHEAST:    return DOOR_NORTHWEST;
    case DOOR_SOUTH:        return DOOR_NORTH;
    case DOOR_SOUTHWEST:    return DOOR_NORTHEAST;
    case DOOR_NORTHWEST:    return DOOR_SOUTHEAST;
    default:                break;
  }
  return 0;
}

bool HexTile::HasConnectedDoor( uint32 door )
{
  int connectedDoor = GetConnectedDoor( door );
  if ( connectedDoor != 0 )
  {
    return HasDoor( connectedDoor );
  }
  return false;
}

Vector3 HexTile::GetWorldPos( const Vector2& gridPos )
{
  float spacerX = 3.175f;
  float spacerY = 1.075f;

  if ( ( m_GridPosition.GetIntY() + gridPos.GetIntY() ) % 2 == 0 )
  {
    return Vector3( 
      m_Size.x * spacerX * gridPos.x + ( m_Size.x * spacerX * 0.5f ), 
      m_Size.y * spacerY * gridPos.y + ( m_Size.y * 10.25f ),
      0.0f );
//    tile->SetPosition( (m_Zoom * spacerX * tileGridPos.x) + m_Offset.x + ( tile->GetSize().x * spacerX * 0.5f ), ( spacerY * m_Zoom * tileGridPos.y) + m_Offset.y + ( m_Zoom * tile->GetSize().y * 10.25f ), 0.0f );
  }
  else
  {
    return Vector3( 
      m_Size.x * spacerX * gridPos.x, 
      m_Size.y * spacerY * ( gridPos.y + 1 ),
      0.0f );

    //tile->SetPosition( (m_Zoom * spacerX * tileGridPos.x) + m_Offset.x, (m_Zoom * spacerY * ( tileGridPos.y + 1 )) + m_Offset.y, 0.0f );
  }
}

#if defined CLIENT

void HexTile::ShowWalls( uint32 resourceId, const Color& color )
{
  RemoveWalls();

  //int side = 1;
  int numSides = GetNumSides();
  for ( int side = 0; side < numSides; side++ )
  {
    int door            = 1 << side;
    if ( !HasDoor( door ) )
    {
      //float radX    = (float)( 60.0f * side * M_PI / 180.0f);
      float height  = m_Size.z * 0.5f;

      float mult = 0.83f;

      //Vector3 dir         = GetCorner( door ) * ( m_Size.x * mult );
      //Quaternion quat(  dir, radX );
      //dir = dir * quat;

      float angle = ( ((side-1.5f) / -6.0f) * 2 * (float)M_PI );
      Vector3 dir( m_Size.x * mult * cos(angle), m_Size.y * mult * sin(angle), 0.0f );

      Vector3 worldPos    = GetPosition() + dir;
      worldPos.z          += m_Size.z * 0.5f;
      worldPos.z          += height * 0.5f;
      CubeRenderObject* wall = new CubeRenderObject( worldPos, Vector3( m_Size.x * 0.5f, m_Size.y * 0.05f, height ), resourceId );
      wall->SetRotation( -60.0f * side, 0.0f, 0.0f );
      wall->SetColor( color ); 

      //switch ( side )
      //{
      //  case 0:             wall->SetColor( Color::Yellow );          break;    // DOOR_NORTH
      //  case 1:             wall->SetColor( Color::Red );             break;    // DOOR_NORTHEAST
      //  case 2:             wall->SetColor( Color::Blue );            break;    // DOOR_SOUTHEAST
      //  case 3:             wall->SetColor( Color::Green );           break;    // DOOR_SOUTH
      //  case 4:             wall->SetColor( Color::White );           break;    // DOOR_SOUTHWEST
      //  case 5:             wall->SetColor( Color::Black );           break;    // DOOR_NORTHWEST
      //}

      m_Walls[ door ] = wall;    
    }
  }
}
#endif

#if defined CLIENT
void HexTile::SetResourceId( uint32 resourceId )
{
  m_RenderObject->SetResourceId( resourceId );
  m_RenderObject->SetResourceId( GroundRender, resourceId );
  //m_RenderObject->SetResourceId( WallRender, resourceId );
}
#endif

#if defined CLIENT
uint32 HexTile::GetResourceId() const
{
  RenderObject* renderObject = m_RenderObject->GetRenderObject( GroundRender );
  if ( renderObject != NULL )
  {
    return renderObject->GetResourceId();
  }
  return 0;
}
#endif