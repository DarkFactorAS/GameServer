#ifndef EVILENGINE_COREBUGREPORT_COMMON_BUGREPORT
#define EVILENGINE_COREBUGREPORT_COMMON_BUGREPORT 1

#include "EvilEngine/CoreLib/BasicTypes/BinaryStream/BinaryStream.h"
#include "EvilEngine/CoreLib/BasicTypes/Variant/VariantTree.h"

class BugReportFile
{
private:
  String      m_Filename;
  uint32      m_FileId;
  const char* m_Data;
  uint32      m_CurrentLength;
  uint32      m_TotalLength;
public:
  BugReportFile(const String& fileName, uint32 fileId, const char* data, uint32 dataLength, uint32 totalLength)
  {
    m_Filename      = fileName;
    m_FileId        = fileId;
    m_Data          = data;
    m_CurrentLength = dataLength;
    m_TotalLength   = totalLength;
  }

  ~BugReportFile()
  {
    delete m_Data;
    m_Data = NULL;
  }

  uint32          GetFileId() const{ return m_FileId; }
  uint32          GetCurrentLength() const { return m_CurrentLength; }
  uint32          GetFileLength() const { return m_TotalLength; }
  const String&   GetFileName() const { return m_Filename; }
  const char*     GetData() const{ return m_Data; }
};

class BugReport
{
public:
  BugReport();
  BugReport( const String& title, const String& message, const String& email );
  ~BugReport();


  void                AddFileData( const String& fileName, const char* data, uint32 dataLength );
  void                AddFile(BugReportFile* file);
  void                DeleteFileWithId(uint32 fileId);

  uint32              GetClientId() const{ return m_ClientId; }
  uint32              GetServerId() const{ return m_ServerId; }
  const String&       GetTitle() const { return m_Title; }
  const String&       GetMessageDesc() const { return m_Message; }
  const String&       GetEmail() const { return m_Email; }
  const String&       GetClientName() const { return m_ClientName; }
  const String&       GetClientVersion() const { return m_ClientVersion; }
  const String&       GetCreatedDate() const { return m_CreatedDate; }
  uint32              GetNumFiles() const{ return (uint32)m_Files.size(); }
  uint32              GetTotalFileSize() const;
  const BugReportFile* GetFileAt( uint32 position ) const;
  void                SetSaved( bool isSaved ){ m_IsSaved = isSaved; }

  void                SetClientId( uint32 id ){ m_ClientId = id; }
  void                SetServerId( uint32 id ){ m_ServerId = id; }
  void                SetTitle( const String& title ){ m_Title = title; }
  void                SetMessage( const String& message ){ m_Message = message; }
  void                SetEmail( const String& email ){ m_Email = email; }
  void                SetClientName( const String& name ){ m_ClientName = name; }
  void                SetClientVersion( const String& version ){ m_ClientVersion = version; }
  void                SetCreatedDate( const String& createdDate ){ m_CreatedDate = createdDate; }
  static uint32       GenerateClientId();
  static void         SetMaxClientId(uint32 clientId);

  bool                AddFile( const String& fileName );
  bool                IsSaved() const{ return m_IsSaved; }

  void                SetSendingToServer(bool isSending){ m_IsSending = isSending; }
  bool                IsSendingToServer() const { return m_IsSending; }

  static BugReport*   CreateHeaderFromData(const BinaryStream* data, uint16 formatVersion);
  static BugReport*   CreateFromData( const BinaryStream* data );
  static BugReport*   CreateFromData( VariantTree* data );
  VariantTree*        CreateVariantTree();
  void                WriteData( BinaryStream* data ) const;
  void                WriteFileData(const BugReportFile* brFile, BinaryStream* data) const;

  static BugReport*   ReadFileData( const BinaryStream* data );
  static uint32       GetFormatVersion();
  void                SetBugVersion(int bugVersion){ m_BugVersion = bugVersion; }
  int                 GetBugVersion(){ return m_BugVersion; }

private:

  static BugReport*   ReadFileDataV3(const BinaryStream* data);
  static BugReport*   ReadFileDataV4(const BinaryStream* data);
  static BugReport*   ReadFileDataV5(const BinaryStream* data);

  uint32              m_ClientId;
  uint32              m_ServerId;
  String              m_Title;
  String              m_Message;
  String              m_Email;
  String              m_ClientName;
  String              m_ClientVersion;
  String              m_CreatedDate;
  bool                m_IsSaved;
  bool                m_IsSending;
  int                 m_BugVersion;

  static uint32       s_LastBugId;

  std::vector< BugReportFile* > m_Files;
};

#endif /// EVILENGINE_COREBUGREPORT_COMMON_BUGREPORT 