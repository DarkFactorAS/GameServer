#ifndef PROJECT_ONLINEGAME_DATA_ACTIONCARD
#define PROJECT_ONLINEGAME_DATA_ACTIONCARD 1

#include "ProjectModules/ProjectOnlineGameModule/Data/ActionCardType.h"

class ActionCard
{
public:

  ActionCard() :
    m_CardType(ActionCards::None),
    m_Initiative(0)
  {
  }

  ActionCard(ActionCards::ActionCardType cardType, uint32 initiative) :
    m_CardType(cardType),
    m_Initiative(initiative)
  {
  }

  ActionCards::ActionCardType GetCardType()
  {
    return m_CardType;
  }

  uint32 GetInitiative()
  {
    return m_Initiative;
  }

private:

  ActionCards::ActionCardType m_CardType;
  uint32                      m_Initiative;
};

#endif /// PROJECT_ONLINEGAME_DATA_ONLINEGAMEDATA
