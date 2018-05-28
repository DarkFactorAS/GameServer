/************************************************************************
* Project     : Conan Chat Client 2008-2011
* Author      : Thor Richard Hansen
* License     : This program is free software; you can redistribute it
*               and/or modify it under the terms of the GNU General 
*               Public License as published by the Free Software 
*               Foundation; version 2 of the License only.
*               This program is distributed without ANY WARRANTY.
*
* File        : BinaryStream
* Description : Read/Write data from/to a data blob
*************************************************************************/
#include "Precompile.h"
#include "BinaryStream.h"
#include "stdlib.h"
#include "string.h"
#include "EvilEngine/CoreLib/Assert/DFAssert.h"

const BinaryStream BinaryStream::s_Empty;
const BinaryStream BinaryStream::zero;

// Cap the binarystream blobs to 10mb
const uint32 BinaryStream::MAX_DATA_SIZE = 10485760;

BinaryStream::BinaryStream() :
  m_ReadStreamPtr( 0 ),
  m_WriteStreamPtr( 0 ),
  m_DataSize( 0 ),
  m_AllocSize( 0 ),
  m_IsOK( true ),
  m_ByteOrder(BigEndian),
  m_Data(NULL)
{
}

BinaryStream::BinaryStream( ByteOrder_e byteOrder ) :
  m_ReadStreamPtr( 0 ),
  m_WriteStreamPtr( 0 ),
  m_DataSize( 0 ),
  m_AllocSize( 0 ),
  m_IsOK( true ),
  m_ByteOrder(byteOrder),
  m_Data(NULL)
{
}

BinaryStream::BinaryStream( const char* dataBlob, uint32 blobLength, ByteOrder_e byteOrder ) : 
  m_ReadStreamPtr( 0 ),
  m_WriteStreamPtr( 0 ),
  m_IsOK( true ),
  m_ByteOrder(byteOrder)
{
  m_DataSize = m_AllocSize = blobLength;

  m_Data = (char*) malloc( m_AllocSize );
  if ( m_Data != NULL )
  {
    memcpy( m_Data, dataBlob, m_AllocSize );    
  }
}

BinaryStream::BinaryStream( const uint8* dataBlob, uint32 blobLength, ByteOrder_e byteOrder ) : 
  m_ReadStreamPtr( 0 ),
  m_WriteStreamPtr( 0 ),
  m_IsOK( true ),
  m_ByteOrder(byteOrder)
{
  m_DataSize = m_AllocSize = blobLength;

  m_Data = (char*) malloc( m_AllocSize );
  if ( m_Data != NULL )
  {
    memcpy( m_Data, dataBlob, m_AllocSize );    
  }
}

BinaryStream::BinaryStream( const BinaryStream* otherStream ) :
  m_ReadStreamPtr( 0 ),
  m_IsOK( true )
{
  m_AllocSize       = otherStream->m_AllocSize;
  m_DataSize        = otherStream->m_DataSize;
  m_WriteStreamPtr  = otherStream->m_WriteStreamPtr;
  m_ByteOrder       = otherStream->m_ByteOrder;

  m_Data = (char*) malloc( m_AllocSize );
  if ( m_Data != NULL )
  {
    memcpy( m_Data, otherStream->m_Data, m_AllocSize );    
  }
}

BinaryStream::BinaryStream(const BinaryStream* otherStream, uint32 blobLength) :
  m_ReadStreamPtr(0),
  m_WriteStreamPtr(0),
  m_IsOK(true)
{
  uint32 readPos = otherStream->GetReadPos();
  uint32 dataSize = otherStream->m_DataSize - readPos;

  if (blobLength > dataSize)
  {
    blobLength = dataSize;
  }

  if (blobLength < dataSize)
  {
    m_AllocSize = blobLength;
    m_DataSize = blobLength;
  }
  else
  {
    m_AllocSize = dataSize;
    m_DataSize = dataSize;
  }
  m_ByteOrder = otherStream->m_ByteOrder;

  m_Data = (char*)malloc(m_AllocSize);
  if (m_Data != NULL)
  {
    memcpy(m_Data, otherStream->m_Data + readPos, m_AllocSize);
  }
}

BinaryStream::~BinaryStream()
{
  if ( m_Data != NULL )
  {
    // Gennerated heap corruption..have to debug this a bit
    //free( m_Data );
    m_Data = NULL;
  }
}

/// VerifyRead
/// Checks if we are trying to read outside the buffer
/// @param size [in] The length of the data we want to read
/// @return bool [out] True if it is ok to read this value
/// @author Thor Richard Hansen
bool BinaryStream::VerifyRead( int size ) const
{
  if ( m_ReadStreamPtr + size > m_DataSize )
  {
#ifdef DEBUG
    if ( m_IsOK && dfAssertHandler::AmIBeingDebugged() )
    {
      dfBugreport( false, "BinaryStream::ReadOverflow" );
    }
#endif
    m_IsOK = false;
  }
  return m_IsOK;
}

/// VerifyRead
/// Checks if we are trying to read outside the buffer
/// @param size [in] The length of the data we want to read
/// @return bool [out] True if it is ok to read this value
/// @author Thor Richard Hansen
bool BinaryStream::VerifyRead( uint32 offset, int size ) const
{
  if ( offset + size > m_DataSize )
  {
#ifdef DEBUG
    if ( m_IsOK )
    {
      dfBugreport( false, "BinaryStream::ReadOverflow" );
    }
#endif
    m_IsOK = false;
  }
  return m_IsOK;
}

/// ReadUInt8
/// Reads one byte from the databuffer
/// @return uint8 [ret] Value on the current location
/// @author Thor Richard Hansen
uint8 BinaryStream::ReadUInt8() const
{
  if ( VerifyRead( 1 ) )
  {
    uint8 data = uint8( m_Data[ m_ReadStreamPtr++ ] );
    return data;
  }
  return 0;
}

/// ReadBoolean
/// Reads one byte from the databuffer
/// @return bool [ret] Value on the current location
/// @author Thor Richard Hansen
bool BinaryStream::ReadBoolean() const
{
  if ( VerifyRead( 1 ) )
  {
    uint8 data = uint8( m_Data[ m_ReadStreamPtr++ ] );
    return ( data != 0 ) ? true : false;
  }
  return false;
}

/***********************************************************************************
* ReadUInt16:
* Reads two bytes from the databuffer
*
* @return (uint16) The uint16 value that was in the databuffer
*
* @author Thor Richard Hansen
***********************************************************************************/
uint16 BinaryStream::ReadUInt16() const
{
  uint16 data = 0;
  if ( VerifyRead( 2 ) )
  {
    uint16 b0   = uint8( m_Data[ m_ReadStreamPtr++ ] );
    uint16 b1   = uint8( m_Data[ m_ReadStreamPtr++ ] );

    if ( m_ByteOrder == BigEndian )
    {
      data = (b0 << 8) | (b1);
    }
    else
    {
      data = (b1 << 8) | (b0);
    }
  }
  return data;
}

/***********************************************************************************
* ReadUInt16:
* Reads two bytes from the databuffer
*
* @return (uint16) The uint16 value that was in the databuffer
*
* @author Thor Richard Hansen
***********************************************************************************/
uint16 BinaryStream::ReadUInt16( uint32 pos ) const
{
  uint16 data = 0;
  if ( VerifyRead( pos + 1 ) )
  {
    uint16 b0   = uint8( m_Data[ pos + 0 ] );
    uint16 b1   = uint8( m_Data[ pos + 1 ] );

    if ( m_ByteOrder == BigEndian )
    {
      data = (b0 << 8) | (b1);
    }
    else
    {
      data = (b1 << 8) | (b0);
    }
  }
  return data;
}

uint32 BinaryStream::PeekUInt32() const
{
  uint32 data = 0;

  if ( VerifyRead( 4 ) )
  {
    uint32 b0 = uint8( m_Data[ m_ReadStreamPtr + 0 ] );
    uint32 b1 = uint8( m_Data[ m_ReadStreamPtr + 1 ] );
    uint32 b2 = uint8( m_Data[ m_ReadStreamPtr + 2 ] );
    uint32 b3 = uint8( m_Data[ m_ReadStreamPtr + 3 ] );

    if ( m_ByteOrder == BigEndian )
    {
      data = (b0 << 24) | (b1 << 16) | (b2 << 8)  | (b3);
    }
    else
    {
      data = (b3 << 24) | (b2 << 16) | (b1 << 8)  | (b0);
    }
  }
  return data;
}

/***********************************************************************************
* ReadUInt32:
* Reads four bytes from the databuffer
*
* @return (uint32) The uint32 value that was in the databuffer
*
* @author Thor Richard Hansen
***********************************************************************************/
uint32 BinaryStream::ReadUInt32() const
{
  uint32 data = 0;

  if ( VerifyRead( 4 ) )
  {
    uint32 b0 = uint8( m_Data[ m_ReadStreamPtr++ ] );
    uint32 b1 = uint8( m_Data[ m_ReadStreamPtr++ ] );
    uint32 b2 = uint8( m_Data[ m_ReadStreamPtr++ ] );
    uint32 b3 = uint8( m_Data[ m_ReadStreamPtr++ ] );
    
    if ( m_ByteOrder == BigEndian )
    {
      data = (b0 << 24) | (b1 << 16) | (b2 << 8)  | (b3);
    }
    else
    {
      data = (b3 << 24) | (b2 << 16) | (b1 << 8)  | (b0);
    }
  }
  return data;
}

uint32 BinaryStream::ReadUInt32(uint32 pos) const
{
  uint32 data = 0;

  if (VerifyRead(pos + 4))
  {
    uint32 b0 = uint8(m_Data[pos + 0]);
    uint32 b1 = uint8(m_Data[pos + 1]);
    uint32 b2 = uint8(m_Data[pos + 2]);
    uint32 b3 = uint8(m_Data[pos + 3]);

    if (m_ByteOrder == BigEndian)
    {
      data = (b0 << 24) | (b1 << 16) | (b2 << 8) | (b3);
    }
    else
    {
      data = (b3 << 24) | (b2 << 16) | (b1 << 8) | (b0);
    }
  }
  return data;
}

/***********************************************************************************
* ReadUInt64:
* Reads eight bytes from the databuffer
*
* @return (uint64) The uint64 value that was in the databuffer
*
* @author Thor Richard Hansen
***********************************************************************************/
uint64 BinaryStream::ReadUInt64() const
{
  uint64 data   = 0;

  if ( VerifyRead( 8 ) )
  {
    uint64 word1 = ReadUInt32();
    uint64 word2 = ReadUInt32();

    if ( m_ByteOrder == BigEndian )
    {
      data = (word1 << 32) + word2;
    }
    else
    {
      data = (word2 << 32) + word1;
    }
  }
  return data;
}

/// ReadString
/// Reads a string from the databuffer
/// @return String [ret] Value of the string
/// @author Thor Richard Hansen
String BinaryStream::ReadString() const
{
  uint16 stringLength = ReadUInt16();

  if ( stringLength > 0 && VerifyRead( stringLength ) )
  {
    char* pStr = new char[ stringLength + 1 ];
    memcpy(pStr, m_Data + m_ReadStreamPtr, stringLength);
    pStr[ stringLength ] = 0;

    String retString = String(pStr);
    delete [] pStr;

    m_ReadStreamPtr += stringLength;
    return retString;
  }
  return String::zero;
}

/// ReadRaw
/// Reads a partial piece of the datablob
/// @return QByteArray [ret] Value of the partial datablob
/// @author Thor Richard Hansen
const char* BinaryStream::ReadRaw( uint32 dataLength ) const
{
  if (dataLength == 0)
  {
    return NULL;
  }

  if ( VerifyRead( dataLength ) )
  {
    char* cpyData = ( char * ) malloc( dataLength + 1 );
    if ( cpyData != NULL )
    {
      memcpy( cpyData, m_Data + m_ReadStreamPtr , dataLength );
      cpyData[ dataLength ] = 0;
    }
    m_ReadStreamPtr += dataLength;
    return cpyData;
  }

  char* cpyData = ( char * ) malloc( 1 );
  if ( cpyData != NULL )
  {
    cpyData[ 0 ] = 0;
  }
  return cpyData;
}

/// ReadRaw
/// Reads a partial piece of the datablob
/// @return QByteArray [ret] Value of the partial datablob
/// @author Thor Richard Hansen
const char* BinaryStream::ReadRaw( uint32 startPos, int dataLength ) const
{
  if ( m_DataSize >= startPos + dataLength )
  {
    char* cpyData = ( char * ) malloc( dataLength + 1 );
    if ( cpyData != NULL )
    {
      memcpy( cpyData, m_Data + startPos , dataLength );
      cpyData[ dataLength ] = 0;
    }
    return cpyData;
  }

  char* cpyData = ( char * ) malloc( 1 );
  if ( cpyData != NULL )
  {
    cpyData[ 0 ] = 0;
  }
  return cpyData;
}

/// ReadIgnore
/// Just moves the read pointer
/// @author Thor Richard Hansen
void BinaryStream::ReadIgnore( uint32 dataLength ) const
{
  if ( VerifyRead( dataLength ) )
  {
    m_ReadStreamPtr += dataLength;
  }
}

/// WriteUInt8
/// Writes one byte to the datastream
/// @param data [in] The data we want to write
/// @author Thor Richard Hansen
void BinaryStream::WriteUInt8( uint8 data )
{  
  if ( AddDataSize(1) )
  {
    m_Data[m_WriteStreamPtr++] = uint8(data);
  }
}

/// WriteBoolean
/// Writes one byte to the datastream
/// @param data [in] The data we want to write
/// @author Thor Richard Hansen
void BinaryStream::WriteBoolean(bool data)
{
  if (AddDataSize(1))
  {
    m_Data[m_WriteStreamPtr++] = (data) ? 1 : 0;
  }
}

/// WriteUInt18
/// Writes two bytes to the datastream
/// @param data [in] The data we want to write
/// @author Thor Richard Hansen
void BinaryStream::WriteUInt16( uint16 data )
{
  AddDataSize(2);

  if ( m_ByteOrder == BigEndian )
  {    
    m_Data[ m_WriteStreamPtr++ ] = uint8((data>>8 )&0xff);
    m_Data[ m_WriteStreamPtr++ ] = uint8((data>>0 )&0xff);
  }
  else
  {
    m_Data[ m_WriteStreamPtr++ ] = uint8((data>>0 )&0xff);
    m_Data[ m_WriteStreamPtr++ ] = uint8((data>>8 )&0xff);
  }
}

/// WriteUInt16
/// Writes two bytes to the datastream at a specific position
/// @param data [in] The data we want to write
/// @author Thor Richard Hansen
void BinaryStream::WriteUInt16( uint32 pos, uint16 data )
{
  if ( pos >= m_DataSize || pos > m_WriteStreamPtr )
  {
    return;
  }

  if ( m_ByteOrder == BigEndian )
  {
    m_Data[ pos + 0 ] = uint8((data>>8 )&0xff);
    m_Data[ pos + 1 ] = uint8((data>>0 )&0xff);
  }
  else
  {
    m_Data[ pos + 0 ] = uint8((data>>0 )&0xff);
    m_Data[ pos + 1 ] = uint8((data>>8 )&0xff);
  }
}

/// WriteUInt32
/// Writes four bytes to the datastream
/// @param data [in] The data we want to write
/// @author Thor Richard Hansen
void BinaryStream::WriteUInt32( uint32 data )
{
  if ( !AddDataSize(4) )
  {
    return;
  }

  if ( m_ByteOrder == BigEndian )
  {
    m_Data[ m_WriteStreamPtr++ ] = uint8((data>>24 )&0xff);
    m_Data[ m_WriteStreamPtr++ ] = uint8((data>>16 )&0xff);
    m_Data[ m_WriteStreamPtr++ ] = uint8((data>>8 )&0xff);
    m_Data[ m_WriteStreamPtr++ ] = uint8((data>>0 )&0xff);
  }
  else
  {
    m_Data[ m_WriteStreamPtr++ ] = uint8((data>>0 )&0xff);
    m_Data[ m_WriteStreamPtr++ ] = uint8((data>>8 )&0xff);
    m_Data[ m_WriteStreamPtr++ ] = uint8((data>>16 )&0xff);
    m_Data[ m_WriteStreamPtr++ ] = uint8((data>>24 )&0xff);
  }
}

void BinaryStream::WriteUInt32( uint32 pos, uint32 data )
{
  if ( pos >= m_DataSize || pos > m_WriteStreamPtr )
  {
    return;
  }

  if ( m_ByteOrder == BigEndian )
  {
    m_Data[ pos     ] = uint8((data>>24 )&0xff);
    m_Data[ pos + 1 ] = uint8((data>>16 )&0xff);
    m_Data[ pos + 2 ] = uint8((data>>8 )&0xff);
    m_Data[ pos + 3 ] = uint8((data>>0 )&0xff);
  }
  else
  {
    m_Data[ pos     ] = uint8((data>>0 )&0xff);
    m_Data[ pos + 1 ] = uint8((data>>8 )&0xff);
    m_Data[ pos + 2 ] = uint8((data>>16 )&0xff);
    m_Data[ pos + 3 ] = uint8((data>>24 )&0xff);
  }
}

/// WriteUInt64
/// Writes four bytes to the datastream
/// @param data [in] The data we want to write
/// @author Thor Richard Hansen
void BinaryStream::WriteUInt64( uint64 data )
{
  if ( !AddDataSize(8) )
  {
    return;
  }

  if ( m_ByteOrder == BigEndian )
  {
    m_Data[ m_WriteStreamPtr++ ] = uint8((data>>48 )&0xff);
    m_Data[ m_WriteStreamPtr++ ] = uint8((data>>40 )&0xff);
    m_Data[ m_WriteStreamPtr++ ] = uint8((data>>36 )&0xff);
    m_Data[ m_WriteStreamPtr++ ] = uint8((data>>32 )&0xff);
    m_Data[ m_WriteStreamPtr++ ] = uint8((data>>24 )&0xff);
    m_Data[ m_WriteStreamPtr++ ] = uint8((data>>16 )&0xff);
    m_Data[ m_WriteStreamPtr++ ] = uint8((data>>8 )&0xff);
    m_Data[ m_WriteStreamPtr++ ] = uint8((data>>0 )&0xff);
  }
  else
  {
    m_Data[ m_WriteStreamPtr++ ] = uint8((data>>0 )&0xff);
    m_Data[ m_WriteStreamPtr++ ] = uint8((data>>8 )&0xff);
    m_Data[ m_WriteStreamPtr++ ] = uint8((data>>16 )&0xff);
    m_Data[ m_WriteStreamPtr++ ] = uint8((data>>24 )&0xff);
    m_Data[ m_WriteStreamPtr++ ] = uint8((data>>32 )&0xff);
    m_Data[ m_WriteStreamPtr++ ] = uint8((data>>36 )&0xff);
    m_Data[ m_WriteStreamPtr++ ] = uint8((data>>40 )&0xff);
    m_Data[ m_WriteStreamPtr++ ] = uint8((data>>48 )&0xff);
  }
}

/// WriteString
/// Writes a string to the datablob
/// @param data [in] The string we want to write
/// @author Thor Richard Hansen
void BinaryStream::WriteString(const String& data)
{
  uint16 stringLength = (uint16) data.Length();
  WriteUInt16( stringLength );

  if ( stringLength > 0 )
  {
    if ( AddDataSize( stringLength ) )
    {
      memcpy(m_Data + m_WriteStreamPtr, data.c_str(), stringLength);
      m_WriteStreamPtr += stringLength;
    }
  }
}

/// WriteRaw
/// Appends a datablob to the current datablob
/// @param dataBlob [in] The datablob we want to write
/// @param blobLength [in] Length of the data we want to write
/// @author Thor Richard Hansen
void BinaryStream::WriteRaw( const uint8* dataBlob, uint32 blobLength )
{
  if ( blobLength > 0 )
  {
    if ( AddDataSize( blobLength ) )
    {
      memcpy(m_Data + m_WriteStreamPtr, (char*)dataBlob, blobLength);
      m_WriteStreamPtr += blobLength;
    }
  }
}

/// WriteRaw
/// Appends a datablob to the current datablob
/// @param dataBlob [in] The datablob we want to write
/// @param blobLength [in] Length of the data we want to write
/// @author Thor Richard Hansen
void BinaryStream::WriteRaw( const char* dataBlob, uint32 blobLength )
{
  if ( blobLength > 0 )
  {
    if ( AddDataSize( blobLength ) )
    {
      memcpy(m_Data + m_WriteStreamPtr, dataBlob, blobLength);
      m_WriteStreamPtr += blobLength;
    }
  }
}

/// WriteRaw
/// Appends a datablob to the current datablob
/// @param dataBlob [in] The datablob we want to write
/// @param blobLength [in] Length of the data we want to write
/// @author Thor Richard Hansen
void BinaryStream::WriteRaw( const BinaryStream& otherStream )
{
  WriteRaw( otherStream.GetData(), otherStream.GetSize() );
}

void BinaryStream::WriteRaw(const BinaryStream* otherStream)
{
  if (otherStream == this)
  {
    dfBugreport(false, "BinaryStream::WriteRaw : Cannot write itself into stream");
    return;
  }
  else if ( otherStream != NULL )
  {
    WriteRaw(otherStream->GetData(), otherStream->GetSize());
  }
}

void BinaryStream::WriteRaw( uint32 position, const char* dataBlob, uint32 blobLength )
{
  // If we have no data, just do a normal write
  if ( m_Data == NULL )
  {
    WriteRaw( dataBlob, blobLength );
    return;
  }

  // Remember the old data
  char* oldData = m_Data;

  // Calculate the new size and alloc it
  m_AllocSize = m_DataSize + blobLength;
  m_Data = (char*) malloc( m_AllocSize );
  if ( m_Data == NULL )
  {
    return;
  }

  // Write old data in the beginning if the new blob is not at the start
  if ( position > 0 )
  {
    memcpy( m_Data, oldData, position );
  }

  // Write the new data
  memcpy ( m_Data + position, dataBlob, blobLength );

  // Write the rest of the old data if we have any left
  if ( position + blobLength < m_DataSize )
  {
    memcpy( m_Data + position + blobLength, oldData, m_DataSize - position );
  }

  // Update the datasize
  m_DataSize = m_AllocSize;

  // Delete the old block
  free(oldData);
  oldData = NULL;
}


/// AtEnd
/// Returns true if we have read to the end of the blob
/// @return boolean [in] True if we are at the end
/// @author Thor Richard Hansen
bool BinaryStream::AtEnd()
{
  return ( m_ReadStreamPtr >= m_DataSize );
}

/// AddDataSize
/// Makes sure we have enough space in the datablob
/// to write to. If not, allocate more space.
/// @param size [in] Size of the data we want to add
/// @author Thor Richard Hansen
bool BinaryStream::AddDataSize( int addSize )
{
  m_DataSize = m_WriteStreamPtr + addSize;
  uint32 computedSize = m_AllocSize;

  // Initialize the datablob with 128 bytes
  if ( computedSize == 0 )
  {
    computedSize = 128;
  }

  //double the size for each allocation
  while( m_DataSize > computedSize && computedSize > 0 )
  {
    computedSize *= 2;

    // Make sure  we do not exceed the max cap
    if (computedSize > MAX_DATA_SIZE)
    {
      dfBugreportFMT(false, "BinaryStream::AddDataSize : addSize is too large ( %u > %u ) wamted[%u] : Max = %u", m_AllocSize, computedSize, addSize, MAX_DATA_SIZE);
      return false;
    }
  }

  // Make sure  we do not exceed the max cap
  if ( computedSize > MAX_DATA_SIZE )
  {
    dfBugreportFMT( false, "BinaryStream::AddDataSize : addSize is too large ( %d > %d ) : Max = %d", m_AllocSize, computedSize, MAX_DATA_SIZE );
    return false;
  }

  // Check if we have to allocate more
  if ( computedSize != m_AllocSize )
  {
    char* newBlob = (char*) malloc( computedSize );
    if ( newBlob == NULL )
    {
      return false;
    }
    memset( newBlob, 0, computedSize );

    if ( m_AllocSize != 0 )
    {
      memcpy( newBlob, m_Data, m_AllocSize );    
      free( m_Data );
    }
    m_AllocSize = computedSize;
    m_Data = newBlob;
  }
  return true;
}

/// Reset
/// Resets the read/write headers
/// @author Thor Richard Hansen
void BinaryStream::Reset()
{
  m_WriteStreamPtr = 0;
  m_ReadStreamPtr  = 0;
  m_DataSize       = 0;
}

/************************************************************************************************
* SetReadPos:
* Sets the read pointer. IF we try to set the read pointer outside of the binaryblob, the blob
* is set as failed.
*
* @param  (uint32)     pos - The new read position
*
* @author Thor Richard Hansen
*************************************************************************************************/
bool BinaryStream::SetReadPos( uint32 pos ) const
{
  if ( pos <= m_DataSize )
  {
    m_ReadStreamPtr = pos;
    return true;
  }
  else
  {
    dfBugreportFMT(false, "BinaryStream::SetReadPos : Readpos (%d) is outside of memorybounds (%d)", pos, m_DataSize);

    m_IsOK = false;
    return false;
  }
}

bool BinaryStream::SetWritePos(uint32 pos)
{
  if (pos <= m_DataSize)
  {
    m_WriteStreamPtr = pos;
    return true;
  }
  else
  {
    m_IsOK = false;
    return false;
  }
}

/// GetData
/// Returns the datablob
/// @param QByteArray [ret] The datablob
/// @author Thor Richard Hansen
const char* BinaryStream::GetData() const
{
//  QByteArray data( m_Data, m_DataSize );
//  return data;
  return m_Data;
}

/// GetSize
/// Returns the size of the datablob
/// @param uint32 [ret] The size of the datablob
/// @author Thor Richard Hansen
uint32 BinaryStream::GetSize() const
{
  return m_DataSize;
}

/// GetReadDataLeft
/// Returns how much we can read before going out of bounds
/// @param uint32 [ret] The size of the readbuffer left
/// @author Thor Richard Hansen
uint32 BinaryStream::GetReadDataLeft() const
{
  int dataLeft = m_DataSize - m_ReadStreamPtr;
  if ( dataLeft > 0 )
  {
    return dataLeft;
  }
  return 0;
}
