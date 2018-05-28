
#include "PreCompile.h"

#ifdef WINDOWS

#include "Keyboard.h"

Keyboard::Key Keyboard::GetKey( int /*keycode*/, int /*metastate*/ )
{
  return Key::KEY_NONE;
}

int Keyboard::GetUnicode(int /*keycode*/, int /*metastate*/ )
{
  return Key::KEY_NONE;
}

#if 0
    case WM_KEYDOWN:
      if (wParam == VK_SHIFT || wParam == VK_LSHIFT || wParam == VK_RSHIFT)
        shiftDown = true;

      if (wParam == VK_CAPITAL)
        capsOn = !capsOn;

      // Suppress key repeats.
      if ((lParam & 0x40000000) == 0)
        gameplay::Platform::keyEventInternal(gameplay::Keyboard::KEY_PRESS, getKey(wParam, shiftDown ^ capsOn));
      break;

    case WM_KEYUP:
      if (wParam == VK_SHIFT || wParam == VK_LSHIFT || wParam == VK_RSHIFT)
        shiftDown = false;

      gameplay::Platform::keyEventInternal(gameplay::Keyboard::KEY_RELEASE, getKey(wParam, shiftDown ^ capsOn));
      break;

    case WM_CHAR:
      // Suppress key repeats.
      if ((lParam & 0x40000000) == 0)
        gameplay::Platform::keyEventInternal(gameplay::Keyboard::KEY_CHAR, wParam);
      break;

    case WM_UNICHAR:
      // Suppress key repeats.
      if ((lParam & 0x40000000) == 0)
        gameplay::Platform::keyEventInternal(gameplay::Keyboard::KEY_CHAR, wParam);
      break;

#endif

#endif
