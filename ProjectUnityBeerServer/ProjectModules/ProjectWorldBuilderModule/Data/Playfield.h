#ifndef PROJECT_WORLDBUILDERMODULE_DATA_PLAYFIELD
#define PROJECT_WORLDBUILDERMODULE_DATA_PLAYFIELD 1

#include "EvilEngine/CoreLib/BasicTypes/BinaryStream/BinaryStream.h"

class PlayfieldTile;

class Playfield
{
public:

  enum SourceType
  {
    SourceType_Network = 4
  };

  enum BoardSizes
  {
    StartBoardSize = 4,
    PlayBoardSize = 12,
    PlayBoardXSize = 12,
    PlayBoardYSize = 16,
  };

  enum PlayfieldFlags
  {
    PlayfieldFlag_Closed = 0,   // Playfield is only available for the owner when creating a new game
    PlayfieldFlag_Open = 1,     // All players can create games with this playfield
  };

  static uint16                 PLAYFIELD_VERSION;

  Playfield();

  void                          SetPlayfieldId(uint32 playfieldId);
  uint32                        GetPlayfieldId() const { return m_PlayfieldId; }

  uint32                        GetPlayfieldSource() const { return SourceType_Network; }

  void                          SetPlayfieldToken(const String& token) { m_Token = token; }
  const String&                 GetPlayfieldToken() const { return m_Token; }

  void                          SetOwnerAccountId( uint32 ownerAccountId ){ m_OwnerAccountId = ownerAccountId;}
  uint32                        GetOwnerAccountId() const { return m_OwnerAccountId; }

  void                          SetPlayfieldName(const String& name){ m_PlayfieldName = name; }
  const String&                 GetPlayfieldName() const{ return m_PlayfieldName; }

  void                          SetPlayfieldDescription(const String& desc) { m_Description = desc; }
  const String&                 GetPlayfieldDescription() const { return m_Description; }

  void                          SetFlags(uint32 flags){ m_Flags = flags; }
  uint32                        GetFlags() const { return m_Flags; }

  bool                          HasFlag(uint32 flags);

  void                          SetNumPlayers(uint8 numPlayers){ m_NumPlayers = numPlayers; }
  uint8                         GetNumPlayers() const{ return m_NumPlayers; }

  void                          SetNumGoals(uint8 numGoals) { m_NumGoals = numGoals; }
  uint8                         GetNumGoals() const { return m_NumGoals; }

  void                          SetBoardSizeX(uint8 boardSizeX) { m_BoardSizeX = boardSizeX; }
  uint8                         GetBoardSizeX() const { return m_BoardSizeX; }

  void                          SetBoardSizeY(uint8 boardSizeY) { m_BoardSizeY = boardSizeY; }
  uint8                         GetBoardSizeY() const{ return m_BoardSizeY; }

  uint16                        GetNumTiles() const{ return (uint16)m_Tiles.size(); }
  
  void                          AddTile( PlayfieldTile* tile );
  const PlayfieldTile*          GetTile( uint32 index ) const;
  PlayfieldTile*                GetTileAt(uint16 x, uint16 y);

  static Playfield*             CreatePlayfieldFromStream(const BinaryStream* binaryStream);
  BinaryStream*                 CreateBinaryStream(BinaryStream* outData, bool writeData);

private:

  uint32                        m_PlayfieldId;
  String                        m_Token;
  uint32                        m_OwnerAccountId;
  String                        m_PlayfieldName;
  String                        m_Description;
  uint32                        m_Flags;
  uint8                         m_NumPlayers;
  uint8                         m_NumGoals;
  uint8                         m_BoardSizeX;
  uint8                         m_BoardSizeY;
  std::vector< PlayfieldTile* > m_Tiles;

};

#endif /// PROJECT_WORLDBUILDERMODULE_DATA_PLAYFIELD#pragma once
