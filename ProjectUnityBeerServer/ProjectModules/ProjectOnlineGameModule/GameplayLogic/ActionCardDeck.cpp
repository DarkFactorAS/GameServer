/*************************************************************************************
* Project     : EvilEngine Multiplatform 2014-2018
* File        : ProjectOnlineGameServerModule
* Description : Abstract interface class for CoreGameEngine modules. This interface
*               has to be implemented to be able to run as a sub component in the
*               CoreGameEngine.
*
* Author      : Thor Richard Hansen
*************************************************************************************/

#include "PreCompile.h"
#include "ActionCardDeck.h"

ActionCardDeck::ActionCardDeck()
{
  CreateDeck();
}

void ActionCardDeck::CreateDeck()
{
  AddCard(ActionCards::MoveOneForward, 8);
  AddCard(ActionCards::MoveTwoForward, 8);
  AddCard(ActionCards::MoveThreeForward, 8);
  AddCard(ActionCards::MoveOneBack, 8);
  AddCard(ActionCards::TurnLeft, 8);
  AddCard(ActionCards::TurnRight, 8);
  AddCard(ActionCards::UTurn, 8);

  // Shuffle deck
}

void ActionCardDeck::AddCard(ActionCards::ActionCardType actioncardType, uint32 numCards)
{
  uint32 startRange = ActionCardTypeRange(actioncardType);
  for (uint32 index = 0; index < numCards; index++)
  {
    uint32 initiative = startRange + ( index * 10 );
    m_Deck.push_back( new ActionCard(actioncardType, initiative ) );
  }
}

uint32 ActionCardDeck::ActionCardTypeRange(ActionCards::ActionCardType actioncardType )
{
  switch (actioncardType)
  {
    case ActionCards::MoveOneForward:
      return 100;
    case ActionCards::MoveTwoForward:
      return 200;
    case ActionCards::MoveThreeForward:
      return 300;
    case ActionCards::MoveOneBack:
      return 400;
    case ActionCards::TurnLeft:
      return 500;
    case ActionCards::TurnRight:
      return 600;
    case ActionCards::UTurn:
      return 700;
  }
  return 0;
}

std::vector<ActionCard*> ActionCardDeck::GetCards(uint32 numCards)
{
  std::vector<ActionCard*> cardList;
  for (uint32 index = 0; index < numCards; index++)
  {
    ActionCard* card = m_Deck[index];
    cardList.push_back( card );
  }
  return cardList;
}
