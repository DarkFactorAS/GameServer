#ifndef EVILENGINE_CORELIB_DEFINITIONS_BASICTYPES_H
#define EVILENGINE_CORELIB_DEFINITIONS_BASICTYPES_H

//#define CC_VERSION 1,0,0
//#define VER_NAME(ver) VER_NAME2(ver)
//#define VER_NAME2(ver) #ver

#define COMPANY_NAME "DarkFactor"

#if defined PLATFORM_LINUX
  #include <inttypes.h>
#else
  typedef unsigned char      uint8_t;
  typedef unsigned short     uint16_t;
  typedef unsigned int       uint_t;
  typedef unsigned int       uint32_t;
#endif

typedef unsigned char      uint8;
typedef unsigned short     uint16;
typedef unsigned int       uint;
typedef unsigned int       uint32;
typedef unsigned long long uint64;

typedef int                int32;

#define Swap8Bytes(val) \
 ( (((val) >> 56) & 0x00000000000000FF) | (((val) >> 40) & 0x000000000000FF00) | \
 (((val) >> 24) & 0x0000000000FF0000) | (((val) >>  8) & 0x00000000FF000000) | \
 (((val) <<  8) & 0x000000FF00000000) | (((val) << 24) & 0x0000FF0000000000) | \
 (((val) << 40) & 0x00FF000000000000) | (((val) << 56) & 0xFF00000000000000) )

#endif /// EVILENGINE_CORELIB_DEFINITIONS_BASICTYPES_H
