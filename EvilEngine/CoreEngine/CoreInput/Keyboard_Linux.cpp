
#include "Precompile.h"

#ifdef LINUX

#include "Keyboard.h"

Keyboard::Key Keyboard::GetKey( int /*keycode*/, int /*metastate*/ )
{
  return Keyboard::KEY_NONE;
}

int Keyboard::GetUnicode(int /*keycode*/, int /*metastate*/ )
{
  return Keyboard::KEY_NONE;
}

#endif
