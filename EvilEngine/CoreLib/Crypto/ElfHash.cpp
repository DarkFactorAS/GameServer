#include "Precompile.h"

#include "ElfHash.h"

uint32 Crypto::ElfHash(const unsigned char* str)
{
  unsigned int h = 0, g;
  while ( *str )
  {
    h = ( h << 4 ) + *str++;
    g = ( h & 0xF0000000 );
    if( g )
    {
      h ^= g >> 24;
    }
    h &= ~g;
  }
  return h;
}

uint32 Crypto::ElfHash(const String& str)
{
  return ElfHash(reinterpret_cast<const unsigned char*>(str.c_str()));
}
