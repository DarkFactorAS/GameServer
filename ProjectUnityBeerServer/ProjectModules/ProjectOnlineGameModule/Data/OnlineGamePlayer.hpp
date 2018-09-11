#ifndef PROJECT_ONLINEGAME_DATA_ONLINEGAMEPLAYER
#define PROJECT_ONLINEGAME_DATA_ONLINEGAMEPLAYER 1

#include "ProjectModules/ProjectOnlineGameModule/GameplayLogic/ActionCardInventory.h"

class OnlineGamePlayer
{
public:
  OnlineGamePlayer(uint32 acccountId, const String& playerName, uint32 robotId, uint32 spawnpointId) :
    m_AccountId(acccountId),
    m_PlayerName(playerName),
    m_RobotId(robotId),
    m_SpawnPointId(spawnpointId)
  {
    m_Inventory = new ActionCardInventory();
  }

  uint32 GetAccountId() const{ return m_AccountId; }
  uint32 GetRobotID() const{ return m_RobotId; }
  uint8 GetLives() const{ return m_Lives; }
  String GetPlayerName() const { return m_PlayerName; }

  uint16 GetPositionX() const{ return m_Position.GetUInt16X(); }
  uint16 GetPositionY() const { return m_Position.GetUInt16Y(); }
  uint32 GetSpawnPointId() const{ return m_SpawnPointId; }

  ActionCardInventory*  GetInventory() const { return m_Inventory; }

private:

  uint32                m_AccountId;
  String                m_PlayerName;
  uint32                m_RobotId;
  uint8                 m_Lives;
  uint32                m_SpawnPointId;
  Vector2               m_Position;
  ActionCardInventory*  m_Inventory;
};

#endif /// PROJECT_ONLINEGAME_DATA_ONLINEGAMEPLAYER
