#ifndef PROJECT_ONLINEGAME_DATA_ONLINEGAMEPLAYER
#define PROJECT_ONLINEGAME_DATA_ONLINEGAMEPLAYER 1

class OnlineGamePlayer
{
public:
  OnlineGamePlayer(uint32 acccountId, const String& playerName, uint32 robotId) :
    m_AccountId(acccountId),
    m_PlayerName(playerName),
    m_RobotId(robotId)
  {
  }

  uint32 GetAccountId() const{ return m_AccountId; }
  uint16 GetRobotID() const{ return m_RobotId; }
  uint8 GetLives() const{ return m_Lives; }
  String GetPlayerName() const { return m_PlayerName; }

  uint16 GetPositionX() const{ return m_Position.x; }
  uint16 GetPositionY() const { return m_Position.y; }

private:

  uint32 m_AccountId;
  String m_PlayerName;
  uint32 m_RobotId;
  uint8 m_Lives;

  Vector2 m_Position;

};

#endif /// PROJECT_ONLINEGAME_DATA_ONLINEGAMEPLAYER
