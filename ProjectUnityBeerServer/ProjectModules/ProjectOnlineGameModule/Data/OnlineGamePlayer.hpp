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

private:

  uint32 m_AccountId;
  String m_PlayerName;
  uint32 m_RobotId;

};

#endif /// PROJECT_ONLINEGAME_DATA_ONLINEGAMEPLAYER
