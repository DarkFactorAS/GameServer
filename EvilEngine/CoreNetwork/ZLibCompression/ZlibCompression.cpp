
#include "Precompile.h"
#include "ZLibCompression.h"
#include "CoreLib/BasicTypes/BinaryStream/BinaryStream.h"

const int ZLIB_BUFFER_SIZE  = 65535;

ZlibCompression::ZlibCompression()
{
  m_SendStream = (z_stream*)malloc(sizeof(*m_SendStream));
  memset(m_SendStream, 0, sizeof(*m_SendStream));
  deflateInit(m_SendStream, 1);

  m_RecvStream = (z_stream*)malloc(sizeof(*m_RecvStream));
  memset(m_RecvStream, 0, sizeof(*m_RecvStream));
  inflateInit(m_RecvStream);
}

/*******************************************************************
* Author: Thor Richard Hansen
*******************************************************************/
ZlibCompression::~ZlibCompression()
{
  if( m_RecvStream != NULL )
  {
    inflateEnd(m_RecvStream);
    free(m_RecvStream);
    m_RecvStream = NULL;
  }

  if( m_SendStream != NULL )
  {
    deflateEnd(m_SendStream);
    free(m_SendStream);
    m_SendStream = NULL;
  }
}

BinaryStream* ZlibCompression::Decompress( uint8* compressedStreamData, uint32 compressedStreamSize )
{
  if(m_RecvStream)
  {
    unsigned char decompressBuffer[ ZLIB_BUFFER_SIZE ];
    BinaryStream* outStream = new BinaryStream();

    m_RecvStream->avail_in = compressedStreamSize;
    m_RecvStream->next_in = compressedStreamData;

    int decompressionResult = 0;
    do 
    {
      m_RecvStream->avail_out = sizeof(decompressBuffer);
      m_RecvStream->next_out = decompressBuffer;

      decompressionResult = inflate(m_RecvStream, Z_SYNC_FLUSH);
      if(decompressionResult != Z_OK)
      {
        delete outStream;
        return NULL;
      }
      size_t decompressedSize = m_RecvStream->next_out - decompressBuffer;

      // Add the data to the out stream
      outStream->WriteRaw( decompressBuffer, (uint32) decompressedSize );

    } while (m_RecvStream->avail_in != 0 || (decompressionResult == Z_OK && m_RecvStream->avail_out==0));

    return outStream;
  }
  return NULL;
}

BinaryStream* ZlibCompression::Compress( uint8* uncompressedStreamData, uint32 uncompressedStreamSize )
{
  m_SendStream->avail_in  = uncompressedStreamSize;
  m_SendStream->next_in   = uncompressedStreamData;

  BinaryStream* outStream = new BinaryStream();

  int deflateResult = 0;
  do 
  {
    uint8 compressionBuffer[ ZLIB_BUFFER_SIZE ];
    m_SendStream->avail_out = (uint)sizeof(compressionBuffer);
    m_SendStream->next_out = (Bytef*)compressionBuffer;
    deflateResult = deflate(m_SendStream, Z_SYNC_FLUSH);
    if(deflateResult != Z_OK && deflateResult != Z_BUF_ERROR)
    {
      return NULL;
    }
    outStream->WriteRaw( compressionBuffer, m_SendStream->next_out - compressionBuffer );

  } while (m_SendStream->avail_in != 0 || (m_SendStream->avail_out == 0 && Z_OK == deflateResult));

  return outStream;
}

bool ZlibCompression::UnitTest( String message )
{
  ZlibCompression* m_Compressor = new ZlibCompression();
  BinaryStream* compDataStream = m_Compressor->Compress( (uint8*) message.c_str(), message.Length() );
  BinaryStream* uncompStream = m_Compressor->Decompress( (uint8*) compDataStream->GetData(), compDataStream->GetSize() );
  const char* outTxt = uncompStream->ReadRaw( uncompStream->GetSize() );
  if ( message.CompareNoCase( outTxt ) == 0 )
  {
    return true;
  }
  return false;
}
