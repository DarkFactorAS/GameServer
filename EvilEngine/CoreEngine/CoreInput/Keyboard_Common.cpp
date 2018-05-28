
#include "Precompile.h"
#include "Keyboard.h"

Keyboard::Keyboard()
{

}

bool Keyboard::IsValidCharacter( char c, uint32 keyValidator )
{
  // 0-9
  if ( c >= KEY_ZERO && c <= KEY_NINE )
  {
    return ( keyValidator & ALLOW_09 ) == ALLOW_09;
  }
  // a-z
  if ( c >= KEY_A && c <= KEY_Z )
  {
    return ( keyValidator & ALLOW_AZ ) == ALLOW_AZ;
  }
  // A-Z
  if ( c >= KEY_CAPITAL_A && c <= KEY_CAPITAL_Z )
  {
    return ( keyValidator & ALLOW_AZ ) == ALLOW_AZ;
  }

  // Some special characters
  switch( c )
  {
    case KEY_AT:
    case KEY_PERIOD:
    case KEY_MINUS:
      return ( keyValidator & ALLOW_EMAIL ) == ALLOW_EMAIL;
    case KEY_SPACE:
      return ( keyValidator & ALLOW_SPACE ) == ALLOW_SPACE;
  }

  return false;
}
