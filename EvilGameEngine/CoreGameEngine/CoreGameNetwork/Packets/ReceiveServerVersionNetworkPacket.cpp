/*************************************************************************************
* Project     : Evil Engine 2012 - 2016
* File        : ReceivedServerVersionNetworkPacket
*
* Description : Recieve the version of the server
*
* Author      : Thor Richard Hansen
*************************************************************************************/

#include "Precompile.h"
#include "ReceiveServerVersionNetworkPacket.h"
#include "EvilEngine/CoreVersion/version.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/GameEnginePacketData.h"

ReceivedServerVersionNetworkPacket::ReceivedServerVersionNetworkPacket( const BinaryStream* datastream) :
  BaseNetworkPacket( GameEnginePacketData::PacketData_ReceiveServerVersion, datastream )
{
  m_BuildVersion = datastream->ReadUInt32();
  m_BuildDate    = datastream->ReadString();
}

ReceivedServerVersionNetworkPacket::ReceivedServerVersionNetworkPacket( uint32 buildVersion, String buildDate ) :
  BaseNetworkPacket(GameEnginePacketData::PacketData_ReceiveServerVersion),
  m_BuildVersion( buildVersion ),
  m_BuildDate( buildDate )
{
}

BinaryStream* ReceivedServerVersionNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseNetworkPacket::GetDataStream();
  datastream->WriteUInt32( m_BuildVersion );
  datastream->WriteString( m_BuildDate );
  return datastream;
}

void ReceivedServerVersionNetworkPacket::Execute()
{
  //s_ServerVersionTxt = data->ReadString();
  //if (m_VersionTxt != NULL)
  //{
  //  ShowVersionText();
  //}


  //const char* ch = BUILD_DATE;
  //uint32 num = BUILD_REVISION;
  //String buildVersion = String::FormatString("%s - %d", ch, num);
}
