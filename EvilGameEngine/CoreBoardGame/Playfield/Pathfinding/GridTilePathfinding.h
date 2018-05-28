#ifndef EVILGAMEENGINE_COREBOARDGAME_PLAYFIELD_GRIDTILEPATHFINDING
#define EVILGAMEENGINE_COREBOARDGAME_PLAYFIELD_GRIDTILEPATHFINDING 1
#pragma once

#include <stdlib.h>
#include <vector>

#include "EvilEngine/CoreLib/BasicTypes/FixedMaps/FixedMultimap.h"
#include "EvilEngine/CoreLib/CoreMath/Vector2.h"
#include "EvilEngine/CoreLib/CoreMath/Color.h"
#include "EvilEngine/CoreLib/Signal/SignalTarget.h"

#if defined CLIENT
#include "EvilEngine/CoreLib/BasicTypes/FixedMaps/FixedSizeVector.h"
#endif

//#if defined DEBUG
#define VISUALIZE_PATHFINDING 1
//#endif

class Player;
class GameObject;
class GridTilePlayfield;
class BaseTile;
class CoreBaseObject;

class GridTilePathfinding : public SignalTarget
{
public:
  struct PathFinding
  {
    bool                            m_IsVisible;
    int                             m_ColorIndex;
    std::vector< Vector2 >          m_Nodes;

    PathFinding() :
      m_IsVisible(false),
      m_ColorIndex(0)
    {
    }

    PathFinding(bool isVisible, std::vector< Vector2 >& path, int colorIndex) :
      m_IsVisible(isVisible),
      m_ColorIndex(colorIndex % 4),
      m_Nodes(path)
    {
    };

    void SetPathColor(int colorIndex)
    {
      m_ColorIndex = colorIndex % 4;
    }
  };

  GridTilePathfinding( GridTilePlayfield* playfield );

  void                              FrameProcess(float deltaTime);

  std::vector< uint32 >             GetPathIdList();
  GridTilePathfinding::PathFinding* GetPathFromId(uint32 pathId);

  std::vector< Vector2 >            GetPathToPosition(GameObject* pathOwner, const Vector2& endPos, bool mustWalk );
  std::vector< Vector2 >            GetPathToPositions(GameObject* pathOwner, std::vector< Vector2 >& endPosList, bool mustWalk );
  void                              GetShortestPathToGridPosition(const Vector2& gridPosition, std::vector< Vector2 >& shortestPath, std::vector< Vector2 >& currentPath, std::vector< Vector2 >& objectiveList, std::vector< std::pair< Vector2, uint32 > >& visitedTiles, uint32 level, uint32 maxLevel, bool mustWalk);

  std::vector< Vector2 >            GetMaxPath(const std::vector< Vector2 >& fullPath, uint32 maxDistance);

  void                              AddPathfinding(const GameObject* gameObject, std::vector< Vector2 >& path);
  void                              UpdatePath(uint32 pathId);
  void                              RemovePathfinding(uint32 pathId);
  bool                              ClearPathfindingPath(uint32 pathId);
  void                              ClearAllPathfindingPaths();

  static void                       TogglePathfinding();
  static const Color&               GetPathfindingColor(uint32 objectId);

  bool                              UpdateTileColors(BaseTile* tile);

  static bool                       s_ShowPathfinding;

  Signal1< void, uint32 >           SignalAddPath;
  Signal1< void, uint32 >           SignalRemovePath;

  void                              SlotOnCoreBaseObjectDeath(const CoreBaseObject* coreBaseObject);

private:
  static bool                       s_UpdatePathfinding;

  void                              ShowPathfindingPath();

  bool                              HasTilePos(std::vector< std::pair< Vector2, uint32 > >& visitedTiles, const Vector2& gridPos, uint32 currentLevel);
  bool                              HasTilePos(std::vector< Vector2 > & tilePositionList, const Vector2& gridPosition);

//#if defined VISUALIZE_PATHFINDING

  std::map< uint32, PathFinding >   m_PathfindingList;

//#endif

  GridTilePlayfield*                m_Playfield;

};

#endif /// EVILGAMEENGINE_COREBOARDGAME_PLAYFIELD_GRIDTILEPATHFINDING
