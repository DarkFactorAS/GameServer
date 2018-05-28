#ifndef EVILENGINE_CORELIB_CRYPTO_ELFHASH
#define EVILENGINE_CORELIB_CRYPTO_ELFHASH 1
#pragma once

namespace Crypto
{
  uint32 ElfHash(const unsigned char* str);
  uint32 ElfHash(const String& str);
}

#endif // EVILENGINE_CORELIB_CRYPTO_ELFHASH
