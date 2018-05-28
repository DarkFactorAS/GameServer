#ifndef ChatLib_Utils_BinaryStream_H
#define ChatLib_Utils_BinaryStream_H

#include "../Definitions/BasicTypes.h"
#include "../String/CoreString.h"

class BinaryStream
{
public:

  enum ByteOrder_e
  { 
    BigEndian = 0,
    LittleEndian
  };

  BinaryStream();
  BinaryStream( ByteOrder_e byteOrder );
  BinaryStream( const char* dataBlob, uint32 blobLength, ByteOrder_e byteOrder = BigEndian );
  BinaryStream( const uint8* dataBlob, uint32 blobLength, ByteOrder_e byteOrder = BigEndian );
  BinaryStream( const BinaryStream* otherStream );
  BinaryStream( const BinaryStream* otherStream, uint32 blobLength );
  ~BinaryStream();

  static const BinaryStream s_Empty;
  static const BinaryStream zero;

  // The max size we are allowed to allocate for a binary blob
  static const uint32       MAX_DATA_SIZE;
  BinaryStream operator = ( char* dataBlob )
  {
    m_ReadStreamPtr   = 0;
    m_WriteStreamPtr  = 0;
    m_DataSize        = 0;
    m_IsOK            = false;
    m_ByteOrder       = BigEndian;
    m_Data            = dataBlob;
	return *this;
  }

  void SetSize( uint32 size )
  {
    m_DataSize = m_AllocSize = size;
  }

  //////////////////////////////////////////////////////////////////////////
  ///
  /// Read datastream functions
  ///
  //////////////////////////////////////////////////////////////////////////
  uint8       ReadUInt8() const;
  uint16      ReadUInt16() const;
  uint16      ReadUInt16( uint32 pos ) const;
  uint32      ReadUInt32() const;
  uint32      ReadUInt32(uint32 pos) const;
  uint32      PeekUInt32() const;
  uint64      ReadUInt64() const;
  bool        ReadBoolean() const;
  String      ReadString() const;
  const char* ReadRaw( uint32 dataLength ) const;
  const char* ReadRaw( uint32 startPos, int dataLength ) const;
  void        ReadIgnore( uint32 ignoredLength ) const;

  //////////////////////////////////////////////////////////////////////////
  ///
  /// Write datastream functions
  ///
  //////////////////////////////////////////////////////////////////////////
  void    WriteUInt8(uint8 data);
  void    WriteBoolean(bool data);
  void    WriteUInt16(uint16 data);
  void    WriteUInt16(uint32 pos, uint16 data);
  void    WriteUInt32(uint32 data);
  void    WriteUInt32(uint32 pos, uint32 data);
  void    WriteUInt64(uint64 data);
  void    WriteString(const String& data);
  void    WriteRaw(const uint8* dataBlob, uint32 blobLength );
  void    WriteRaw(const char* dataBlob, uint32 blobLength );
  void    WriteRaw(const BinaryStream& otherStream );
  void    WriteRaw(const BinaryStream* otherStream);
  void    WriteRaw(uint32 position, const char* dataBlob, uint32 blobLength);
  bool    AtEnd();

  //////////////////////////////////////////////////////////////////////////
  ///
  /// Util methods
  ///
  //////////////////////////////////////////////////////////////////////////

  void        Reset();
  uint32      GetWritePos() const { return m_WriteStreamPtr; }
  uint32      GetReadPos() const { return m_ReadStreamPtr; }
  bool        SetWritePos( uint32 pos );
  bool        SetReadPos( uint32 pos ) const;
//  QByteArray  GetData();
  const char* GetData() const;
  uint32      GetSize() const;
  uint32      GetReadDataLeft() const;
  bool        IsOK() const { return m_IsOK; }
  void        SetByteOrder( ByteOrder_e byteOrder ) { m_ByteOrder = byteOrder; }
  ByteOrder_e GetByteOrder() const { return m_ByteOrder; }

private:

  /// Pointer to where we are reading in the stream
  mutable uint32  m_ReadStreamPtr;
  /// Pointer to where we are writing in the stream 
  uint32  m_WriteStreamPtr;
  /// Datasize
  uint32  m_DataSize;
  /// Allocated Size
  uint32  m_AllocSize;
  /// Flag if the stream is corrupt
  mutable bool    m_IsOK;
  /// Byteorder ( BigEndian is default )
  ByteOrder_e m_ByteOrder;
  /// Datablob
  char*   m_Data;

  bool    AddDataSize( int addSize );
//  bool    VerifyRead( int checkSize );
  bool    VerifyRead( int checkSize ) const;
  bool    VerifyRead( uint32 offset, int checkSize ) const;
};

#endif // ChatLib_Utils_BinaryStream_H
