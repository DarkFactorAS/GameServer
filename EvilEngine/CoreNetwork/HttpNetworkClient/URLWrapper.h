#ifndef CORELIBRARIES_CUREUTILS_URLWRAPPER_H
#define CORELIBRARIES_CUREUTILS_URLWRAPPER_H

class IOBuffer;

class URLWrapper
{
  class InternalData;
  friend class MultiURLWrapper;
public:
  URLWrapper();
  ~URLWrapper();

  enum ResultCodes
  {
    eUninitialized,
    eOK,
    eCouldntResolveProxy,
    eCouldntResolveHost,
    eCouldntConnect,
    eSomething 
  };

  /// Set the URL to fetch, can also be set in the Constructor
  void SetURL(const String& url);
  /// Tell the underlying url manager to only use http
  void UseHTTP();
  /// Set the timeout for the entire operation
  void SetTimeout(long seconds);
  /// Set the timeout for the connection 
  void SetConnectionTimeout(long seconds);
  /// Tell the url manager to shut off the progress meter
  void SetNoProgress();
  /// Tell the url manager to not send any signals 
  void SetNoSignals();
  /// Set the max number of redirects before giving up
  void SetMaxRedirects(long redirects);
  void SetPostData(const String& data);
  void SetFollowLocation();
  void SetLowSpeedLimit(long bytesPerSec);
  void SetLowSpeedTime(long seconds);
  void SetUserAgent(const String& userAgent);
  void SetProxy(const String& host, uint16 port);
  void AddHeaderLine(const String& header, const String& value);
  void Recycle();
  void PauseConnection();
  void ResumeConnection();
  void AbortTransfer() { m_AbortSignaled = true; }
  void SetMaxRecvSpeed(long bytesPerSec);
  void SetUserData(void* userdata) { m_UserData = userdata; } 
  typedef size_t (*fnDataCallback)(IOBuffer& buffer, URLWrapper* object);
  void SetCallback(fnDataCallback callback);
  void SetHeaderCallback(fnDataCallback callback);
  /// Download the actual data from the url
  bool Load(const String& url = String::zero);
  void SetResultCode(long result);
  long GetResultCode() const;
  long GetResponse() const;
  
  String GetPrimaryIP() const;
  String GetErrorMessage() const;
  /// Return the internal data 
  String GetData() const;

  fnDataCallback GetCallback() const { return m_Callback; }
  fnDataCallback GetHeaderCallback() const  { return m_HeaderCallback; }
  void* GetUserData() const { return m_UserData; } 

  // Needed for some of the non-member callback functions
  InternalData* GetInternalData() { return m_Data; }

  bool AbortSignaled() { return m_AbortSignaled; }
private:

  void ResetHandleState();

  InternalData* m_Data;

  /// The callback function pointer
  fnDataCallback m_Callback;
  fnDataCallback m_HeaderCallback;
  void* m_UserData;
  bool m_AbortSignaled;
  ResultCodes m_ResultCode;
};

/// This class is responsible for being able to download multiple files at the same time in the same thread
/// It is currently modeled heavily on the curl_multi system and will probably need some work when we make the flash side.
class MultiURLWrapper
{
  class InternalData;
public:
  MultiURLWrapper();
  ~MultiURLWrapper();

  void Add(URLWrapper* urlWrapper);
  void Remove(URLWrapper* urlWrapper);

  int Select();
  bool IsValid() const;
  std::vector<URLWrapper*> Load();

  uint32 GetMaxPipelinedRequests() const;
  void EnablePipelining();

public:
  InternalData* m_Data;

  std::vector<URLWrapper*> m_CurrentlyLoading;

private:
  ThreadMutex m_Mutex;
};

#endif //CORELIBRARIES_CUREUTILS_URLWRAPPER_H
