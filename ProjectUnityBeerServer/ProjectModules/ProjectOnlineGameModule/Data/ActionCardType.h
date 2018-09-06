#pragma once

namespace ActionCards
{
  enum ActionCardType
  {
    None,
    MoveOneForward,
    MoveTwoForward,
    MoveThreeForward,
    MoveOneBack,
    TurnLeft,
    TurnRight,
    UTurn,
    Template,
    PlayerQueueSize = 5,
  };
}
