#ifndef EVILENGINE_CORENETWORK_ZLIBCOMPRESSION
#define EVILENGINE_CORENETWORK_ZLIBCOMPRESSION

#include "zlib.h"

class BinaryStream;

class ZlibCompression
{
public:

  ZlibCompression();
  ~ZlibCompression();

  BinaryStream* Decompress( uint8* compressedStreamData, uint32 compressedStreamSize );
  BinaryStream* Compress( uint8* uncompressedStreamData, uint32 uncompressedStreamSize );
  static bool UnitTest( String message );

private:
  z_stream* m_RecvStream;
  z_stream* m_SendStream;
};

#endif /// EVILENGINE_CORENETWORK_ZLIBCOMPRESSION