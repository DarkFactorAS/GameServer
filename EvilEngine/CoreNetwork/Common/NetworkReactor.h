#ifndef CORE_NETWORK_REACTOR
#define CORE_NETWORK_REACTOR
#pragma once

class NetworkReactor
{
public:
  static NetworkReactor& GetInstance();
  static boost::asio::io_service& GetIOService() { return *(GetInstance().m_IOService); }

  void Start();
  void Stop();

  bool Poll();
  void Run();
private:
  NetworkReactor();
  NetworkReactor(const NetworkReactor&);
  NetworkReactor& operator=(const NetworkReactor&);

  static NetworkReactor s_NetworkReactorInstance;

  boost::asio::io_service* m_IOService;
  boost::asio::io_service::work* m_WorkOrder;
  std::vector<boost::shared_ptr<boost::thread> > m_ServiceThreads;
};

#endif // CORE_NETWORK_REACTOR
