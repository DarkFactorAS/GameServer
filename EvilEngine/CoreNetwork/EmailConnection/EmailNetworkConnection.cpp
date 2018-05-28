/*************************************************************************************
* Project     : Evil Engine 2012
* File        : EmailNetworkConnection
* Description : Handles everything regarding a connection. All classes in the 
*               application/game that uses this network implementation will overload
*               this class to do specific readers/writers. This specific overload
*               is used to handle and send emails.
*               
* Author      : Thor Richard Hansen
*************************************************************************************/

#include "Precompile.h"
#include "EmailNetworkConnection.h"

/***********************************************************************************
* Constructor:
* 
* @param (NetworkBase) parent - Pointer to the client or server owning this 
*                               connection.
*
* @author Thor Richard Hansen
***********************************************************************************/
EmailNetworkConnection::EmailNetworkConnection( NetworkBase* parent ) : NetworkConnection( parent )
{
  m_ExpectedCommand = e_WaitingToConnect;
}

/***********************************************************************************
* Destructor:
* 
* @author Thor Richard Hansen
***********************************************************************************/
EmailNetworkConnection::~EmailNetworkConnection()
{
}

/***********************************************************************************
* SendMail:
* Sends a mail to the mailserver.
* 
* @param (String) receiver  - email address for the receiver of the mail
* @param (String) sender    - email address for the sender of the mail
* @param (String) subject   - The subject of the mail
* @param (String) message   - The body text of the mail
*
* @author Thor Richard Hansen
***********************************************************************************/
void EmailNetworkConnection::SendMail( String receiver, String sender, String subject, String message )
{
  m_Sender    = sender;
  m_Receiver  = receiver;
  m_Subject   = subject;
  m_Message   = message;

  if ( m_ExpectedCommand == e_WaitingForEmail )
  {
    SendCommand( String::FormatString("HELO %s", IpAddress().c_str() ) );
    m_ExpectedCommand = e_WaitingForHalo;
  }
  else
  {
    m_ExpectedCommand = e_WaitingForGreeting;
  }
}

/***********************************************************************************
* OnReceivedNetworkData:
* Virtual function where we receive the data from the network. This function will
* handle the data and break it up into commands from the server. All commands to
* and from the email server are strings.
* 
* @param (BinaryStream) data - The data we received from the network 
*
* @author Thor Richard Hansen
***********************************************************************************/
void EmailNetworkConnection::OnReceivedNetworkData( BinaryStream& data )
{
  // All messages are strings
  const char* commandStr = data.ReadRaw( data.GetSize() );

  String command( commandStr );
  int commandId = command.ToInt();

  LogInfoFMT("Email", "[receive] (code:%d) %s \n", commandId, command.c_str() );

  switch( m_ExpectedCommand )
  {
    case e_WaitingToConnect:
      m_ExpectedCommand = e_WaitingForEmail;
      break;

    case e_WaitingForGreeting:
      SendCommand( String::FormatString("HELO %s", IpAddress().c_str() ) );
      m_ExpectedCommand = e_WaitingForHalo;
      break;

    case e_WaitingForHalo:
      SendCommand( String::FormatString("MAIL FROM: %s", m_Sender.c_str() ) );
      m_ExpectedCommand = e_WaitingForMailFromReply;
      break;

    case e_WaitingForMailFromReply:
      SendCommand( String::FormatString("RCPT TO: %s", m_Receiver.c_str() ) );
      m_ExpectedCommand = e_WaitingForRcptToReply;
      break;

    case e_WaitingForRcptToReply:
      SendCommand("DATA");
      m_ExpectedCommand = e_WaitingForDataReply;
      break;

    case e_WaitingForDataReply:
      SendCommand( String::FormatString("Subject:%s\n%s\r\n.", m_Subject.c_str(), m_Message.c_str() ) );
      m_ExpectedCommand = e_WaitingForDataDone;
      break;

    case e_WaitingForDataDone:
      SendCommand("QUIT");
      m_ExpectedCommand = e_WaitingForQuit;
      break;

    case e_WaitingForEmail:
      break;

    case e_WaitingForQuit:
      Disconnect();
  }
}

/***********************************************************************************
* SendCommand:
* Send a command to the email server. All commands are ended with carrier return.
* 
* @param (String) command - The command we want to give to the server 
*
* @author Thor Richard Hansen
***********************************************************************************/
void EmailNetworkConnection::SendCommand( const String& command )
{
  BinaryStream* data = new BinaryStream();
  data->WriteRaw( command.c_str(), command.Length() );
  data->WriteRaw( "\r\n", 2 );
  SendData( data );

  LogInfoFMT("Email", "[send] %s \n",command.c_str() );
}
