/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : Item
*
* Description : Item class that can be placed in the world
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "Item.h"

Item::Item() : 
  GameObject()
{
}

Item::Item( String name, uint32 iconId, const Color& color ) :
  GameObject( ++s_Id, e_Item, name, iconId, color )
{
}

Item::Item( String name, uint32 iconId, const Color& color, const Vector3& position, const Vector3& size ) :
  GameObject( ++s_Id, e_Item, name, iconId, color, position, size )
{
}

Item::Item( const Item* item ) :
  GameObject( item )
{
}


GameObject* Item::Clone() const
{
  return new Item( this );
}
