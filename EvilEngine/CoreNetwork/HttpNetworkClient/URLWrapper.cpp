#include "Precompile.h"

#include <curl/curl.h>
#include "URLWrapper.h"
#include "Core/IO/IOBuffer.h"

#define URLWrapper_MaxPipelineLength 2
#define URLWrapper_MaxHostConnections 8

#define CURLVAR2(x, y) x ## y
#define CURLVAR(x, y) CURLVAR2(x, y)
#define CURL_CHECK_RETURN(expr) \
  CURLcode CURLVAR(curlCode, __LINE__) = expr; \
  if(CURLVAR(curlCode, __LINE__) != CURLE_OK) \
  { \
  DBPrintf(INFOL_CODEERROR, e_URLWrapper, #expr" returned error : %d", CURLVAR(curlCode, __LINE__)); \
  }

// Static class in charge of curl initialization / shutdown
struct URLWrapperInitializer
{
  URLWrapperInitializer()
  {
    curl_global_init( CURL_GLOBAL_ALL );
  }
  ~URLWrapperInitializer()
  {
    curl_global_cleanup();
  }
} s_URLWrapper;

class URLWrapper::InternalData
{
public:
  InternalData() : m_Curl(NULL), m_Headers(NULL), m_ErrorBuffer(NULL) {}

  //typedef void CURL;
  CURL* m_Curl;
  String m_IP; // filled upon errors, or completed transfer.
  curl_slist* m_Headers;
  char* m_ErrorBuffer;
  IOBuffer m_Data;
};

size_t CurlHeaderDataCallback(void* buffer, size_t size, size_t count, void* userData);
size_t CurlWriteDataCallback(void* buffer, size_t size, size_t count, void* userData);
int CurlXferInfoCallback(void *buffer, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);

URLWrapper::URLWrapper()
  : m_Data(new URLWrapper::InternalData)
  , m_Callback(NULL)
  , m_HeaderCallback(NULL)
  , m_UserData(NULL)
  , m_AbortSignaled(false)
  , m_ResultCode(eUninitialized)
{
  // initialize the curl handle and reset it to nice default settings
  m_Data->m_Curl = curl_easy_init();
  curl_easy_reset(m_Data->m_Curl);

  //allocate and clear an error buffer curl can use to write error messages
  m_Data->m_ErrorBuffer = new char[CURL_ERROR_SIZE];
  memset( m_Data->m_ErrorBuffer, 0, CURL_ERROR_SIZE );
  CURL_CHECK_RETURN(curl_easy_setopt(m_Data->m_Curl, CURLOPT_ERRORBUFFER, m_Data->m_ErrorBuffer));

  CURL_CHECK_RETURN(curl_easy_setopt(m_Data->m_Curl, CURLOPT_WRITEFUNCTION, CurlWriteDataCallback));
  CURL_CHECK_RETURN(curl_easy_setopt(m_Data->m_Curl, CURLOPT_WRITEDATA, this));

  CURL_CHECK_RETURN(curl_easy_setopt(m_Data->m_Curl, CURLOPT_XFERINFOFUNCTION, CurlXferInfoCallback));
  CURL_CHECK_RETURN(curl_easy_setopt(m_Data->m_Curl, CURLOPT_XFERINFODATA, this));

  //CURL_CHECK_RETURN(curl_easy_setopt(m_Data->m_Curl, CURLOPT_TIMEOUT_MS, 2000));
}

URLWrapper::~URLWrapper()
{
  if(m_Data->m_Headers)
  {
    curl_slist_free_all(m_Data->m_Headers);
  }

  //free the curl handle and the error buffer
  curl_easy_cleanup(m_Data->m_Curl);
  delete[] m_Data->m_ErrorBuffer;
  delete m_Data;
}

void URLWrapper::SetURL(const String& url)
{
  CURL_CHECK_RETURN(curl_easy_setopt(m_Data->m_Curl, CURLOPT_URL, url.c_str()));
}

void URLWrapper::UseHTTP()
{
  CURL_CHECK_RETURN(curl_easy_setopt(m_Data->m_Curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP));
}

void URLWrapper::SetTimeout(long seconds)
{
  CURL_CHECK_RETURN(curl_easy_setopt(m_Data->m_Curl, CURLOPT_TIMEOUT, seconds));
}

void URLWrapper::SetConnectionTimeout(long seconds)
{
  CURL_CHECK_RETURN(curl_easy_setopt(m_Data->m_Curl, CURLOPT_CONNECTTIMEOUT, seconds));
}

void URLWrapper::SetNoProgress()
{
  CURL_CHECK_RETURN(curl_easy_setopt(m_Data->m_Curl, CURLOPT_NOPROGRESS, 1));
}

void URLWrapper::SetNoSignals()
{
  /// in unix systems, CURLOPT_NOSIGNAL need to be used if you're using the
  /// default resolver, as it will use signals to time-out otherwise and signals
  /// and threads don't mix.
  CURL_CHECK_RETURN(curl_easy_setopt( m_Data->m_Curl, CURLOPT_NOSIGNAL, 1)); // http://curl.haxx.se/mail/tracker-2009-04/0014.html
}

void URLWrapper::SetMaxRedirects(long redirects)
{
  CURL_CHECK_RETURN(curl_easy_setopt(m_Data->m_Curl, CURLOPT_MAXREDIRS, redirects));
}

void URLWrapper::SetFollowLocation()
{
  CURL_CHECK_RETURN(curl_easy_setopt(m_Data->m_Curl, CURLOPT_FOLLOWLOCATION, 1));
}

void URLWrapper::SetLowSpeedLimit(long bytesPerSec)
{
  CURL_CHECK_RETURN(curl_easy_setopt(m_Data->m_Curl, CURLOPT_LOW_SPEED_LIMIT, bytesPerSec));
}

void URLWrapper::SetLowSpeedTime(long seconds)
{
  CURL_CHECK_RETURN(curl_easy_setopt(m_Data->m_Curl, CURLOPT_LOW_SPEED_TIME, seconds)); // Seconds for connection to be below LOW_SPEED_LIMIT in order to consider it dead
}

void URLWrapper::SetUserAgent(const String& userAgent)
{
  CURL_CHECK_RETURN(curl_easy_setopt(m_Data->m_Curl, CURLOPT_USERAGENT, userAgent.c_str()));
}

void URLWrapper::SetProxy(const String& host, uint16 port)
{
  CURL_CHECK_RETURN(curl_easy_setopt(m_Data->m_Curl, CURLOPT_PROXY, host.c_str()));
  CURL_CHECK_RETURN(curl_easy_setopt(m_Data->m_Curl, CURLOPT_PROXYPORT, port));
}

void URLWrapper::AddHeaderLine(const String& header, const String& value)
{
  String headerLine = boost::format("%1%: %2%") % header % value;
  m_Data->m_Headers = curl_slist_append(m_Data->m_Headers, headerLine.c_str());
}

void URLWrapper::Recycle()
{
  ResumeConnection();
  SetUserData(NULL);
  SetCallback(NULL);
  if(m_Data->m_Headers)
  {
    curl_slist_free_all(m_Data->m_Headers);
    m_Data->m_Headers = NULL;
  }
  CURL_CHECK_RETURN(curl_easy_setopt(m_Data->m_Curl, CURLOPT_POST, 0));
  CURL_CHECK_RETURN(curl_easy_setopt(m_Data->m_Curl, CURLOPT_HTTPHEADER, NULL));
}

void URLWrapper::PauseConnection()
{
  CURL_CHECK_RETURN(curl_easy_pause(m_Data->m_Curl, CURLPAUSE_ALL));
}

void URLWrapper::ResumeConnection()
{
  CURL_CHECK_RETURN(curl_easy_pause(m_Data->m_Curl, CURLPAUSE_CONT));
}

void URLWrapper::SetMaxRecvSpeed(long bytesPerSec)
{
  curl_easy_setopt(m_Data->m_Curl, CURLOPT_MAX_RECV_SPEED_LARGE, static_cast<curl_off_t>(bytesPerSec));
}

void URLWrapper::SetCallback(fnDataCallback callback)
{
  m_Callback = callback;
}

void URLWrapper::SetHeaderCallback(fnDataCallback callback)
{
  CURL_CHECK_RETURN(curl_easy_setopt(m_Data->m_Curl, CURLOPT_HEADERFUNCTION, CurlHeaderDataCallback));
  CURL_CHECK_RETURN(curl_easy_setopt(m_Data->m_Curl, CURLOPT_WRITEHEADER,     this));
  m_HeaderCallback = callback;
}

String URLWrapper::GetPrimaryIP() const
{
  return m_Data->m_IP;
}

String URLWrapper::GetErrorMessage() const
{
  return String(m_Data->m_ErrorBuffer);
}

String URLWrapper::GetData() const
{
  return String(reinterpret_cast<char*>(m_Data->m_Data.GetBuffer()), static_cast<size_t>(m_Data->m_Data.GetSize()));
}

size_t CurlWriteDataCallback(void* buffer, size_t size, size_t count, void* userData)
{
  URLWrapper* urlWrapper = reinterpret_cast<URLWrapper*>(userData);
  size_t totalSize = size*count;
  if(urlWrapper && urlWrapper->GetCallback())
  {
    IOBuffer data;
    data.Write(buffer, totalSize);
    data.SetPos(0, IOPOS_Start);
    return urlWrapper->GetCallback()(data, urlWrapper);
  }
  // No callback was specified
  if (urlWrapper && buffer && totalSize)
  {
    urlWrapper->GetInternalData()->m_Data.Write(buffer, totalSize);
  }
  return totalSize;
}

size_t CurlHeaderDataCallback(void* buffer, size_t size, size_t count, void* userData)
{
  URLWrapper* urlWrapper = reinterpret_cast<URLWrapper*>(userData);
  size_t totalSize = size*count;
  if(urlWrapper && urlWrapper->GetHeaderCallback())
  {
    IOBuffer data;
    data.Write(buffer, totalSize);
    data.SetPos(0, IOPOS_Start);
    return urlWrapper->GetHeaderCallback()(data, urlWrapper);
  }
  return totalSize;
}

int CurlXferInfoCallback(void *buffer, curl_off_t /*dltotal*/, curl_off_t /*dlnow*/, curl_off_t /*ultotal*/, curl_off_t /*ulnow*/)
{
  URLWrapper* urlWrapper = reinterpret_cast<URLWrapper*>(buffer);

  if (urlWrapper->AbortSignaled())
    return -1;

  return 0;
}

void URLWrapper::SetPostData(const String& data)
{
  AddHeaderLine("Content-Type", "application/x-www-form-urlencoded");
  AddHeaderLine("Content-Length", String::FormatString("%d", data.Length()));

  CURL_CHECK_RETURN(curl_easy_setopt(m_Data->m_Curl, CURLOPT_POST, 1));
  CURL_CHECK_RETURN(curl_easy_setopt(m_Data->m_Curl, CURLOPT_COPYPOSTFIELDS, data.c_str()));
}


bool URLWrapper::Load(const String& url)
{
  m_Data->m_Data.Clear(false);
  if(!url.empty())
  {
    CURL_CHECK_RETURN(curl_easy_setopt(m_Data->m_Curl, CURLOPT_URL,           url.c_str()));
  }

  //if a header data callback is registered, we set the internal callback function so it can pack the data and send the actual callback
  if(m_HeaderCallback != NULL)
  {
    CURL_CHECK_RETURN(curl_easy_setopt(m_Data->m_Curl, CURLOPT_HEADERFUNCTION, CurlHeaderDataCallback));
    CURL_CHECK_RETURN(curl_easy_setopt(m_Data->m_Curl, CURLOPT_WRITEHEADER, this));
  }
  if(m_Data->m_Headers != NULL)
  {
    CURL_CHECK_RETURN(curl_easy_setopt(m_Data->m_Curl, CURLOPT_HTTPHEADER, m_Data->m_Headers));
  }

  long resultCode = curl_easy_perform(m_Data->m_Curl);
  SetResultCode(resultCode);
  if (resultCode == CURLE_OK)
  {
    return true;
  }
  return false;
}

void URLWrapper::SetResultCode(long result)
{
  switch(result)
  {
  case CURLE_OK:
    m_ResultCode = eOK;
    break;
  case CURLE_COULDNT_RESOLVE_PROXY:
    m_ResultCode = eCouldntResolveProxy;
    break;
  case CURLE_COULDNT_RESOLVE_HOST:
    m_ResultCode = eCouldntResolveHost;
    break;
  case CURLE_COULDNT_CONNECT:
    m_ResultCode = eCouldntConnect;
    break;
  default:
    DBPrintf(INFOL_CODEWARNING, debug::e_URLWrapper, "Unknown CURL warning occurred: %d", result);
    m_ResultCode = eSomething;
    break;
  }
}

long URLWrapper::GetResultCode() const
{
  return m_ResultCode;
}

long URLWrapper::GetResponse() const
{
  long responseCode = 0;
  CURL_CHECK_RETURN(curl_easy_getinfo( m_Data->m_Curl, CURLINFO_RESPONSE_CODE, &responseCode ));
  return responseCode;
}

class MultiURLWrapper::InternalData
{
public:
  InternalData() : m_MultiCurl(NULL),m_Pipelining(false) {}
  CURLM* m_MultiCurl;
  bool m_Pipelining;
};

MultiURLWrapper::MultiURLWrapper()
  : m_Data(new MultiURLWrapper::InternalData)
{
  m_Data->m_MultiCurl = curl_multi_init();
  fcAssertMsg(m_Data->m_MultiCurl, "curl_multi_init failed");
}

MultiURLWrapper::~MultiURLWrapper()
{
  curl_multi_cleanup(m_Data->m_MultiCurl);
  delete m_Data;
}

void MultiURLWrapper::Add(URLWrapper* urlWrapper)
{
  ThreadLock lock(&m_Mutex);
  if(urlWrapper->GetHeaderCallback() != NULL)
  {
    CURL_CHECK_RETURN(curl_easy_setopt(urlWrapper->m_Data->m_Curl, CURLOPT_HEADERFUNCTION, CurlHeaderDataCallback));
    CURL_CHECK_RETURN(curl_easy_setopt(urlWrapper->m_Data->m_Curl, CURLOPT_WRITEHEADER, urlWrapper));
  }
  if(urlWrapper->m_Data->m_Headers != NULL)
  {
    CURL_CHECK_RETURN(curl_easy_setopt(urlWrapper->m_Data->m_Curl, CURLOPT_HTTPHEADER, urlWrapper->m_Data->m_Headers));
  }
  if (m_Data->m_Pipelining)
  {
    // HTTP pipelining requires use of HTTP 1.1.
    CURL_CHECK_RETURN(curl_easy_setopt(urlWrapper->m_Data->m_Curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1));
  }

  m_CurrentlyLoading.push_back(urlWrapper);
  CURLMcode code = curl_multi_add_handle(m_Data->m_MultiCurl, urlWrapper->m_Data->m_Curl);
  if(code != CURLM_OK)
  {
    DBPrintf(INFOL_CODEERROR, e_URLWrapper, "curl_multi_add_handle failed %d", code);
  }
}

void MultiURLWrapper::Remove(URLWrapper* urlWrapper)
{
  ThreadLock lock(&m_Mutex);
  std::vector<URLWrapper*>::iterator i = std::find(m_CurrentlyLoading.begin(), m_CurrentlyLoading.end(), urlWrapper);
  if(i != m_CurrentlyLoading.end())
  {
    m_CurrentlyLoading.erase(i);
  }
  CURLMcode code = curl_multi_remove_handle(m_Data->m_MultiCurl, urlWrapper->m_Data->m_Curl);
  if(code != CURLM_OK)
  {
    DBPrintf(INFOL_CODEERROR, e_URLWrapper, "curl_multi_remove_handle failed %d", code);
  }

}

int MultiURLWrapper::Select()
{
#if defined( WINDOWS )
  ThreadLock lock(&m_Mutex);
  // currently disabled on the server for two reasons:
  // it slows patching down a lot, could be that curl are returning bogus fd_set's.
  // it won't work when the server has used more that 1024 filedescriptors (default select limit).
  //   so this will have to be rewritten to use poll or epoll on linux.
  fd_set  readSet;
  fd_set  writeSet;
  fd_set  execSet;
  int     maxFD;
  timeval timeout;
  {
    FD_ZERO( &readSet );
    FD_ZERO( &writeSet );
    FD_ZERO( &execSet );
    curl_multi_fdset( m_Data->m_MultiCurl, &readSet, &writeSet, &execSet, &maxFD );
    timeout.tv_usec = 100000;
    timeout.tv_sec = 0;
  }
  return select( maxFD, &readSet, &writeSet, NULL, &timeout );
#else
  return 0;
#endif
}

bool MultiURLWrapper::IsValid() const
{
  return m_Data->m_MultiCurl != NULL;
}

void MultiURLWrapper::EnablePipelining()
{
  ThreadLock lock(&m_Mutex);
  // Enable HTTP pipelining. HTTP 1.1 must be enabled on each curl_easy interface too.
  curl_multi_setopt(m_Data->m_MultiCurl, CURLMOPT_PIPELINING, 1);
  m_Data->m_Pipelining = true;

#if LIBCURL_VERSION_NUM >= 0x071E00 //< 7.30.0
  curl_multi_setopt(m_Data->m_MultiCurl, CURLMOPT_MAX_PIPELINE_LENGTH, URLWrapper_MaxPipelineLength); //< Default value is 5. Available in cURL 7.30.
  curl_multi_setopt(m_Data->m_MultiCurl, CURLMOPT_MAX_HOST_CONNECTIONS, URLWrapper_MaxHostConnections); //< If not set, CURLMOPT_PIPELINING will force this to 1.
  //curl_multi_setopt(m_Data->m_MultiCurl, CURLMOPT_MAX_TOTAL_CONNECTIONS, 8); //< If not set, CURLMOPT_PIPELINING will force this to 1.
  curl_multi_setopt(m_Data->m_MultiCurl, CURLMOPT_CONTENT_LENGTH_PENALTY_SIZE, 1048576);
  curl_multi_setopt(m_Data->m_MultiCurl, CURLMOPT_CHUNK_LENGTH_PENALTY_SIZE, 1048576);
  // Also available for pipeline tweaking:
  // CURLMOPT_CONTENT_LENGTH_PENALTY_SIZE
  // CURLMOPT_CHUNK_LENGTH_PENALTY_SIZE
  // CURLMOPT_PIPELINING_SITE_BL
  // CURLMOPT_PIPELINING_SERVER_BL
#endif // LIBCURL_VERSION_NUM >= 0x071E00
}

uint32 MultiURLWrapper::GetMaxPipelinedRequests() const
{
  return URLWrapper_MaxHostConnections * URLWrapper_MaxPipelineLength * 2;
}

std::vector<URLWrapper*> MultiURLWrapper::Load()
{
  ThreadLock lock(&m_Mutex);
  std::vector<URLWrapper*> returnVal;
  int runningHandles = 0;
  while( curl_multi_perform( m_Data->m_MultiCurl, &runningHandles ) == CURLM_CALL_MULTI_PERFORM ) 
  {
    //empty body
  }

  CURLMsg* curlMsg;
  //Loop over all messages that are ready within the CURL multi system
  for( curlMsg = curl_multi_info_read( m_Data->m_MultiCurl, &runningHandles ); curlMsg; curlMsg = curl_multi_info_read( m_Data->m_MultiCurl, &runningHandles ))
  {
    if(curlMsg->msg == CURLMSG_DONE)
    {
      //Find the correct URLWrapper 
      std::vector<URLWrapper*>::const_iterator i;
      for( i = m_CurrentlyLoading.begin(); i != m_CurrentlyLoading.end(); ++i )
      {
        CURL* curl = (*i)->m_Data->m_Curl;
        if(curlMsg->easy_handle == curl)
        {
          (*i)->SetResultCode(curlMsg->data.result);
          // Get the remote address
          char *primaryIP = nullptr;
          long primaryPort = 0;
          if (curl_easy_getinfo(curl, CURLINFO_PRIMARY_IP, &primaryIP) == CURLE_OK)
          {
            curl_easy_getinfo(curl, CURLINFO_PRIMARY_PORT, &primaryPort);
            (*i)->m_Data->m_IP = String::FormatString("%s:%d", primaryIP, primaryPort);
          } else
            (*i)->m_Data->m_IP = "unresolved ip address";

          returnVal.push_back(*i);
          break;
        }
      }
      fcAssert( i != m_CurrentlyLoading.end() );
    }
  }
  return returnVal;
}
