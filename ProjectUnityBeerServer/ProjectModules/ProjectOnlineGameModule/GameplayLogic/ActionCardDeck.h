#ifndef PROJECT_PROJECTONLINEGAMEMODULE_GAMEPLAYLOGIC_ACTIONCARDDECK
#define PROJECT_PROJECTONLINEGAMEMODULE_GAMEPLAYLOGIC_ACTIONCARDDECK 1

#include "EvilEngine/CoreEngine/CoreEngine/EngineModule.h"
#include "EvilEngine/CoreLib/BasicTypes/Definitions/VolatileArgs.h"

#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngineModule.h"

//#include "ProjectModules/ProjectOnlineGameModule/Data/OnlineGameData.hpp"
#include "ProjectModules/ProjectOnlineGameModule/Data/ActionCardType.h"
#include "ProjectModules/ProjectOnlineGameModule/Data/ActionCard.hpp"

class ActionCardDeck
{
public:

  ActionCardDeck();

  void                        CreateDeck();
  std::vector<ActionCard*>    GetCards(uint32 numCards);

private:

  void                        AddCard(ActionCards::ActionCardType actioncardType, uint32 numCards);
  uint32                      ActionCardTypeRange(ActionCards::ActionCardType actioncardType);

  std::vector<ActionCard*>    m_Deck;
};

#endif /// PROJECT_PROJECTONLINEGAMEMODULE_GAMEPLAYLOGIC_ACTIONCARDDECK
