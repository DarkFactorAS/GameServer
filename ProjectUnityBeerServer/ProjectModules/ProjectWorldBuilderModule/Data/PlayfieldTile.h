#ifndef PROJECT_WORLDBUILDERMODULE_DATA_PLAYFIELDTILE
#define PROJECT_WORLDBUILDERMODULE_DATA_PLAYFIELDTILE 1

#include "EvilEngine/CoreLib/CoreMath/Vector2.h"

class BinaryStream;

class PlayfieldTile
{
public:

  PlayfieldTile(uint16 tileType, uint16 rotation, Vector2* position);

  uint16     m_TileType;
  uint16     m_Rotation;
  Vector2*   m_Position;

  void WriteData(BinaryStream* data) const;
};

#endif /// PROJECT_WORLDBUILDERMODULE_DATA_PLAYFIELDTILE
