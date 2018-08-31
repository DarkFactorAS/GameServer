
enum TileType
{
  FloorTile_Space,                // 0
  FloorTile_Normal,               // 1
  FloorTile_SpawnPoint1,
  FloorTile_SpawnPoint2,
  FloorTile_SpawnPoint3,
  FloorTile_SpawnPoint4,
  FloorTile_Conveyor1,
  FloorTile_Conveyor1TurnRight,
  FloorTile_Conveyor1TurnLeft,    // 8
  FloorTile_Conveyor2,
  FloorTile_Goal1,
  FloorTile_Goal2,
  FloorTile_Goal3,
  FloorTile_OneWall,
  FloorTile_Repair,
  FloorTile_CogwheelLeft,
  FloorTile_CogwheelRight,
  FloorTile_Goal4,
  FloorTile_WallCorner,
  FloorTile_WallH,
  FloorTile_WallU,
  FloorTile_WallSquared,
  FloorTile_WallLazer1,
  FloorTile_WallHLazer1,
  FloorTile_NormalLazer1,
  FloorTile_Conveyor1WallLazer1,

  // Not actual tiles, but used for pathfinding

  FloorTile_Conveyor1_Rotated,
  FloorTile_Conveyor1_Back,
  FloorTile_Conveyor2_Rotated,
  FloorTile_Conveyor2_Back,
};

