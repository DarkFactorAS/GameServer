#ifndef PROJECT_PROJECTGAMEMANAGEMENTMODULE_DATA_LOBBYGAMEPLAYER
#define PROJECT_PROJECTGAMEMANAGEMENTMODULE_DATA_LOBBYGAMEPLAYER 1

class LobbyGamePlayer
{
public:

  LobbyGamePlayer() :
    m_AccountId(0),
    m_RobotId(0)
  {
  }

  LobbyGamePlayer(uint32 accountId, const String& playerName, uint32 robotId) :
    m_AccountId(accountId),
    m_PlayerName(playerName),
    m_RobotId(robotId)
  {
  }

  LobbyGamePlayer(LobbyGamePlayer* otherPlayer) :
    m_AccountId(0),
    m_RobotId(0)
  {
    if (otherPlayer != NULL)
    {
      m_AccountId = otherPlayer->m_AccountId;
      m_PlayerName = otherPlayer->m_PlayerName;
      m_RobotId = otherPlayer->m_RobotId;
    }
  }


  uint32 GetAccountId() const { return m_AccountId; }
  uint32 GetRobotId() const { return m_RobotId; }
  const String& GetPlayerName() const{ return m_PlayerName; }

private:

  uint32 m_AccountId;
  uint32 m_RobotId;
  String m_PlayerName;

};

#endif /// PROJECT_PROJECTGAMEMANAGEMENTMODULE_DATA_LOBBYGAMEPLAYER
