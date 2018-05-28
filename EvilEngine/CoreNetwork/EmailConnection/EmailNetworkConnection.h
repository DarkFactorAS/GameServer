#ifndef EVILENGINE_CORENETWORK_EMAILCONNECTION_H
#define EVILENGINE_CORENETWORK_EMAILCONNECTION_H

#include "EvilEngine/CoreLib/BasicTypes/Mutex/Mutex.h"
#include "../Common/NetworkConnection.h"
//#include <boost/detail/atomic_count.hpp>

class BinaryStream;
class NetworkBase;

class EmailNetworkConnection : public NetworkConnection
{
public:
  EmailNetworkConnection( NetworkBase* parent );
  ~EmailNetworkConnection();

  enum MailCommands
  {
    e_WaitingToConnect = 0,
    e_WaitingForGreeting,
    e_WaitingForHalo,
    e_WaitingForEmail,
    e_WaitingForMailFromReply,
    e_WaitingForRcptToReply,
    e_WaitingForDataReply,
    e_WaitingForDataDone,
    e_WaitingForQuit
  };

  /// Abstract functions that has to be implemented
  virtual void OnReceivedNetworkData( BinaryStream& dataStream );

  void SendMail( String receiver, String sender, String subject, String message );

private:
  void SendCommand( const String& command );

  MailCommands m_ExpectedCommand;

  String m_Sender;
  String m_Receiver;
  String m_Subject;
  String m_Message;
};

#endif // EVILENGINE_CORENETWORK_EMAILCONNECTION_H
