/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : Item
*
* Description : Item class that can be placed in the world
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "XPTable.h"

#ifdef SERVER
  #include "EvilEngine/CoreDatabase/CoreDatabase.h"
#endif

XPTable::XPTable() :
  m_MaxLevel( 0 ),
  m_MaxXp( 0 )
{
  Load();
}

XPTable* XPTable::GetInstance()
{
  static XPTable* instance = NULL;
  if ( instance == NULL )
  {
    instance = new XPTable();
  }
  return instance;
}

void XPTable::Load()
{
#ifdef SERVER
  uint32 startXp = 0;

  const SQLResultSet& result = CoreDatabase::GetInstance()->ExecuteSelect( "select level, xp from xptable order by level" );
  while ( result.Next() )
  {
    uint8 level = (uint8)result.GetUInt32();
    uint32 endXp = result.GetUInt32();

    m_XpList.push_back( XpEntry( startXp, endXp, level ) );
    startXp = endXp + 1;

    m_MaxLevel = level;
    m_MaxXp    = endXp;
  }
#endif
}

uint8 XPTable::GetLevel( uint32 xp )
{
  for ( uint32 index = 0; index < m_XpList.size(); index++ )
  {
    const XpEntry& entry = m_XpList[ index ];
    if ( entry.m_FromXp <= xp && xp <= entry.m_ToXp )
    {
      return entry.m_Level;
    }
  }
  return 0;
}

uint8 XPTable::GetMaxLevel()
{
  return m_MaxLevel;
}

uint32 XPTable::GetMaxXp()
{
  return m_MaxXp;
}
