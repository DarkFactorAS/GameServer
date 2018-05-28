

#include "Precompile.h"
#include "PlayfieldInfo.h"
#include "EvilEngine/CoreLib/CoreMath/Vector2.h"
#include <stdlib.h>

PlayfieldInfo::PlayfieldInfo() :
  m_PlayfieldId( 0 ),
  m_OwnerId( 0 ),
  m_SizeX( 0 ),
  m_SizeY( 0 ),
  m_DeckSize( 0 ),
  m_Tileset(0)
{
}

PlayfieldInfo::PlayfieldInfo( uint32 playfieldId, String playfieldName, uint32 ownerId, uint16 sizeX, uint16 sizeY, uint16 deckSize, uint32 tileSet ) :
  m_PlayfieldId( playfieldId ),
  m_PlayfieldName( playfieldName ),
  m_OwnerId( ownerId ),
  m_SizeX( sizeX ),
  m_SizeY( sizeY ),
  m_DeckSize( deckSize ),
  m_Tileset( tileSet )
{
  m_TilesetName = String::FormatString("DungeonTiles_%d.dds", m_Tileset);
}

PlayfieldInfo* PlayfieldInfo::CreateFromData( const BinaryStream* data )
{
  uint32 playfieldId        = data->ReadUInt32();
  uint32 ownerId            = data->ReadUInt32();
  uint16 gridSizeX          = data->ReadUInt16();
  uint16 gridSizeY          = data->ReadUInt16();
  String playfieldName      = data->ReadString();
  uint16 deckSize           = data->ReadUInt16();
  uint32 tileset            = data->ReadUInt32();

  return new PlayfieldInfo( playfieldId, playfieldName, ownerId, gridSizeX, gridSizeY, deckSize, tileset );
}

void PlayfieldInfo::WriteData( BinaryStream& data ) const
{
  data.WriteUInt32( m_PlayfieldId );
  data.WriteUInt32( m_OwnerId );
  data.WriteUInt16( m_SizeX );
  data.WriteUInt16( m_SizeY );
  data.WriteString( m_PlayfieldName );
  data.WriteUInt16( m_DeckSize );
  data.WriteUInt32( m_Tileset );
}

void PlayfieldInfo::AddStartPoint( int indexX, int indexY )
{
  m_StartPoints.push_back( Vector2( (float)indexX, (float)indexY ) );
}

void PlayfieldInfo::AddStartPoint( const Vector2& startPoint )
{
  m_StartPoints.push_back( startPoint );
}

const Vector2& PlayfieldInfo::GetRandomStartPoint()
{
  int index = rand() % m_StartPoints.size();
  return m_StartPoints[ index ];
}
  
const Vector2& PlayfieldInfo::GetStartPoint( size_t index )
{
  if ( index < m_StartPoints.size() )
  {
    return m_StartPoints[ index ];
  }
  return Vector2::s_Vec2Zero;
}

const Vector2& PlayfieldInfo::GetPlacedTiles( size_t index )
{
  if ( index < m_PlacedTiles.size() )
  {
    return m_PlacedTiles[ index ];
  }
  return Vector2::s_Vec2Zero;
}

//void PlayfieldInfo::CreateSpawnPoints( int numSpawnPoints )
//{
//  for ( int spawn = 0; spawn < numSpawnPoints; spawn++ )
//  {
//    int numRetries = 10;
//    int x = 0;
//    int y = 0;
//    while ( numRetries > 0 )
//    {
//      bool succeeded = true;
//
//      x = rand() % m_SizeX;
//      y = rand() % m_SizeY;
//
//      // Not place on startpoints
//      for ( uint32 i = 0; i< m_StartPoints.size(); i++ )
//      {
//        const Vector2& pos = m_StartPoints[ i ];
//        if ( pos.GetIntX() == x && pos.GetIntY() == y )
//        {
//          succeeded = false;
//          break;
//        }
//      }
//
//      if ( succeeded )
//      {
//        numRetries = 0;
//      }
//    }
//
//    AddSpawnPoint( x, y );
//  }
//}
//
//void PlayfieldInfo::AddSpawnPoint( int indexX, int indexY )
//{
//  m_SpawnPoints.push_back( Vector2( (float)indexX, (float)indexY ) );
//}
//
//void PlayfieldInfo::AddSpawnPoint( const Vector2& startPoint )
//{
//  m_SpawnPoints.push_back( startPoint );
//}
//
//const Vector2& PlayfieldInfo::GetSpawnPoint( size_t index )
//{
//  if ( index < m_SpawnPoints.size() )
//  {
//    return m_SpawnPoints[ index ];
//  }
//  return Vector2::s_Vec2Zero;
//}
//
//const Vector2& PlayfieldInfo::GetRandomSpawnPoint()
//{
//  if ( !m_SpawnPoints.empty() )
//  {
//    size_t index = rand() % m_SpawnPoints.size();
//    return m_SpawnPoints[ index ];
//  }
//  return Vector2::s_Vec2Zero;
//}
