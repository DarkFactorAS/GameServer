#include "Precompile.h"

//#ifdef SERVER

#include "BugReportServer.h"

#include "EvilEngine/CoreLib/BasicTypes/Definitions/VolatileArgs.h"
#include "EvilEngine/CoreLib/Utils/CoreUtils.h"
#include "EvilEngine/CoreLib/Md5/md5.h"
#include "EvilEngine/CoreLib/BasicTypes/String/StringList.h"
#include "EvilEngine/CoreIO/File/IOFile.h"
#include "EvilEngine/CoreDatabase/CoreDatabase.h"
#include "EvilEngine/CoreNetwork/Server/NetworkServer.h"
#include "EvilEngine/CoreNetwork/Common/NetworkConnection.h"

#include "EvilEngine/CoreBugReport/Common/BugReportNetworkPacketType.h"
#include "EvilEngine/CoreBugReport/Common/BugReport.h"

BugReportServer::BugReportServer( const String& appName, uint32 appVersion, uint16 serverPort, const String& datafolder ) :
  m_AppName(appName),
  m_AppVersion(appVersion),
  m_Datafolder(datafolder)
{
  String sAppVersion = String::FromInt( appVersion );
  String threadName = String::FormatString("%s-BugServerNetwork", appName.c_str());

  m_NetworkServer = new NetworkServer(threadName, serverPort, true);
  SetNetworkBase( m_NetworkServer );
  //m_NetworkServer->SignalReceivedBugReport.Connect(this, &BugReportServerEngine::SlotReceivedBugReport);
}

BugReportServer::~BugReportServer()
{
}

void BugReportServer::SetNetworkBase(NetworkBase* networkBase)
{
  m_NetworkServer = static_cast<NetworkServer*> (networkBase);
  CoreNetworkEngine::SetNetworkBase(networkBase);
}

bool BugReportServer::InitServer()
{
  String touchedFile = String::FormatString("%s/init.touched", m_Datafolder.c_str() );
  IOFile* fileStream = new IOFile( touchedFile );
  if ( !fileStream->OpenWrite() )
  {
    //fileStream->WriteString("SUCCESS");
    return false;
  }
  fileStream->Close();

  if (m_NetworkServer != NULL)
  {
    return m_NetworkServer->Start();
  }
  return true;
}

bool BugReportServer::Run()
{
  if (m_NetworkServer != NULL)
  {
    m_NetworkServer->Run();
  }
  return CoreNetworkEngine::Run();
}

void BugReportServer::OnReceivedNetworkPacket(uint32 connectionInstance, const BinaryStream* dataStream)
{
  uint32 packetType = dataStream->ReadUInt32();
  LogInfoFMT("BugReportServer", "Received network data on connection :%d (packet:%u)", connectionInstance, packetType);

  NetworkConnection* connection = m_NetworkServer->GetConnection( connectionInstance );
  if ( connection == NULL )
  {
    LogWarningFMT("BugReportServer","Cannot receive packettype[%d] from client (connection[%d] unknown)", packetType, connectionInstance );
    return;
  }

  switch (packetType)
  {
    case BugReportNetworkPacketType::PacketType_BugReport:
      HandleBugReport(connection, dataStream);
      break;
    case BugReportNetworkPacketType::PacketType_BugReportFile:
      HandleFile(connection, dataStream);
      break;
    case BugReportNetworkPacketType::PacketType_BugReportStatusFile:
      HandleStatusFile(connection, dataStream);
      break;
    case BugReportNetworkPacketType::PacketType_BugReportPartialFile:
      HandlePartialFile(connection, dataStream);
      break;
    default:
      LogFatalFMT("BugReportServer", "[NetID:%d] Unknown packet ( %d )", connection->GetConnectionId(), packetType);
      break;
  }
}

//String BugReportServer::WriteBinarystreamToDisk( uint32 bugReportId, const BinaryStream& rawfile )
//{
//  uint32 dataLength       = rawfile.ReadUInt32();
//  String originalFilename = rawfile.ReadString();
//  const char*  rawData    = rawfile.ReadRaw(dataLength);
//
//  // Find correct extension
//  StringList strList = StringList::Split( originalFilename, "." );
//  String extension        = ( strList.size() > 1 ) ? "." + strList.at( strList.size() - 1 ) : String::zero;
//
//  // Create a unique MD5 hash for the filename
//  uint32 maxHexUsage = (dataLength < 1000) ? dataLength : 1000;
//  std::string md5FileName = md5(rawData, maxHexUsage);
//  String filename = String::FormatString("%04d-%s%s", bugReportId, md5FileName.c_str(), extension.c_str() );
//  String physicalFilename = String::FormatString("%s/%s", m_Datafolder.c_str(), filename.c_str() );
//
//  // Make sure the entries are unique.
//  IOFile* fileStream = new IOFile( physicalFilename );
//  CoreDatabase* database = CoreDatabase::GetInstance();
//  if (fileStream->OpenWrite() && database != NULL)
//  {
//    bool didInsert = database->ExecuteInsert("insert into files (bugreportid, originalname, filename,filesize) values( %d,'%s', '%s', %d)",
//      bugReportId,
//      originalFilename.c_str(),
//      filename.c_str(),
//      dataLength);
//    if (didInsert)
//    {
//      fileStream->WriteRaw(rawData, dataLength);
//      fileStream->Close();
//      return filename;
//    }
//    else
//    {
//      fileStream->Close();
//    }
//  }
//  return String::zero;
//}

bool BugReportServer::InsertInDatabase(BugReport* report)
{
#ifdef SERVER
  CoreDatabase* database = CoreDatabase::GetInstance();
  if (database != NULL)
  {
    boost::posix_time::ptime localTime = boost::posix_time::microsec_clock::universal_time();
    uint32 seconds = localTime.time_of_day().seconds();
    uint32 minutes = localTime.time_of_day().minutes();
    uint32 hours = localTime.time_of_day().hours();
    uint32 day = localTime.date().day();
    uint32 month = localTime.date().month();
    uint32 year = localTime.date().year();
    String receivedDate = String::FormatString("%u-%02u-%02u %02u:%02u:%02u", year, month, day, hours, minutes, seconds);

    // Check for duplicates
    const SQLResultSet& result = database->ExecuteSelect("select Id from bugreport where clientname='%s' and clientversion='%s' and title='%s' and text='%s'",
      report->GetClientName().c_str(),
      report->GetClientVersion().c_str(),
      CoreDatabase::SQLFormat(report->GetTitle(), 255),
      CoreDatabase::SQLFormat(report->GetMessageDesc(), 4096)
    );
    if (result.Next())
    {
      uint32 serverId = result.GetUInt32();
      report->SetServerId(serverId);
      int rows = database->ExecuteUpdate("update bugreport set LastReceived='%s', count=count+1 where id=%d ", receivedDate.c_str(), serverId);
      if (rows > 0)
      {
        return true;
      }
    }
    else
    {
      // Generate MD5 of title and text. Used to identify the bug
      String uniqueKey = String::FormatString("%s-%s", report->GetTitle().c_str(), report->GetMessageDesc().c_str() );
      uint32 maxLenght = ( uniqueKey.length() ) < 1000 ? uniqueKey.length() : 1000;
      std::string md5key = md5( uniqueKey.c_str(), maxLenght );

      String createdDate = report->GetCreatedDate();
      if ( createdDate.empty() )
      {
        createdDate = receivedDate;
      }

      bool didInsert = database->ExecuteInsert("insert into bugreport (md5,email,title,text,clientname,clientversion,created,received, LastReceived, Count) values('%s','%s','%s','%s','%s','%s','%s','%s','%s',1)",
        CoreDatabase::SQLTrunk(md5key, 40),
        CoreDatabase::SQLFormat(report->GetEmail(), 255),
        CoreDatabase::SQLFormat(report->GetTitle(), 255),
        CoreDatabase::SQLFormat(report->GetMessageDesc(), 4096),
        report->GetClientName().c_str(),
        report->GetClientVersion().c_str(),
        createdDate.c_str(),
        receivedDate.c_str(),
        receivedDate.c_str()
      );
      if (!didInsert)
      {
        return false;
      }

      // Get the ID back
      const SQLResultSet& resultLastId = database->ExecuteSelect("SELECT LAST_INSERT_ID()");
      if (resultLastId.Next())
      {
        report->SetServerId(resultLastId.GetUInt32());
        return true;
      }
    }
  }
#else
  VOLATILE_ARG(report);
#endif
  return false;
}

BugReportFile* BugReportServer::GetPartialFile( uint32 serverId )
{
#ifdef SERVER
  CoreDatabase* database = CoreDatabase::GetInstance();
  if ( database == NULL )
  {
    return NULL;
  }

  const SQLResultSet& result = database->ExecuteSelect("select WrittenBytes, FileSize, FileId, Filename from files where bugreportid=%d and WrittenBytes < FileSize", serverId );
  if (result.Next())
  {
    uint32 writtenData  = result.GetUInt32();
    uint32 fileSize     = result.GetUInt32();
    uint32 fileId       = result.GetUInt32();
    String fileName     = result.GetString();

    return new BugReportFile(fileName,fileId, NULL, writtenData, fileSize );
  }
#else
  VOLATILE_ARG(serverId);
#endif

  return NULL;
}

/***********************************************************************************
* HandleBugReport:
* Receive the initial packet for a bugreport. Store the bugreport in the database.
*
* @param (NetworkConnection)  connection  - Current network connection
* @param (const BinaryStream*) dataStrean - binary blob containing the data
*
* @author Thor Richard Hansen
***********************************************************************************/
void BugReportServer::HandleBugReport(NetworkConnection* connection, const BinaryStream* dataStream)
{
  BugReport* report = BugReport::CreateFromData(dataStream);
  if (report != NULL)
  {
    if (InsertInDatabase(report))
    {
      String bugHeader = (report->GetTitle().Length() > 20) ? report->GetTitle().SubString(0, 20) : report->GetTitle();

      LogInfoFMT("BugReportServer", "[NetID:%d] Received bugreport[CID:%u => SID:%u] from [ip:%s] '%s'",
        connection->GetConnectionId(),
        report->GetClientId(),
        report->GetServerId(), 
        connection->IpAddress().c_str(), 
        bugHeader.c_str());

      BinaryStream* outStream = m_NetworkServer->CreatePacket(BugReportNetworkPacketType::PacketType_BugReportId);
      switch( report->GetBugVersion() )
      {
        case 3:
        case 4:
          outStream->WriteUInt32(report->GetClientId());
          outStream->WriteUInt32(report->GetServerId());
          break;
        case 5:
          outStream->WriteUInt32(report->GetServerId());
          outStream->WriteUInt32(report->GetClientId());
          break;
      }
      LogDebugFMT("BugReportServer", "SendData [ConnectionId:%d] [Packet:%d] ", connection->GetConnectionId(), BugReportNetworkPacketType::PacketType_BugReportId);
      m_NetworkServer->SendData( connection, outStream );
    }
    else
    {
      String bugHeader = (report->GetTitle().Length() > 20) ? report->GetTitle().SubString(0, 20) : report->GetTitle();
      LogInfoFMT("BugReportServer", "Failed to insert bugreport from '%s' [%s] '%s'", report->GetEmail().c_str(), connection->IpAddress().c_str(), bugHeader.c_str());
    }
    delete report;
  }
}

String BugReportServer::GetFileExtension( const String& fileName )
{
  StringList strList = StringList::Split( fileName, "." );
  if ( strList.size() > 0 )
  {
    return strList.at( strList.size() - 1 );
  }
  return String::zero;
}

/***********************************************************************************
* HandleFile
* Receive and handle files sent from the bugreport
*
* @param (NetworkConnection)  connection  - Current network connection
* @param (const BinaryStream*) dataStrean - binary blob containing the data
*
* @author Thor Richard Hansen
***********************************************************************************/
void BugReportServer::HandleFile(NetworkConnection* connection, const BinaryStream* dataStream)
{
  BugReport* report = BugReport::ReadFileData(dataStream);
  if (report != NULL)
  {
    for (uint32 index = 0; index < report->GetNumFiles(); index++)
    {
      const BugReportFile* file = report->GetFileAt(index);

      // Create a unique MD5 hash for the filename
      std::string md5FileName = md5( file->GetData(), file->GetCurrentLength());
      String fileExtension = GetFileExtension( file->GetFileName() );
      if (!fileExtension.empty())
      {
        fileExtension = "." + fileExtension;
      }

      //String filename = String::FormatString("%04d-%s%s", report->GetServerId(), md5FileName.c_str(), fileExtension.c_str());
      String fullMD5Filename = String::FormatString("%s%s", md5FileName.c_str(), fileExtension.c_str());

      LogInfoFMT("BugReportServer", "[NetID:%d] Received bugreport[SID:%u MD5:%s] file[FID:%u] (%d/%d bytes) from [%s]",
        connection->GetConnectionId(),
        report->GetServerId(), 
        fullMD5Filename.c_str(),
        file->GetFileId(),
        file->GetCurrentLength(),
        file->GetFileLength(), 
        connection->IpAddress().c_str());

#ifdef SERVER

      CoreDatabase* database = CoreDatabase::GetInstance();
      if ( database != NULL )
      {

        // Check if the file is a duplicate
        bool fileExist = false;
        const SQLResultSet& result = database->ExecuteSelect("select bugreportid from files where bugreportid=%d and filename='%s'", report->GetServerId(), fullMD5Filename.c_str());
        if (result.Next())
        {
          fileExist = true;
        }

        // Update database
        if (fileExist)
        {
          database->ExecuteUpdate("update files set count=count+1 where bugreportid=%d and filename='%s'", report->GetServerId(),fullMD5Filename.c_str() );
        }
        else
        {
          // Write file to disk
          String fullFilename = String::FormatString("%s/%s", m_Datafolder.c_str(), fullMD5Filename.c_str());
          IOFile* fileStream = new IOFile(fullFilename);
          if (fileStream->OpenWrite())
          {
            fileStream->WriteRaw( file->GetData(), file->GetCurrentLength() );
            fileStream->Close();
          }

          database->ExecuteInsert("insert into files (bugreportid, originalname, filename, fileid, writtenbytes, filesize, count) values( %d,'%s', '%s', %d, %d, %d, 1)",
            report->GetServerId(),
            file->GetFileName().c_str(),
            fullMD5Filename.c_str(),
            file->GetFileId(),
            file->GetCurrentLength(),
            file->GetFileLength());
        }
      }
#endif

      // Notify client that we received the files
      BinaryStream* outStream = m_NetworkServer->CreatePacket(BugReportNetworkPacketType::PacketType_BugReportFile);
      switch (report->GetBugVersion())
      {
        case 3:
        case 4:
          outStream->WriteUInt32(report->GetClientId());
          outStream->WriteUInt32(report->GetServerId());
          break;
        case 5:
          outStream->WriteUInt32(report->GetServerId());
          outStream->WriteUInt32(file->GetFileId());
          break;
      }
      LogDebugFMT("BugReportServer", "SendData [ConnectionId:%d] [Packet:%d] ", connection->GetConnectionId(), BugReportNetworkPacketType::PacketType_BugReportFile);
      m_NetworkServer->SendData(connection, outStream);
    }

    delete report;
  }
}

/***********************************************************************************
* HandleStatusFile
* Check if the bugreport have any partial written files on disk. ( If the user
* disconnected during file transfer). If this is the case, the client should
* continue to send the rest of the file.
*
* @param (NetworkConnection)  connection  - Current network connection
* @param (const BinaryStream*) dataStrean - binary blob containing the data
*
* @author Thor Richard Hansen
***********************************************************************************/
void BugReportServer::HandleStatusFile(NetworkConnection* connection, const BinaryStream* dataStream)
{
  /*uint16 formatVersion =*/ dataStream->ReadUInt16();
  uint32 serverId = dataStream->ReadUInt32();
  BugReportFile* brFile = GetPartialFile( serverId );

  BinaryStream* outStream = m_NetworkServer->CreatePacket(BugReportNetworkPacketType::PacketType_BugReportStatusFile);
  outStream->WriteUInt32(serverId);

  if (brFile != NULL)
  {
    outStream->WriteUInt32(brFile->GetFileId());
    outStream->WriteUInt32(brFile->GetCurrentLength());
  }
  else
  {
    outStream->WriteUInt32(0);
  }

  LogDebugFMT("BugReportServer", "SendData [ConnectionId:%d] [Packet:%d] ", connection->GetConnectionId(), BugReportNetworkPacketType::PacketType_BugReportStatusFile);
  m_NetworkServer->SendData(connection, outStream);
  delete brFile;
}

void BugReportServer::HandlePartialFile(NetworkConnection* /*connection*/, const BinaryStream* /*dataStream*/)
{
  LogError("BugReportServer", "HandlePartialFile :: Not implemented");
}

//#endif
