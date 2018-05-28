/*************************************************************************************************
* Project     : EvilEngine Multi-platform 2014-2018
* File        : Playfield
*
* Description : Data class to hold the members of one playfield
*
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "PlayfieldTile.h"

#include "EvilEngine/CoreLib/BasicTypes/BinaryStream/BinaryStream.h"

PlayfieldTile::PlayfieldTile(uint16 tileType, uint16 rotation, Vector2* position)
{
  m_TileType = tileType;
  m_Rotation = rotation;
  m_Position = position;
}

void PlayfieldTile::WriteData(BinaryStream* data) const
{
  data->WriteUInt16( m_TileType );
  data->WriteUInt16( m_Rotation );
  data->WriteUInt16( (uint16) m_Position->x );
  data->WriteUInt16( (uint16) m_Position->y );
}
