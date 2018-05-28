/*************************************************************************************
* Project     : Evil Engine 2012
* File        : EmailNetworkClient
*
* Description : Network class that contains the logic to connect from a client to
*               a mail server. This is used to send an email from code.
*
* Author      : Thor Richard Hansen
*************************************************************************************/

#include "Precompile.h"
#include "EmailNetworkClient.h"
#include "EmailNetworkConnection.h"

/***********************************************************************************
* EmailNetworkClient:
* Constructor for the EmailNetworkClient class
* 
* @param (String)         threadName - Debugname of the thread this is running in
* 
* @author Thor Richard Hansen
***********************************************************************************/
EmailNetworkClient::EmailNetworkClient( const String& threadName, const String& serverAddress, uint16 serverPort ) : 
  NetworkClient( threadName )
{
  AddNetworkHost( NetworkClient::NETWORKMODE_LIVE, serverAddress, String::FormatString("$d", serverPort ) );
}

/***********************************************************************************
* SendEmail:
* Initiates sending an email with a specific receiver, sender and subject.
* 
* @param (String) receiver  - Email address of the receiver ( like mailme@google.com )
* @param (String) sender    - Email address of the sender ( like automail@evilengine.com )
* @param (String) subject   - Subject
* @param (String) message   - What the mail is about.
* 
* @author Thor Richard Hansen
***********************************************************************************/
void EmailNetworkClient::SendEmail( String receiver, String sender, String subject, String message )
{
  if ( m_Connection == NULL )
  {
    m_Connection = CreateConnectionObject();
  }

  EmailNetworkConnection* connection = static_cast< EmailNetworkConnection* > ( m_Connection );
  if ( connection != NULL )
  {
    connection->SendMail(receiver, sender, subject, message );
  }
}

/***********************************************************************************
* CreateConnectionObject:
* Virtual function that returns the connection object to use during the connection
* to the server. This overload returns the email version of the connection object.
* 
* @return (CreateConnectionObject) - The connection object to use
* 
* @author Thor Richard Hansen
***********************************************************************************/
NetworkConnection* EmailNetworkClient::CreateConnectionObject()
{
  m_Connection = new EmailNetworkConnection( this );
  return m_Connection;
}
