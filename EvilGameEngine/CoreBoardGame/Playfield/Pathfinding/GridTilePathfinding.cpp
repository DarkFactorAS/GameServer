/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014-2016
* File        : GridTilePathfinding
*
* Description : Finds shortest path between two placed tiles on a grid tile playfield. Also
*               contains utility functins to visualize the path
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "GridTilePathfinding.h"

#include "EvilEngine/CoreLib/CoreMath/Color.h"
#include "EvilEngine/CoreLib/BasicTypes/Definitions/VolatileArgs.h"

#include "EvilGameEngine/CoreBoardGame/GameLogic/Player/Player.h"
#include "EvilGameEngine/CoreBoardGame/GameLogic/GameObject/GameObject.h"
#include "EvilGameEngine/CoreBoardGame/Playfield/Tiles/BaseTile.h"
#include "EvilGameEngine/CoreBoardGame/Playfield/Tiles/HexTile.h"
#include "EvilGameEngine/CoreBoardGame/GameLogic/Player/Player.h"
#include "EvilGameEngine/CoreBoardGame/GameLogic/GameObject/Item.h"
#include "EvilGameEngine/CoreBoardGame/Playfield/GridTilePlayfield.h"
#include "EvilGameEngine/CoreGameLogin/Account/Account.h"

#ifdef CLIENT
  #include "EvilGameEngine/CoreGameEngine/CoreEffectSystem/CoreEffectSystem.h"
  #include "EvilGameEngine/CoreGameEngine/CoreEffectSystem/Effects/BallisticMoveEffect.h"
  #include "EvilGameEngine/CoreGameEngine/CoreEffectSystem/Effects/CameraFollowObjectEffect.h"
  #include "EvilEngine/CoreRendering/RenderObjects/HexagonRenderObject.h"
  #include "EvilEngine/CoreGuiToolkit/CoreGuiResources.h"
  #include "EvilEngine/CoreRendering/Camera/RenderCamera.h"
  #include "EvilEngine/CoreEngine/CoreInput/CoreInputManager.h"
  #include "EvilEngine/CoreEngine/CoreEffectSystem/EffectPackage.h"
#endif

bool GridTilePathfinding::s_ShowPathfinding   = false;
bool GridTilePathfinding::s_UpdatePathfinding = false;

GridTilePathfinding::GridTilePathfinding( GridTilePlayfield* playfield ) :
  m_Playfield( playfield )
{
}

void GridTilePathfinding::FrameProcess(float /*deltaTime*/)
{
#if defined VISUALIZE_PATHFINDING
  if (s_UpdatePathfinding)
  {
    s_UpdatePathfinding = false;

    if (s_ShowPathfinding)
    {
      ShowPathfindingPath();
    }
    else
    {
      ClearAllPathfindingPaths();
    }
  }
#endif
}

void GridTilePathfinding::TogglePathfinding()
{
  s_ShowPathfinding = !s_ShowPathfinding;
  s_UpdatePathfinding = true;
}

void GridTilePathfinding::ClearAllPathfindingPaths()
{
#if defined VISUALIZE_PATHFINDING

  // First disable all paths
  for (std::map< uint32, PathFinding >::iterator itPath = m_PathfindingList.begin(); itPath != m_PathfindingList.end(); ++itPath)
  {
    PathFinding& path = itPath->second;
    path.m_IsVisible = false;
    //SignalRemovePath( itPath->first );
  }

  // Go through all nodes and fix them
  for (std::map< uint32, PathFinding >::iterator itPath = m_PathfindingList.begin(); itPath != m_PathfindingList.end(); ++itPath)
  {
    PathFinding& path = itPath->second;
    for (uint32 index = 0; index < path.m_Nodes.size(); index++)
    {
      const Vector2& point = path.m_Nodes[index];
      BaseTile* tile = m_Playfield->GetTile(point);
      if (tile != NULL)
      {
        m_Playfield->SetDefaultTileColor(tile);
      }
    }
  }

  //m_PathfindingList.clear();

#endif 
}

bool GridTilePathfinding::ClearPathfindingPath(uint32 pathId)
{
#if defined VISUALIZE_PATHFINDING

  std::map< uint32, PathFinding >::iterator itPath = m_PathfindingList.find(pathId);
  if (itPath == m_PathfindingList.end())
  {
    return s_ShowPathfinding;
  }

  PathFinding& path = itPath->second;
  bool isVisible = path.m_IsVisible;
  path.m_IsVisible = false;

  // Clear the old path
  for (uint32 index = 0; index < path.m_Nodes.size(); index++)
  {
    const Vector2& point = path.m_Nodes[index];
    BaseTile* tile = m_Playfield->GetTile(point);
    if (tile != NULL)
    {
      m_Playfield->SetDefaultTileColor(tile);
    }
  }
  path.m_Nodes.clear();
  return isVisible;

#else

  VOLATILE_ARG(pathId);
  return false;

#endif
}

const Color& GridTilePathfinding::GetPathfindingColor(uint32 index)
{
  index %= 4;

  switch (index % 4)
  {
  case 0:
    return Color::Red;
  case 1:
    return Color::Green;
  case 2:
    return Color::Orange;
  case 3:
    return Color::Blue;
  default:
    return Color::White;
  }
};

void GridTilePathfinding::ShowPathfindingPath()
{
#if defined VISUALIZE_PATHFINDING
  for (std::map< uint32, PathFinding >::iterator itPath = m_PathfindingList.begin(); itPath != m_PathfindingList.end(); ++itPath)
  {
    PathFinding& path = itPath->second;

    const Color& color = GetPathfindingColor(path.m_ColorIndex);
    if (!path.m_IsVisible)
    {
      continue;
    }

    for (uint32 index = 0; index < path.m_Nodes.size(); index++)
    {
      const Vector2& point = path.m_Nodes[index];
      BaseTile* tile = m_Playfield->GetTile(point);
      if (tile != NULL)
      {
        tile->SetColor(color);
      }
    }
  }
#endif
}

void GridTilePathfinding::SlotOnCoreBaseObjectDeath(const CoreBaseObject* coreBaseObject)
{
  if (coreBaseObject != NULL)
  {
    RemovePathfinding(coreBaseObject->GetInstance());
  }
}

void GridTilePathfinding::AddPathfinding(const GameObject* gameObject, std::vector< Vector2 >& path)
{
  // Colorize the path
#if defined VISUALIZE_PATHFINDING

  uint32 pathId = gameObject->GetInstance();

  bool isNew = true;
  std::map< uint32, PathFinding >::iterator itPath = m_PathfindingList.find(pathId);
  if (itPath != m_PathfindingList.end())
  {
    isNew = false;
  }

  bool isVisible = ClearPathfindingPath(pathId);

  if (isNew)
  {
    m_PathfindingList[pathId] = PathFinding(true, path, pathId);
    gameObject->m_SignalOnDeath.Connect(this, &GridTilePathfinding::SlotOnCoreBaseObjectDeath);
    SignalAddPath(pathId);
  }
  else
  {
    m_PathfindingList[pathId].m_Nodes = path;
    m_PathfindingList[pathId].m_IsVisible = isVisible;
  }

  s_UpdatePathfinding = true;

#else

  VOLATILE_ARG(pathId);
  VOLATILE_ARG(path);

#endif
}

void GridTilePathfinding::RemovePathfinding(uint32 pathId)
{
  std::map< uint32, PathFinding >::iterator itPath = m_PathfindingList.find(pathId);
  if (itPath != m_PathfindingList.end())
  {
    // Remove the path visually
    itPath->second.m_IsVisible = false;
    UpdatePath(pathId);

    m_PathfindingList.erase(itPath);

    // Update debug gui and other listeners
    SignalRemovePath(pathId);
  }
}

void GridTilePathfinding::UpdatePath(uint32 pathId)
{
  // Go through all nodes and fix them

  std::map< uint32, PathFinding >::iterator itPath = m_PathfindingList.find(pathId);
  if (itPath != m_PathfindingList.end())
  {
    PathFinding& path = itPath->second;
    for (uint32 index = 0; index < path.m_Nodes.size(); index++)
    {
      const Vector2& point = path.m_Nodes[index];
      BaseTile* tile = m_Playfield->GetTile(point);
      if (tile != NULL)
      {
        m_Playfield->SetDefaultTileColor(tile);
      }
    }
  }
}

std::vector< uint32 > GridTilePathfinding::GetPathIdList()
{
  std::vector< uint32 > list;
  for (std::map< uint32, PathFinding >::iterator itPath = m_PathfindingList.begin(); itPath != m_PathfindingList.end(); ++itPath)
  {
    list.push_back(itPath->first);
  }
  return list;
}

GridTilePathfinding::PathFinding* GridTilePathfinding::GetPathFromId(uint32 pathId)
{
  std::map< uint32, PathFinding >::iterator itPath = m_PathfindingList.find(pathId);
  if (itPath != m_PathfindingList.end())
  {
    return &(itPath->second);
  }

  return NULL;
}

std::vector< Vector2 > GridTilePathfinding::GetPathToPosition(GameObject* pathOwner, const Vector2& endPos , bool mustWalk )
{
  // Our currently shortest path to an open spawnpoint
  std::vector< Vector2 > shortestPath;

  if (pathOwner == NULL)
  {
    return shortestPath;
  }

  const Vector2& startPos = pathOwner->GetGridPosition();
  if (startPos == endPos)
  {
    return shortestPath;
  }

  // The point we are looking for
  std::vector< Vector2 > pointOfInterest;
  pointOfInterest.push_back(endPos);

  // The current list of points we are going through
  std::vector< Vector2 > currentPath;
  currentPath.push_back(startPos);

  std::vector< std::pair< Vector2, uint32 > > visitedTiles;
  visitedTiles.push_back(std::pair< Vector2, uint32 >(startPos, 0));

  GetShortestPathToGridPosition(startPos, shortestPath, currentPath, pointOfInterest, visitedTiles, 0, 30, mustWalk );
  if (!shortestPath.empty())
  {
    AddPathfinding(pathOwner, shortestPath);
  }
  else
  {
    ClearPathfindingPath(pathOwner->GetInstance());
  }

  return shortestPath;
}

std::vector< Vector2 > GridTilePathfinding::GetPathToPositions(GameObject* pathOwner, std::vector< Vector2 >& playerPositions, bool mustWalk )
{
  // Our currently shortest path to an open spawnpoint
  std::vector< Vector2 > shortestPath;

  if (pathOwner == NULL)
  {
    return shortestPath;
  }

  // Must have a point of interest
  if (playerPositions.empty())
  {
    return shortestPath;
  }

  const Vector2& startPos = pathOwner->GetGridPosition();

  // The current list of points we are going through
  std::vector< Vector2 > currentPath;
  currentPath.push_back(startPos);

  std::vector< std::pair< Vector2, uint32 > > visitedTiles;
  visitedTiles.push_back(std::pair< Vector2, uint32 >(startPos, 0));

  GetShortestPathToGridPosition(startPos, shortestPath, currentPath, playerPositions, visitedTiles, 0, 30, mustWalk);
  if (!shortestPath.empty())
  {
    AddPathfinding(pathOwner, shortestPath);
  }
  else
  {
    ClearPathfindingPath(pathOwner->GetInstance());
  }

  return shortestPath;
}

void GridTilePathfinding::GetShortestPathToGridPosition(const Vector2& gridPosition,
  std::vector< Vector2 >& shortestPath,
  std::vector< Vector2 >& currentPath,
  std::vector< Vector2 >& objectiveList,
  std::vector< std::pair< Vector2, uint32 > >& visitedTiles,
  uint32 level,
  uint32 maxLevel,
  bool mustWalk)
{
  uint32 currentLevel = level + 1;

  // Fail safe for now
  if (currentLevel > maxLevel)
  {
    return;
  }

  // Do not bother to check this path further since it will be longer than my current path
  if (!shortestPath.empty() && currentLevel > shortestPath.size())
  {
    return;
  }

  // Make sure the tile is not outside of the grid
  BaseTile* tile = m_Playfield->GetTile(gridPosition);
  if (tile == NULL)
  {
    return;
  }

  //// If the tile is empty, add 1 extra cost to it since 2 walking tiles = 1 building tile
  ////uint32 emptyTileDoors = 0;
  //if ( IsEmpty( tile ) )
  //{
  //  int numNeighbours = 0;
  //  for ( int side = 0; side < tile->GetNumSides(); side++ )
  //  {
  //    uint32 door = 1 << side;

  //    const Vector2& gridPosition = tile->GetWorldGridPosFromSide( door );
  //    if ( gridPosition == Vector2::s_Vec2Minus )
  //    {
  //      continue;
  //    }

  //    BaseTile* tile2 = GetTile( gridPosition.GetIntX(), gridPosition.GetIntY() );
  //    if ( tile2 != NULL && !IsEmpty( tile2 ) && tile2->HasConnectedDoor( door ) )
  //    {
  //      //emptyTileDoors += door;
  //      numNeighbours++;
  //    }
  //  }


  //  if ( numNeighbours > 0 && numNeighbours < 4 )
  //  {
  //    currentLevel++;
  //  }
  //  else if ( numNeighbours > 0 )
  //  {
  //    LogInfoFMT( "Path","[%d] Tile: [%d x %d] numNeighbours : %d", level, gridPosition.GetIntX(), gridPosition.GetIntY(), numNeighbours );
  //    currentLevel += numNeighbours - 2;  // 4,5,6 neighbours will have 2,3,4 cost to build there
  //  }
  //}

  // Check if this is one of the objectives
  if (HasTilePos(objectiveList, gridPosition))
  {
    shortestPath.clear();
    shortestPath.insert(shortestPath.begin(), currentPath.begin(), currentPath.end());
    return;
  }

  for (int side = 0; side < tile->GetNumSides(); side++)
  {
    uint32 door = 1 << side;

    bool isTileEmpty = m_Playfield->IsEmpty(tile);

    // Check if we can only path on placed tiles
    if (isTileEmpty && mustWalk)
    {
      continue;
    }

    const Vector2& localGridPosition = tile->GetWorldGridPosFromSide(door);
    if (localGridPosition == Vector2::s_Vec2Minus)
    {
      continue;
    }

    // We are required to have matching doors on the tiles
    BaseTile* neighbourTile = m_Playfield->GetTile(localGridPosition.GetIntX(), localGridPosition.GetIntY());
    if (neighbourTile == NULL)
    {
      continue;
    }

    // Must have an open door on both ends
    if (!tile->HasDoor(door) || !neighbourTile->HasConnectedDoor(door))
    {
      continue;
    }

    // If the neighbour tile is empty, add 1 extra cost to it since 2 walking tiles = 1 building tile
    //uint32 emptyTileDoors = 0;
    int bonusLevel = currentLevel;
    if (m_Playfield->IsEmpty(neighbourTile))
    {
      int numNeighbours = 0;
      for (int nSide = 0; nSide < neighbourTile->GetNumSides(); nSide++)
      {
        uint32 nDoor = 1 << nSide;

        const Vector2& neighbourGridPosition = neighbourTile->GetWorldGridPosFromSide(nDoor);
        if (neighbourGridPosition == Vector2::s_Vec2Minus)
        {
          continue;
        }

        BaseTile* tile2 = m_Playfield->GetTile(neighbourGridPosition.GetIntX(), neighbourGridPosition.GetIntY());
        if (tile2 != NULL && !m_Playfield->IsEmpty(tile2) && tile2->HasConnectedDoor(nDoor))
        {
          numNeighbours++;
        }
      }

      if (numNeighbours > 1 && numNeighbours < 4)
      {
        bonusLevel++;
      }
      else if (numNeighbours > 1)
      {
        bonusLevel += numNeighbours - 2;  // 4,5,6 neighbours will have 2,3,4 cost to build there
      }
    }

    // Check if I have been to the neighbour before
    if (!HasTilePos(visitedTiles, localGridPosition, bonusLevel))
    {
      currentPath.push_back(localGridPosition);
      GetShortestPathToGridPosition(localGridPosition, shortestPath, currentPath, objectiveList, visitedTiles, bonusLevel, maxLevel, mustWalk);
      currentPath.pop_back();
    }
  }
}

std::vector< Vector2 > GridTilePathfinding::GetMaxPath(const std::vector< Vector2 >& fullPath, uint32 maxDistance)
{
  std::vector< Vector2 > pathList;
  if (!fullPath.empty())
  {
    pathList.insert(pathList.begin(), fullPath.begin(), fullPath.begin() + maxDistance);
  }
  return pathList;
}

// Return true if the tile is in the list and closer/equand distance away from the startpoint
// False if it should go there because it is not in the list or shorter from the start point
bool GridTilePathfinding::HasTilePos(std::vector< std::pair< Vector2, uint32 > >& visitedTiles, const Vector2& gridPosition, uint32 currentLevel)
{
  for (uint32 index = 0; index < visitedTiles.size(); index++)
  {
    std::pair< Vector2, uint32 >& point = visitedTiles[index];
    if (point.first == gridPosition)
    {
      if (point.second > currentLevel)
      {
        point.second = currentLevel;
        return false;
      }
      return true;
    }
  }

  visitedTiles.push_back(std::pair< Vector2, uint32 >(gridPosition, currentLevel));
  return false;
}

bool GridTilePathfinding::HasTilePos(std::vector< Vector2 > & tilePositionList, const Vector2& gridPosition)
{
  for (uint32 index = 0; index < tilePositionList.size(); index++)
  {
    Vector2& point = tilePositionList[index];
    if (point == gridPosition)
    {
      return true;
    }
  }
  return false;
}


bool GridTilePathfinding::UpdateTileColors( BaseTile* tile )
{
  bool updated = false;
#if defined VISUALIZE_PATHFINDING
  if (s_ShowPathfinding)
  {
    for (std::map< uint32, PathFinding >::iterator itPath = m_PathfindingList.begin(); itPath != m_PathfindingList.end(); ++itPath)
    {
      PathFinding& path = itPath->second;
      if (path.m_IsVisible && !updated)
      {
        for (uint32 index = 0; index < path.m_Nodes.size(); index++)
        {
          const Vector2& point = path.m_Nodes[index];
          if (point == tile->GetGridPosition() )
          {
            const Color& color = GridTilePathfinding::GetPathfindingColor(path.m_ColorIndex);
            tile->SetColor(color);
            updated = true;
            break;
          }
        }
      }
    }
  }
#endif
  return updated;
}