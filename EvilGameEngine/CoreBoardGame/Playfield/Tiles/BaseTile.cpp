/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : BaseTile
*
* Description : Class that holds the basic information for a tile.
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "BaseTile.h"
#include "EvilEngine/CoreRendering/RenderObjects/RenderObject.h"
#include "EvilEngine/CoreLib/BasicTypes/BinaryStream/BinaryStream.h"

BaseTile::BaseTile() :
  CoreBaseRenderObject( 0 )
  , m_TileType( 0 )
  , m_OriginalResourceId( 0 )
  , m_Doors( 0 )
  , m_RotatedDoors( 0 )
{

}

BaseTile::BaseTile( uint8 tileType, uint32 resourceId ) :
  CoreBaseRenderObject( resourceId )
  , m_TileType( tileType )
  , m_OriginalResourceId( resourceId )
  , m_Doors( 0 )
  , m_RotatedDoors( 0 )
{
}

BaseTile::~BaseTile()
{
#if defined CLIENT
  RemoveWalls();
#endif
}

#if defined CLIENT
void BaseTile::RemoveWalls()
{
  for ( std::map< int, RenderObject* >::iterator itWall = m_Walls.begin(); itWall != m_Walls.end(); ++itWall )
  {
    delete itWall->second;
  }
  m_Walls.clear();
}
#endif

/************************************************************************************************
* CreateFromData:
* Read the data from a binary blob and then create an instance of BaseTile with that data.
*
* @param  (BinaryStream)   data       - The binarystream we want to write to
* @return (BaseTile*)                 - The newly created BaseTile with the blob data
*
* @author Thor Richard Hansen
*************************************************************************************************/
BaseTile* BaseTile::CreateFromData( const BinaryStream* data )
{
  BaseTile* info = new BaseTile();
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
void BaseTile::ReadData( const BinaryStream* data )
{
  m_TileType          = data->ReadUInt8();
  m_Doors             = data->ReadUInt32();
  uint8 numRotations  = data->ReadUInt8();
  uint16 x            = data->ReadUInt16();
  uint16 y            = data->ReadUInt16();

  RotateDoors( numRotations );
  SetNumRotations( numRotations );
  SetGridPosition( x, y );
}

void BaseTile::WriteData( BinaryStream& data ) const
{
  data.WriteUInt8( m_TileType );
  data.WriteUInt32( m_Doors );
  data.WriteUInt8( GetNumRotations() );
  data.WriteUInt16( (uint16)m_GridPosition.GetIntX() );
  data.WriteUInt16( (uint16)m_GridPosition.GetIntY() );
}

void BaseTile::RotateDoors( uint8 numRotations )
{
  m_RotatedDoors = m_Doors;
  uint32 overflowDoor = 1 << GetNumSides();

  for ( uint8 numRot = 0; numRot < numRotations; numRot++)
  {
    m_RotatedDoors = m_RotatedDoors << 1;
    if ( ( m_RotatedDoors & overflowDoor ) == overflowDoor )
    {
      m_RotatedDoors += 1;
      m_RotatedDoors -= overflowDoor;
    }
  }
}
