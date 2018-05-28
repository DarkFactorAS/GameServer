/*************************************************************************************
* Project     : Evil Engine 2012
* File        : HttpNetworkClient
*
* Description : Network class that contains the logic to connect from a client to
*               a http/web server. This is used to fetch data from a webpage
*
* Author      : Thor Richard Hansen
*************************************************************************************/

#include "Precompile.h"
#include "HttpNetworkClient.h"
#include "EvilEngine/CoreLib/BasicTypes/BinaryStream/BinaryStream.h"
#include "EvilEngine/CoreNetwork/Common/NetworkConnection.h"

/***********************************************************************************
* EmailNetworkClient:
* Constructor for the EmailNetworkClient class
* 
* @param (String)         threadName - Debugname of the thread this is running in
* 
* @author Thor Richard Hansen
***********************************************************************************/
HttpNetworkClient::HttpNetworkClient( const String& threadName, const String& serverAddress, uint16 serverPort ) : 
  NetworkClient( threadName )
{
  AddNetworkHost( NetworkClient::NETWORKMODE_LIVE, serverAddress, String::FormatString("$d", serverPort ) );
}

void HttpNetworkClient::CancelNavigation()
{
  SignalReveivedData.DisconnectAll();
}

void HttpNetworkClient::NavigateTo( const String& url )
{
  BinaryStream* dataStream = new BinaryStream();
  dataStream->WriteString( String::FormatString( "GET %s HTTP/1.0\r\n", url.c_str() ) );
  dataStream->WriteString( String::FormatString( "HOST: %s\r\n", "127.0.0.1" ) );
  
  NetworkConnection* networkConnection = GetConnection();
  networkConnection->SendData( dataStream );
}
