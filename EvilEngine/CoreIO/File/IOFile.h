#ifndef EVILENGINE_COREIO_FILEREADER_H
#define EVILENGINE_COREIO_FILEREADER_H

#include <fstream>
#include "../../CoreLib/BasicTypes/String/CoreString.h"

class IOFile
{
public:
  IOFile();
  IOFile( const String& fileName );
  ~IOFile();

  bool    OpenRead( const String& fileName );
  bool    OpenRead();
  bool    OpenWrite( const String& fileName, bool truncateFile = true );
  bool    OpenWrite( bool truncateFile = true);
  void    Close();

  uint32  FileLength();
  bool    IsEOF();
  bool    IsOpen();
  static bool    Exists(const String& fullFileName);
  void    Flush();
  uint32  FileSize();

  static bool     DeleteFileWithName(const String& fullFileName);

  String  ReadLine();
  String  ReadAllAsString();
  uint32  ReadRaw( char* outData, uint32 dataLength );
  void    WriteString( const String& text );
  void    WriteRaw( const char* data, uint32 dataLength );

private:
  std::fstream    m_File;
  String          m_FileName;
};

#endif /// EVILENGINE_COREIO_FILEREADER_H