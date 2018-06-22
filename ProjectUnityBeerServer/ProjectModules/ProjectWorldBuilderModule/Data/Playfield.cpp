/*************************************************************************************************
* Project     : EvilEngine Multi-platform 2014-2018
* File        : Playfield
*
* Description : Data class to hold the members of one playfield
*
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "Playfield.h"
#include "PlayfieldTile.h"

uint16 Playfield::PLAYFIELD_VERSION = 1;

Playfield::Playfield()
{
}

void Playfield::SetPlayfieldId(uint32 playfieldId)
{
  m_PlayfieldId = playfieldId;
}

bool Playfield::HasFlag(uint32 flags)
{
  return ( m_Flags & flags ) == flags;
}


void Playfield::AddTile(PlayfieldTile* tile)
{
  m_Tiles.push_back(tile);
}

const PlayfieldTile* Playfield::GetTile(uint32 index) const
{
  if (index < m_Tiles.size())
  {
    return m_Tiles[ index ];
  }
  return NULL;
}

PlayfieldTile* Playfield::GetTileAt(uint16 x, uint16 y)
{
  for (uint32 index = 0; index < m_Tiles.size(); index++)
  {
    PlayfieldTile* tile = m_Tiles[index];
    if (tile->m_Position->GetUInt16X() == x && tile->m_Position->GetUInt16Y() == y)
    {
      return tile;
    }
  }
  return NULL;
}

Playfield* Playfield::CreatePlayfieldFromStream(const BinaryStream* datastream)
{
  uint16 version = datastream->ReadUInt16();

  version = version;

  Playfield* playfield = new Playfield();
  playfield->SetPlayfieldId(datastream->ReadUInt32());
  playfield->SetPlayfieldToken(datastream->ReadString());
  playfield->SetPlayfieldName(datastream->ReadString());
  playfield->SetPlayfieldDescription(datastream->ReadString());
  playfield->SetFlags(datastream->ReadUInt32());
  playfield->SetNumPlayers(datastream->ReadUInt8());
  playfield->SetNumGoals(datastream->ReadUInt8());
  playfield->SetBoardSizeX(datastream->ReadUInt8());
  playfield->SetBoardSizeY(datastream->ReadUInt8());

  bool hasData = datastream->ReadBoolean();
  if (hasData)
  {
    uint8 boardX = playfield->GetBoardSizeX();
    uint8 boardY = playfield->GetBoardSizeY();

    for (int x = 0; x < boardX; x++)
    {
      for (int y = 0; y < boardY; y++)
      {
        uint16 tileType = datastream->ReadUInt16();
        uint16 rotation = datastream->ReadUInt16();

        playfield->AddTile(new PlayfieldTile(tileType, rotation, new Vector2((float)x, (float)y)));
      }
    }
  }

  if (!datastream->IsOK())
  {
    delete playfield;
    playfield = NULL;
  }

  return playfield;
}

BinaryStream* Playfield::CreateBinaryStream(BinaryStream* outData, bool writeData)
{
  if (outData == NULL)
  {
    outData = new BinaryStream();
  }

  // Write header
  outData->WriteUInt16(PLAYFIELD_VERSION);
  outData->WriteUInt32(m_PlayfieldId);
  outData->WriteString(m_Token);
  outData->WriteString(m_PlayfieldName);
  outData->WriteString(m_Description);
  outData->WriteUInt32(m_Flags);
  outData->WriteUInt8(m_NumPlayers);
  outData->WriteUInt8(m_NumGoals);
  outData->WriteUInt8(m_BoardSizeX);
  outData->WriteUInt8(m_BoardSizeY);
  outData->WriteBoolean(writeData);

  if (writeData)
  {
    for (uint16 x = 0; x < m_BoardSizeX; x++)
    {
      for (uint16 y = 0; y < m_BoardSizeY; y++)
      {
        PlayfieldTile* tile = GetTileAt(x, y);
        if (tile != NULL)
        {
          outData->WriteUInt16(tile->m_TileType);
          outData->WriteUInt16(tile->m_Rotation);
        }
        else
        {
          outData->WriteUInt32(0);
        }
      }
    }
  }

  return outData;
}
