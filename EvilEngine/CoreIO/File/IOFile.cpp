
#include "Precompile.h"
#include "IOFile.h"

#ifdef _WIN32
  #include <io.h> 
  #define access    _access_s
#else
  #include <unistd.h>
#endif

IOFile::IOFile()
{
}

IOFile::IOFile( const String& fileName ) :
  m_FileName( fileName )
{
}

IOFile::~IOFile()
{
  Close();
}

bool IOFile::OpenRead( const String& fileName )
{
  if ( !fileName.IsEmpty() )
  {
    m_File.open( fileName.c_str(), std::ios_base::in );  
    return IsOpen();
  }
  return false;
}

bool IOFile::OpenRead()
{
  return OpenRead( m_FileName );
}

bool IOFile::OpenWrite( const String& fileName, bool truncateFile )
{
  if ( truncateFile )
  {
    m_File.open( fileName.c_str(), std::ios_base::out | std::ios_base::trunc | std::ios_base::binary );  
  }
  else
  {
    m_File.open( fileName.c_str(), std::ios_base::out | std::ios_base::app | std::ios_base::binary );
  }
  return IsOpen();
}

bool IOFile::OpenWrite(bool truncateFile)
{
  return OpenWrite( m_FileName, truncateFile );
}

void IOFile::Close()
{
  if ( IsOpen() )
  {
    m_File.close();
  }
}

bool IOFile::IsEOF()
{
  return m_File.eof();
}

bool IOFile::IsOpen()
{
  return m_File.is_open();
}

bool IOFile::Exists(const String& fullFileName)
{
  return access(fullFileName.c_str(), 0) == 0;
}

bool IOFile::DeleteFileWithName(const String& fullFileName)
{
  int ret = remove( fullFileName.c_str() );
  return (ret == 0 );
}

void IOFile::Flush()
{
  m_File.flush();
}

String IOFile::ReadLine()
{
  if ( !IsOpen() || IsEOF() )
  {
    return String::zero;
  }

  std::string line;
  if ( std::getline( m_File, line ) )
  {
    return line;
  }
  return String::zero;
}

void IOFile::WriteString( const String& text )
{
  m_File.write( text.c_str(), text.Length() );
}

void IOFile::WriteRaw( const char* data, uint32 dataLength )
{
  if (data != NULL && dataLength > 0)
  {
    m_File.write(data, dataLength);
  }
}

uint32 IOFile::FileSize()
{
  int curr = (int)m_File.tellg();
  m_File.seekg ( 0, m_File.end);
  int length = (int)m_File.tellg();
  m_File.seekg ( curr, m_File.beg );
  return length;
}

uint32 IOFile::FileLength()
{
  if ( !IsOpen() )
  {
    uint32 length = 0;
    if ( OpenRead() )
    {
      uint32 curr = (uint32)m_File.tellg();
      m_File.seekg ( 0, m_File.end);
      length = (uint32)m_File.tellg();
      m_File.seekg(curr, m_File.beg);
    }
    Close();
    return length;
  }
  else
  {
    uint32 curr = (uint32)m_File.tellg();
    m_File.seekg ( 0, m_File.end);
    uint32 length = (uint32)m_File.tellg();
    m_File.seekg ( curr, m_File.beg );
    return length;
  }
}

String IOFile::ReadAllAsString()
{
  uint32 fileLength = FileLength();
  char* blob = new char[ fileLength + 1 ];
  ReadRaw( blob, fileLength );
  blob[ fileLength ] = 0;
  return String( blob );
}

uint32 IOFile::ReadRaw( char* outData, uint32 dataLength )
{
  if (!IsOpen() || IsEOF())
  {
    return 0;
  }

  uint32 curr = (uint32)m_File.tellg();
  m_File.seekg ( 0, m_File.end);
  uint32 length = (uint32)m_File.tellg() - curr;
  m_File.seekg ( curr, m_File.beg );

  if ( dataLength > length )
  {
    dataLength = length - curr;
  }

  if ( dataLength > 0 )
  {
    m_File.read( outData, dataLength );
    return dataLength;
  }

  return 0;
}
