
#include "Precompile.h"
#include "BugReport.h"
#include "EvilEngine/CoreLib/Utils/CoreUtils.h"

#define FORMAT_MIN_VERSION 3
#define FORMAT_CURRENT_VERSION 5
#define MAX_FILESIZE 10240000

#define MAX_TITLE_SIZE 256
#define MAX_EMAIL_SIZE 256
#define MAX_CALLSTACK_SIZE 3000

#include "boost/date_time/posix_time/posix_time_types.hpp"

uint32 BugReport::s_LastBugId = 0;

BugReport::BugReport() :
  m_ClientId( 0 ),
  m_ServerId( 0 ),
  m_IsSaved( false ),
  m_IsSending( false ),
  m_BugVersion(FORMAT_CURRENT_VERSION)
{
}

BugReport::BugReport( const String& title, const String& message, const String& email ) :
  m_ClientId( 0 ),
  m_ServerId( 0 ),
  m_Title( title ),
  m_Message( message ),
  m_Email( email ),
  m_IsSaved( false ),
  m_IsSending( false ),
  m_BugVersion(FORMAT_CURRENT_VERSION)
{
  boost::posix_time::ptime localTime = boost::posix_time::microsec_clock::universal_time();
  uint32 seconds     =localTime.time_of_day().seconds();
  uint32 minutes     =localTime.time_of_day().minutes();
  uint32 hours       =localTime.time_of_day().hours();
  uint32 day         =localTime.date().day();
  uint32 month       =localTime.date().month();
  uint32 year        =localTime.date().year();
  m_CreatedDate      = String::FormatString( "%u-%02u-%02u %02u:%02u:%02u", year,month,day, hours,minutes,seconds );

  // Make sure we cap the callstack
  if ( m_Title.Length() > MAX_TITLE_SIZE )
  {
    m_Title = title.SubString( 0, MAX_TITLE_SIZE );
  }
  if ( m_Email.Length() > MAX_EMAIL_SIZE )
  {
    m_Email = email.SubString( 0, MAX_EMAIL_SIZE );
  }
  if ( message.Length() > MAX_CALLSTACK_SIZE )
  {
    m_Message = "Moved to callstack.txt file";
    AddFileData("callstack.txt", message.c_str(), message.length() );
  }
}

BugReport::~BugReport()
{
  for (uint32 index = 0; index < m_Files.size(); index++)
  {
    BugReportFile* file = m_Files[index];
    delete file;
  }
  m_Files.clear();
}


uint32 BugReport::GetFormatVersion()
{
  return FORMAT_CURRENT_VERSION;
}

void BugReport::AddFileData( const String& fileName, const char* data, uint32 dataLength )
{
  m_Files.push_back( new BugReportFile(fileName,0,data,dataLength, dataLength) );
}

void BugReport::AddFile(BugReportFile* file)
{
  m_Files.push_back(file);
}

bool BugReport::AddFile( const String& fileName )
{
  IOFile* file = new IOFile( fileName );
  if ( file->OpenRead() )
  {
    uint32 fileSize = file->FileLength();
    if ( fileSize < MAX_FILESIZE )
    {
      char* data = (char*) malloc( fileSize );
      fileSize = file->ReadRaw( data, fileSize );
      AddFileData( fileName, data, fileSize );
    }
    file->Close();
    return true;
  }
  else
  {
    LogWarningFMT("BugReport","Could not add file '%s' (not found ) to bugreport '%s' ", fileName.c_str(), m_Title.c_str() );
    file->Close();
    return false;
  }
}

void BugReport::DeleteFileWithId(uint32 fileId)
{
  for (std::vector< BugReportFile* >::iterator itFile = m_Files.begin(); itFile != m_Files.end(); ++itFile )
  {
    const BugReportFile* file = *itFile;
    if (file->GetFileId() == fileId)
    {
      //delete file;
      m_Files.erase(itFile);
      // TODO : Delete the actual file
      break;
    }
  }
}


uint32 BugReport::GetTotalFileSize() const
{
  uint32 fileSize = 0;
  for ( uint32 index = 0; index < m_Files.size(); index++ )
  {
    const BugReportFile* file = m_Files[ index ];
    fileSize += file->GetFileLength();
  }
  return fileSize;
}

const BugReportFile* BugReport::GetFileAt( uint32 position ) const
{
  if ( position < (uint32) m_Files.size() )
  {
    return m_Files[ position ];
  }
  return NULL;
}

BugReport* BugReport::CreateFromData( const BinaryStream* data )
{
  uint16 formatVersion = data->ReadUInt16(); 
  BugReport* bugReport = NULL;
  switch (formatVersion)
  {
    case 3:
      {
        bugReport                   = new BugReport();
        bugReport->SetBugVersion(3);
        bugReport->m_ClientName     = data->ReadString();
        bugReport->m_ClientVersion  = data->ReadString();
        bugReport->m_ClientId       = data->ReadUInt32();
        bugReport->m_ServerId       = data->ReadUInt32();
        bugReport->m_Title          = data->ReadString();
        bugReport->m_Email          = data->ReadString();
        bugReport->m_Message        = data->ReadString();
        bugReport->m_CreatedDate    = data->ReadString();
                                      data->ReadUInt32(); // Num Files. Not in use
      }
      break;

    case 4:
      {
        bugReport = new BugReport();
        bugReport->SetBugVersion(4);
        bugReport->m_ClientName     = data->ReadString();
        bugReport->m_ClientVersion  = data->ReadString();
        bugReport->m_ClientId       = data->ReadUInt32();
        bugReport->m_ServerId       = data->ReadUInt32();
        bugReport->m_Title          = data->ReadString();
        bugReport->m_Email          = data->ReadString();
        bugReport->m_Message        = data->ReadString();
        bugReport->m_CreatedDate    = data->ReadString();
      }
      break;

    case 5:
      {
        bugReport = new BugReport();
        bugReport->SetBugVersion(5);
        bugReport->m_ServerId       = data->ReadUInt32();
        bugReport->m_ClientId       = data->ReadUInt32();
        bugReport->m_ClientName     = data->ReadString();
        bugReport->m_ClientVersion  = data->ReadString();
        bugReport->m_Title          = data->ReadString();
        bugReport->m_Email          = data->ReadString();
        bugReport->m_Message        = data->ReadString();
        bugReport->m_CreatedDate    = data->ReadString();
      }
      break;
  }

  if (bugReport != NULL && data->IsOK())
  {
    return bugReport;
  }

  delete bugReport;
  LogErrorFMT("BugReport", "Could not create bugreport from binarystream data. Wrong version ( minimum:%d - data:%d )", FORMAT_MIN_VERSION, formatVersion);
  return NULL;  
}

void BugReport::WriteData( BinaryStream* data ) const
{
  data->WriteUInt16( FORMAT_CURRENT_VERSION );
  data->WriteUInt32(m_ServerId);

  data->WriteUInt32(m_ClientId);
  data->WriteString(m_ClientName);
  data->WriteString(m_ClientVersion);
  data->WriteString(m_Title);
  data->WriteString(m_Email);
  data->WriteString(m_Message);
  data->WriteString(m_CreatedDate);
}

void BugReport::WriteFileData(const BugReportFile* brFile, BinaryStream* data) const
{
  data->WriteUInt16( FORMAT_CURRENT_VERSION );
  data->WriteUInt32(m_ServerId);

  data->WriteUInt32(brFile->GetFileId());
  data->WriteUInt32(brFile->GetFileLength());
  data->WriteString(brFile->GetFileName());
  data->WriteRaw(brFile->GetData(), brFile->GetFileLength());
}

BugReport* BugReport::CreateFromData( VariantTree* data )
{
  if ( data != NULL )
  {
    uint32 formatVersion = data->GetUInt32("formatversion");
    switch (formatVersion)
    {
      case 3:
      case 4:
      case 5:
        {
            BugReport* bugReport = new BugReport();
            bugReport->SetClientId( data->GetUInt32("clientid") );
            bugReport->SetEmail( data->GetString("email" ) );
            bugReport->SetTitle( data->GetString("title") );
            bugReport->SetMessage( data->GetString("message") );
            bugReport->SetClientName( data->GetString("clientname") );
            bugReport->SetClientVersion( data->GetString("version") );
            bugReport->SetClientVersion( data->GetString("clientversion", bugReport->GetClientVersion() ) );
            bugReport->SetCreatedDate( data->GetString("createddate") );
            bugReport->SetSaved( true );

            // If we do not have ID in the data, try to fetch if from the node name
            if (bugReport->GetClientId() == 0)
            {
              bugReport->SetClientId(data->GetName().ToUInt32());
            }

            return bugReport;
        }
        break;
    }
    LogWarningFMT("BugReport","Could not create from data (VariantTree). Format version %d is not supported (required: %d to %d) (Node:%s)", 
      formatVersion, FORMAT_MIN_VERSION, FORMAT_CURRENT_VERSION, data->GetName().c_str() );
  }
  return NULL;
}

uint32 BugReport::GenerateClientId()
{
  if (s_LastBugId == 0)
  {
    s_LastBugId = (uint32)CoreUtils::GetTime();
  }
  else
  {
    s_LastBugId++;
  }
  return s_LastBugId;
}

void BugReport::SetMaxClientId(uint32 clientId)
{
  if (clientId > s_LastBugId)
  {
    s_LastBugId = clientId;
  }
}

VariantTree* BugReport::CreateVariantTree()
{
  GenerateClientId();

  VariantTree* tree = new VariantTree();
  tree->SetUInt32("formatversion", FORMAT_CURRENT_VERSION);
  tree->SetString("clientid", GetClientId());
  tree->SetString("title", GetTitle());
  tree->SetString("email", GetEmail());
  tree->SetString("message", GetMessageDesc());
  tree->SetString("clientname", GetClientName());
  tree->SetString("clientversion", GetClientVersion());
  tree->SetString("createddate", GetCreatedDate());
  return tree;
}

BugReport* BugReport::ReadFileDataV3(const BinaryStream* data)
{
    BugReport* bugReport        = new BugReport();
    bugReport->m_BugVersion     = 3;
    bugReport->m_ClientVersion  = data->ReadString();
    bugReport->m_ClientId       = data->ReadUInt32();
    bugReport->m_ServerId       = data->ReadUInt32();
    uint32 numFiles             = data->ReadUInt32();
    uint32 numFilesAdded        = 0;

    for (uint32 index = 0; index < numFiles; index++)
    {
        uint32 fileLength = data->ReadUInt32();
        String fileName = data->ReadString();

        if (fileLength >= MAX_FILESIZE)
        {
            LogWarningFMT("BugReport", "File '%s' is larger (%d) than max allowed filesize (%d)", fileName.c_str(), fileLength, MAX_FILESIZE);
            data->SetReadPos( data->GetReadPos() + fileLength );

            // TODO : Send message to client here ?
            continue;
        }

        numFilesAdded++;
        const char* fileData = data->ReadRaw(fileLength);
        bugReport->AddFileData(fileName, fileData, fileLength);
    }

    if (numFilesAdded == 0)
    {
        delete bugReport;
        bugReport = NULL;
    }

    return bugReport;
}

BugReport* BugReport::ReadFileDataV4(const BinaryStream* data)
{
  BugReport* bugReport        = new BugReport();
  bugReport->m_BugVersion     = 4;
  bugReport->m_ClientName     = data->ReadString();
  bugReport->m_ClientVersion  = data->ReadString();
  bugReport->m_ClientId       = data->ReadUInt32();
  bugReport->m_ServerId       = data->ReadUInt32();
  uint32 numFiles             = data->ReadUInt32();
  uint32 numFilesAdded        = 0;

  for (uint32 index = 0; index < numFiles; index++)
  {
    uint32 fileLength = data->ReadUInt32();
    String fileName = data->ReadString();

    if (fileLength >= MAX_FILESIZE)
    {
      LogWarningFMT("BugReport", "File '%s' is larger (%d) than max allowed filesize (%d)", fileName.c_str(), fileLength, MAX_FILESIZE);
      data->SetReadPos(data->GetReadPos() + fileLength);

      // TODO : Send message to client here ?
      continue;
    }

    numFilesAdded++;
    const char* fileData = data->ReadRaw(fileLength);
    bugReport->AddFileData(fileName, fileData, fileLength);
  }

  if (numFilesAdded == 0)
  {
    delete bugReport;
    bugReport = NULL;
  }

  return bugReport;
}

BugReport* BugReport::ReadFileDataV5(const BinaryStream* data)
{
  BugReport* bugReport    = new BugReport();
  bugReport->m_BugVersion = 5;
  bugReport->m_ServerId   = data->ReadUInt32();
  uint32 fileId           = data->ReadUInt32();
  uint32 fileLength       = data->ReadUInt32();
  String fileName         = data->ReadString();

  if (fileLength >= MAX_FILESIZE)
  {
    LogWarningFMT("BugReport", "File '%s' is larger (%d) than max allowed filesize (%d)", fileName.c_str(), fileLength, MAX_FILESIZE);
    data->SetReadPos(data->GetReadPos() + fileLength);

    // TODO : Send error to client
    return NULL;
  }

  const char* fileData = data->ReadRaw(fileLength);
  bugReport->AddFile(new BugReportFile(fileName, fileId, fileData, fileLength, fileLength));

  return bugReport;
}

BugReport* BugReport::ReadFileData( const BinaryStream* data )
{
  uint16 formatVersion = data->ReadUInt16(); 
  switch (formatVersion)
  {
      case 3:
        return ReadFileDataV3(data);
      case 4:
        return ReadFileDataV4(data);
      case 5:
        return ReadFileDataV5(data);
      default:
        break;
  }

  LogErrorFMT("BugReport", "Could not create bugreport files from binarystream data. Wrong version %d ( required: %d - %d )", formatVersion,  FORMAT_MIN_VERSION, FORMAT_CURRENT_VERSION );
  return NULL;  
}
