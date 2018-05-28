/*************************************************************************************
* Project     : Evil Engine 2012
* File        : BugReportClient
* Description : Client that sends bugreports to the server
*
* Author      : Thor Richard Hansen
*************************************************************************************/

#include "Precompile.h"
#include "BugReportClient.h"
#include "EvilEngine/CoreIO/UserConfig/UserConfig.h"
#include "EvilEngine/CoreLib/Utils/CoreUtils.h"
#include "EvilEngine/CoreNetwork/Client/NetworkClient.h"
#include "EvilEngine/CoreNetwork/Common/NetworkConnection.h"

#include "EvilEngine/CoreBugReport/Common/BugReportNetworkPacketType.h"
#include "EvilEngine/CoreBugReport/Common/BugReport.h"

BugReportClient* BugReportClient::s_Instance = NULL;

BugReportClient::BugReportClient( const String& appName, const String& appVersion, const String& serverHostnameAndPort ) :
  m_AppName(appName),
  m_AppVersion(appVersion),
  m_Isloaded(false)
{
  String threadName = String::FormatString("%s-BugClientNetwork", appName.c_str());

  m_NetworkClient = new NetworkClient(threadName, true);
  m_NetworkClient->AddNetworkHost(NetworkClient::NETWORKMODE_LIVE, serverHostnameAndPort);
  SetNetworkBase(m_NetworkClient);

  m_UserConfig = new UserConfig( "BugReports" );

  // Send all asserts as bugreport
  dfAssertHandler::SignalAssertMessage.Connect(this, &BugReportClient::SendAssertAsBugreport);
}

void BugReportClient::SetNetworkBase(NetworkBase* networkBase)
{
  m_NetworkClient = static_cast<NetworkClient*> ( networkBase);
  CoreNetworkEngine::SetNetworkBase(networkBase);
}


void BugReportClient::SendAssertAsBugreport(const char* errorMessage, const char* callstack, bool isFatal )
{
  SendBugReport(new BugReport(errorMessage, callstack, "assert@darkfactor.net"));
  if (isFatal)
  {
    BlockAndSendReports();
  }
}

BugReportClient::~BugReportClient()
{
  delete m_UserConfig;
}

BugReportClient* BugReportClient::CreateInstance(const String& appName, const String& appVersion, const String& serverHostnameAndPort, bool sendReports )
{
  dfAssert(s_Instance == NULL, "BugReportClient already initialized");
  s_Instance = new BugReportClient(appName, appVersion, serverHostnameAndPort);
  if (sendReports)
  {
    s_Instance->BlockAndSendReports();
  }
  return s_Instance;
}

BugReportClient* BugReportClient::CreateInstance(const String& appName, uint32 appVersion, const String& serverHostnameAndPort, bool sendReports)
{
  dfAssert(s_Instance == NULL, "BugReportClient already initialized");
  String sAppVersion = String::FromInt( appVersion );
  s_Instance = new BugReportClient(appName, sAppVersion, serverHostnameAndPort);
  if (sendReports)
  {
    s_Instance->BlockAndSendReports();
  }
  return s_Instance;
}

BugReportClient* BugReportClient::GetInstanceIfAny()
{
  return s_Instance;
}

void BugReportClient::DestroyInstance()
{
  delete s_Instance;
  s_Instance = NULL;
}


bool BugReportClient::InitClient(bool sendAfterLoad)
{
  if (m_NetworkClient != NULL)
  {
    Load(sendAfterLoad);
    return true;
  }
  return false;
}

void BugReportClient::InitSend()
{
  // No need to connect if we do not have reports
  if (m_BugReportList.empty())
  {
    return;
  }

  if ( m_NetworkClient->MustReconnect() )
  {
    m_NetworkClient->ConnectAsync();
  }

  SlotProcessReports();
}

void BugReportClient::BlockAndSendReports()
{
  InitClient(true);
  uint32 numReports = m_BugReportList.size();
  if ( numReports > 0 )
  {
    LogInfoFMT("BugReportClient", "BlockAndSendReports:: Waiting to send %d bugreports to bugreport server", numReports);
    int timeOut = 30000; // 30 second timeout
    bool isRunning = true;
    while ( isRunning && timeOut > 0 && ( m_BugReportList.size() > 0 ) )
    {
      isRunning = Run();
      boost::this_thread::sleep(boost::posix_time::milliseconds(100));
      timeOut -= 100;
    }
    LogInfoFMT("BugReportClient", "BlockAndSendReports:: Sendt %d/%d bugreports to bugreport server", numReports - m_BugReportList.size(), numReports );
  }
}

void BugReportClient::ChangeBugServer(int universeType)
{
  if ( m_NetworkClient->HasHostType( universeType ) )
  {
    m_NetworkClient->SetCurrentHost(universeType);
    m_NetworkClient->Disconnect();
    //BlockAndSendReports();
  }
}

int BugReportClient::GetCurrentHost()
{
  return m_NetworkClient->GetCurrentHost();
}

bool BugReportClient::HasHostType(int networkType)
{
  return m_NetworkClient->HasHostType(networkType);
}

void BugReportClient::SetBugReportHost( const String& serverHostnameAndPort )
{
  if (m_NetworkClient->AddNetworkHost(NetworkClient::NETWORKMODE_LIVE, serverHostnameAndPort))
  {
    ChangeBugServer(NetworkClient::NETWORKMODE_LIVE);
  }
}

uint32 BugReportClient::Load(bool sendAfterLoad)
{
  if (m_Isloaded)
  {
    if (sendAfterLoad)
    {
      InitSend();
    }
    return 0;
  }

  uint32 numLoaded = 0;

  // Load all bugreports
  if (m_UserConfig->Load())
  {
    m_Isloaded = true;
    bool reWriteList = false;
    VariantTree* tree = m_UserConfig->GetConfigTree();
    std::vector< VariantTree* >& groupList = tree->GetGroupList();
    for (uint32 index = 0; index < groupList.size(); index++)
    {
      VariantTree* child = groupList[index];
      BugReport* report = BugReport::CreateFromData(child);
      if (report != NULL)
      {
        if (AddBugReport(report))
        {
          numLoaded++;
        }
      }
      else
      {
        // Rewrite buglist to file/registry since some of them are broken
        reWriteList = true;
      }
    }
    // Rewrite the bugrepots to 
    if (reWriteList)
    {
      //LogErrorFMT("BugReport","Error with bugreport data. Rewriting list. [Old tree]\n%s", tree->DumpTreeToString("[Dump] =").c_str() );
      LogError("BugReport", "Error with bugreport data. Rewriting list.\n");
      m_UserConfig->Delete();

      for (std::vector< BugReport* >::const_iterator itReport = m_BugReportList.begin(); itReport != m_BugReportList.end(); ++itReport)
      {
        BugReport* bugReport = *itReport;
        VariantTree* bugTree = bugReport->CreateVariantTree();
        bugTree->SetName(String(bugReport->GetClientId()));
        m_UserConfig->AddChild(bugTree);
      }
      m_UserConfig->Save();
    }

    if (!m_BugReportList.empty())
    {
      if (sendAfterLoad)
      {
        InitSend();
      }
    }
    else if (groupList.size() > 0)
    {
      LogError("BugReport", "Error with bugreport data");
    }
  }

  return numLoaded;
}

bool BugReportClient::AddBugReport(BugReport* newBugReport)
{
  uint32 maxClientId = 0;

  // Check for duplicate
  for (std::vector< BugReport* >::const_iterator itReport = m_BugReportList.begin(); itReport != m_BugReportList.end(); itReport++)
  {
    BugReport* bugReport = *itReport;
    if (bugReport->GetClientId() == newBugReport->GetClientId())
    {
      return false;
    }
    maxClientId = std::max( maxClientId, bugReport->GetClientId() );
  }

  BugReport::SetMaxClientId(maxClientId);
  m_BugReportList.push_back(newBugReport);
  return true;
}

bool BugReportClient::SaveBugReport(BugReport* bugReport)
{
  if (bugReport->GetServerId() == 0)
  {
    GenerateUniqueClientId(bugReport);
    VariantTree* tree = bugReport->CreateVariantTree();
    tree->SetName(String(bugReport->GetClientId()));
    m_UserConfig->AddChild(tree);
    return m_UserConfig->Save();
  }
  return false;
}

bool BugReportClient::DeleteSavedBugReport(uint32 clientId)
{
  if (m_UserConfig->DeleteChild(String(clientId)))
  {
    return true;
  }
  LogWarning("BugReportClient", "Failed to delete and save bugreports!");
  return false;
}

bool BugReportClient::DeleteBugReport(BugReport* removeBugReport)
{
  bool didDelete = DeleteSavedBugReport( removeBugReport->GetClientId() );

  for (std::vector< BugReport* >::iterator itReport = m_BugReportList.begin(); itReport != m_BugReportList.end(); itReport++)
  {
    BugReport* bugReport = *itReport;
    if (bugReport->GetClientId() == removeBugReport->GetClientId())
    {
      m_BugReportList.erase( itReport );
      delete bugReport;
      return didDelete;
    }
  }
  return didDelete;
}

bool BugReportClient::DeleteAllBugReports()
{
  Load(false);

  bool deletedAllReports = true;
  for (std::vector< BugReport* >::const_iterator itReport = m_BugReportList.begin(); itReport != m_BugReportList.end(); ++itReport)
  {
    BugReport* bugReport = *itReport;
    deletedAllReports = DeleteSavedBugReport(bugReport->GetClientId()) && deletedAllReports;
    delete bugReport;
  }
  m_BugReportList.clear();
  return deletedAllReports;
}

void BugReportClient::GenerateUniqueClientId(BugReport* bugReport)
{
  if ( bugReport->GetClientId() == 0 )
  {
    uint32 clientId = bugReport->GenerateClientId();
    bugReport->SetClientId(clientId);
  }
}

void BugReportClient::SendBugReport(BugReport* bugReport)
{
  GenerateUniqueClientId(bugReport);

  bugReport->SetClientName(m_AppName);
  bugReport->SetClientVersion(m_AppVersion);

  AddBugReport(bugReport);
  SaveBugReport(bugReport);
  InitSend();
}

void BugReportClient::SlotProcessReports()
{
  //BugReportNetworkConnection* bugConnection = safe_cast<BugReportNetworkConnection*> ( m_NetworkClient->GetConnection() );
  //if (bugConnection != NULL && bugConnection->IsConnected())
  if ( m_NetworkClient->IsConnected() && m_BugReportList.size() > 0 )
  {
    BugReport* bugReport = m_BugReportList[0];
    if (bugReport->IsSendingToServer())
    {
      return;
    }

    // Initial bugreport
    if (bugReport->GetServerId() == 0)
    {
      BinaryStream* data = m_NetworkClient->CreatePacket(BugReportNetworkPacketType::PacketType_BugReport);
      bugReport->WriteData(data);
      bugReport->SetSendingToServer(true);
      LogInfoFMT("BugReportClient", "Sending bugreport[ClientID:%d] '%s' to server", bugReport->GetClientId(), bugReport->GetTitle().c_str());
      m_NetworkClient->SendData(data);
    }
    // TODO : Request file status
    // For now : Send first file
    else if (bugReport->GetNumFiles() > 0)
    {
      SaveBugReport(bugReport);
      bugReport->SetSendingToServer(true);
      SendFilesForBugreport(bugReport);
    }
    // Bugreport is complete
    else
    {
      LogInfoFMT("BugReportClient", "Successfully sent bugreport[ClientID:%d - ServerID:%d] '%s' to server...", bugReport->GetClientId(), bugReport->GetServerId(), bugReport->GetTitle().c_str());
      SignalBugReportCompleted(bugReport);
      DeleteBugReport(bugReport);
      SlotProcessReports();
    }
  }
  SignalUpdatedReports((uint16)m_BugReportList.size());
}

bool BugReportClient::Run()
{
  if ( m_NetworkClient != NULL )
  {
    bool netRun     = m_NetworkClient->Run();
    bool engineRun = CoreNetworkEngine::Run();
    return (netRun && engineRun);
  }
  return false;
}

BugReport* BugReportClient::GetBugReportWithClientId( uint32 clientId )
{
  for (uint32 index = 0; index < m_BugReportList.size(); index++)
  {
    BugReport* bugReport = m_BugReportList[index];
    if (bugReport->GetClientId() == clientId)
    {
      return bugReport;
    }
  }
  return NULL;
}

BugReport* BugReportClient::GetBugReportWithServerId(uint32 serverId)
{
  for (uint32 index = 0; index < m_BugReportList.size(); index++)
  {
    BugReport* bugReport = m_BugReportList[index];
    if (bugReport->GetServerId() == serverId)
    {
      return bugReport;
    }
  }
  return NULL;
}

void BugReportClient::ReceivedBugReportID(const BinaryStream* dataStream)
{
  if (m_NetworkClient->IsConnected())
  {
    uint32 serverId = dataStream->ReadUInt32();
    uint32 clientId = dataStream->ReadUInt32();

    BugReport* bugReport = GetBugReportWithClientId( clientId );
    if (bugReport != NULL)
    {
      bugReport->SetServerId(serverId);
      bugReport->SetSendingToServer(false);

      SlotProcessReports();
    }
  }
}

void BugReportClient::ServerReceivedFileOK(const BinaryStream* dataStream)
{
  uint32 serverId = dataStream->ReadUInt32();
  uint32 fileId   = dataStream->ReadUInt32();

  BugReport* bugReport = GetBugReportWithServerId(serverId);
  if (bugReport != NULL)
  {
    LogInfoFMT("BugReportClient", "Successfully sent bugreport[SID:%u] file[SID:%u] '%s' to server. (%d files)", 
      bugReport->GetServerId(), 
      fileId,
      bugReport->GetTitle().c_str(), bugReport->GetNumFiles() );
    SignalBugReportCompleted(bugReport);

    bugReport->DeleteFileWithId(fileId);
    bugReport->SetSendingToServer(false);

    SlotProcessReports();
  }
  else
  {
    LogFatalFMT("BugReportClient", "ServerReceivedFileOK::Unknown bugreport with serverId : %u", serverId);
  }
}

void BugReportClient::SendFilesForBugreport(BugReport* bugReport)
{
  if (bugReport->GetNumFiles() > 0)
  {
    const BugReportFile* brFile = bugReport->GetFileAt(0);
    if (brFile != NULL)
    {
      BinaryStream* outFileStream = m_NetworkClient->CreatePacket(BugReportNetworkPacketType::PacketType_BugReportFile);
      bugReport->WriteFileData(brFile, outFileStream);
      m_NetworkClient->SendData(outFileStream);
      LogInfoFMT("BugReportClient", "Sending bugreport[ServerID:%d] file [FileID:%d] to server", bugReport->GetServerId(), brFile->GetFileId());
      return;
    }
  }
  LogFatal("BugReportClient", "SendFilesForBugreport::No files to send");
}

/***********************************************************************************
* OnConnectionChanged:
* Callback for when the network connection changes status
*
* @param (uint32)         connectionInstance - ID of the instance to the connection
* @param (uint32)         conectionStatus    - The status of the connection
*
* @author Thor Richard Hansen
***********************************************************************************/
void BugReportClient::OnConnectionChanged(uint32 /*connectionInstance*/, uint32 conectionStatus)
{
  SignalConnectionChanged(conectionStatus);

  switch( conectionStatus )
  {
    case NetworkConnection::NETWORKSTATUS_CONNECTED:
      InitSend();
      break;
    default:
      break;
  }
}

/***********************************************************************************
* OnReceivedNetworkPacket:
* Callback for data received from the network
*
* @param (uint32)         connectionInstance - ID of the instance to the connection
* @param (BinaryStream*)  dataStream         - The data we received
*
* @author Thor Richard Hansen
***********************************************************************************/
void BugReportClient::OnReceivedNetworkPacket(uint32 connectionInstance, const BinaryStream* dataStream)
{
  uint32 packetType = dataStream->ReadUInt32();
  LogInfoFMT("BugReportClient", "Received network data on connection :%d (packet:%u)", connectionInstance, packetType);
  switch (packetType)
  {
    case BugReportNetworkPacketType::PacketType_BugReportId:
      ReceivedBugReportID(dataStream);
      break;
    case BugReportNetworkPacketType::PacketType_BugReportFile:
      ServerReceivedFileOK(dataStream);
      break;
    default:
      LogFatalFMT("BugReportClient", "OnReceivedNetworkPacket: Unknown packetType : %d", packetType );
      break;
  }
}

uint32 BugReportClient::GetNumberOfBugReports()
{
  uint32 numLoaded = Load(false);
  if (numLoaded > 0)
  {
    LogInfoFMT("BugReportClient", "GetNumberOfBugReports:: Loaded %d reports", numLoaded );
  }
  return GetNumberOfQueuedBugreports();
}
