/*************************************************************************************
* Project     : EvilEngine Multiplatform 2014-2018
* File        : ActionCardInventory
* Description : 
*
* Author      : Thor Richard Hansen
*************************************************************************************/

#include "PreCompile.h"
#include "ActionCardInventory.h"
#include "ProjectOnlineGameModule/Data/ActionCard.hpp"

ActionCardInventory::ActionCardInventory()
{
}

void ActionCardInventory::AddItems(std::vector< ActionCard* > itemList)
{
  m_Inventory.insert( m_Inventory.begin(), itemList.begin(), itemList.end() );
}
