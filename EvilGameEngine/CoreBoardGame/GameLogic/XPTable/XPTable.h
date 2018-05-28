#ifndef EVILENGINE_COREBOARDGAME_GAMELOGIC_ITEM
#define EVILENGINE_COREBOARDGAME_GAMELOGIC_ITEM 1
#pragma once

#include "EvilGameEngine/CoreBoardGame/GameEngine/CoreBaseRenderObject.h"
#include "EvilGameEngine/CoreBoardGame/GameLogic/GameObject/GameObject.h"

class RenderObject;
class BinaryStream;

class XPTable 
{
public:

  struct XpEntry
  {
    uint32 m_FromXp;
    uint32 m_ToXp;
    uint8  m_Level;

    XpEntry( uint32 fromXp, uint32 toXp, uint8 level )
    {
      m_FromXp  = fromXp;
      m_ToXp    = toXp;
      m_Level   = level;
    }
  };

  static XPTable* GetInstance();

  void                    Load();
  uint8                   GetLevel( uint32 xp );
  uint8                   GetMaxLevel();
  uint32                  GetMaxXp();

protected:

  XPTable();

  std::vector< XpEntry >  m_XpList;
  uint8                   m_MaxLevel;
  uint32                  m_MaxXp;
};

#endif /// EVILENGINE_COREBOARDGAME_GAMELOGIC_ITEM
