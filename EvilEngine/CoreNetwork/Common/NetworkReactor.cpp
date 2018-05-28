/*************************************************************************************
* Project     : EvilEngine 2012
* File        : NetworkReadctor
* Description : Handles the threads that is in charge of doing the actual read/write
*               on the network socket. This class is a singleton and is a shared 
*               resource between all NetworkConnections.
*               
* Author      : Thor Richard Hansen
 *************************************************************************************/

#include "Precompile.h"
#include "NetworkReactor.h"
#include <boost/make_shared.hpp>

NetworkReactor NetworkReactor::s_NetworkReactorInstance;

NetworkReactor::NetworkReactor() :
  m_IOService(NULL),
  m_WorkOrder(NULL)
{
  Start();
}

/***********************************************************************************
* GetInstance:
* Returns the singleton instance of this class
*  
* @return (NetworkRactor) - The instance of the networkreactor
*
* @author Thor Richard Hansen
***********************************************************************************/
NetworkReactor& NetworkReactor::GetInstance()
{
  return s_NetworkReactorInstance;
}

/***********************************************************************************
* Start:
* Initializes the IO service and starts it in a seperate thread
*  
* @author Thor Richard Hansen
***********************************************************************************/
void NetworkReactor::Start()
{
  m_IOService = new boost::asio::io_service();
  // create a work order so ioservice::run wont exit even when it has nothing to do
  m_WorkOrder = new boost::asio::io_service::work(*m_IOService);
  // Network now has 2 dedicated threads. (so it can do incoming, while compressing outgoing -- only servers needs this.)
  m_ServiceThreads.push_back( boost::make_shared<boost::thread>(boost::bind(&NetworkReactor::Run, this)) );
}

/***********************************************************************************
* ThreadRun:
* Starts the thread for this connection
*  
* @author Thor Richard Hansen
***********************************************************************************/
void NetworkReactor::Run()
{
  //SetThreadName("Network");
  //tbb::task_scheduler_init taskPool( 4 );

  boost::system::error_code ec;
  m_IOService->run(ec);
}

/***********************************************************************************
* Stop:
* Stops the network thread. This function will block until the IO service thread
* has finished.
*  
* @author Thor Richard Hansen
***********************************************************************************/
void NetworkReactor::Stop()
{
  //tell the ioservice to exit as soon as possible
  m_IOService->stop();

  for (std::vector<boost::shared_ptr<boost::thread> >::iterator i = m_ServiceThreads.begin(); i != m_ServiceThreads.end(); ++i)
  {
    (*i)->join();
  }

  m_ServiceThreads.clear();

  delete m_WorkOrder;
  delete m_IOService;
}

/***********************************************************************************
* Poll:
* Poll the IO service so it will perform the work that is currently waiting. This 
* should not be called when Start has been called.
*  
* @author Thor Richard Hansen
***********************************************************************************/
bool NetworkReactor::Poll()
{
  boost::system::error_code ec;
  m_IOService->poll(ec);
  return !ec;
}
