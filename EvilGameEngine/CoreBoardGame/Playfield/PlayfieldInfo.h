#ifndef EVILGAMEENGINE_COREBOARDGAME_PLAYFIELD_PLAYFIELDINFO
#define EVILGAMEENGINE_COREBOARDGAME_PLAYFIELD_PLAYFIELDINFO 1
#pragma once

#include "EvilEngine/CoreLib/BasicTypes/BinaryStream/BinaryStream.h"
#include <map>

#include "EvilEngine/CoreLib/CoreMath/Vector2.h"

class PlayfieldInfo
{
public:
  PlayfieldInfo();
  PlayfieldInfo( uint32 playfieldId, String playfieldName, uint32 ownerId, uint16 sizeX, uint16 sizeY, uint16 deckSize, uint32 tileSet );

  uint32                          GetPlayfieldId() const{ return m_PlayfieldId; }
  const String&                   GetPlayfieldName() const { return m_PlayfieldName; }
  uint32                          GetOwnerId() const{ return m_OwnerId; }
  uint16                          GetSizeX() const{ return m_SizeX; }
  uint16                          GetSizeY() const{ return m_SizeY; }
  uint16                          GetDeckSize() const { return m_DeckSize; }
  uint32                          GetTileset() const { return m_Tileset; }
  String                          GetTilesetName() const { return m_TilesetName; }

  static PlayfieldInfo*           CreateFromData( const BinaryStream* data );
  void                            WriteData( BinaryStream& data ) const;

  void                            AddStartPoint( int indexX, int indexY );
  void                            AddStartPoint( const Vector2& startPoint );
  const Vector2&                  GetRandomStartPoint();
  const Vector2&                  GetStartPoint( size_t index );
  uint32                          GetNumStartPoints(){ return (uint32)m_StartPoints.size(); }

  const Vector2&                  GetPlacedTiles( size_t index );
  //uint32                          GetNumPlacedTiles(){ return (uint32)m_PlacedTiles.size(); }
  

  //void                            AddSpawnPoint( int indexX, int indexY );
  //void                            AddSpawnPoint( const Vector2& startPoint );
  //const Vector2&                  GetSpawnPoint( size_t index );
  //const Vector2&                  GetRandomSpawnPoint();
  //uint32                          GetNumSpawnPoints(){ return (uint32)m_SpawnPoints.size(); }
  //void                            CreateSpawnPoints( int numSpawnPoints );

private:

  uint32                          m_PlayfieldId;
  String                          m_PlayfieldName;
  uint32                          m_OwnerId;
  uint16                          m_SizeX;
  uint16                          m_SizeY;
  uint16                          m_DeckSize;
  uint32                          m_Tileset;
  String                          m_TilesetName;

  //std::vector< Vector2 >          m_SpawnPoints;
  std::vector< Vector2 >          m_StartPoints;
  std::vector< Vector2 >          m_PlacedTiles;
};

typedef std::map< uint32, PlayfieldInfo* > PlayfieldInfoMap;

#endif /// EVILGAMEENGINE_COREBOARDGAME_PLAYFIELD_PLAYFIELDINFO
