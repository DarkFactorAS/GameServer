#ifndef EVILENGINE_COREBOARDGAME_GAMELOGIC_ITEM
#define EVILENGINE_COREBOARDGAME_GAMELOGIC_ITEM 1
#pragma once

#include "EvilGameEngine/CoreBoardGame/GameEngine/CoreBaseRenderObject.h"
#include "EvilGameEngine/CoreBoardGame/GameLogic/GameObject/GameObject.h"

class RenderObject;
class BinaryStream;

class Item : public GameObject
{
public:
  Item();
  Item( String itemName, uint32 iconId, const Color& color );
  Item( String itemName, uint32 iconId, const Color& color, const Vector3& position, const Vector3& size );
  virtual ~Item(){};

  virtual GameObject*             Clone() const DF_OVERRIDE;

protected:

  Item( const Item* item );
};

#endif /// EVILENGINE_COREBOARDGAME_GAMELOGIC_ITEM
