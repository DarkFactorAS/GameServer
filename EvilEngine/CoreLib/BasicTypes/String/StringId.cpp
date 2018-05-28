/*************************************************************************************
* Project     : EvilEngine CoreLib 2012
* File        : String
* Description : A custom string class to make it easier to change the underlying type
*               used for strings. This class also has some utility functions to handle
*               comparing, finding and converting strings.
*
* Author      : Thor Richard Hansen
 *************************************************************************************/

#include "Precompile.h"

#include "StringId.h"
#include "EvilEngine/CoreLib/Crypto/ElfHash.h"

#include <map>

StringId::StringId(const char* str)
{
  m_Hash = Crypto::ElfHash(str);
#ifdef DEBUG
  m_Name = str;
  CheckCollisions();
#endif
}

#ifdef DEBUG

void StringId::CheckCollisions()
{
  static std::map<StringId::Key, std::string> s_CollisionMap;

  std::map<Key, std::string>::iterator iCollision = s_CollisionMap.find(m_Hash);
  if (iCollision != s_CollisionMap.end())
  {
    dfAssert(iCollision->second == m_Name,"Collided with string!");
  }
  else
  {
    s_CollisionMap[m_Hash] = m_Name;
  }
}

#endif

StringId::Key StringId::ComputeHash(const char* str)
{
  return Crypto::ElfHash(str);
}
