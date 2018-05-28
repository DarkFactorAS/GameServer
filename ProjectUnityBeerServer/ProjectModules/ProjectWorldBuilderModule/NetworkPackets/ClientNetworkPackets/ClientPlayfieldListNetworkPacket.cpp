/*************************************************************************************************
* Project     : EvilEngine Multi-platform 2014-2016
* File        : LoginAccountNetworkPacket
*
* Description : Network packet to log in the player on the server. Most of the game related
*               packets require the player to be logged on to the server.
*
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "ClientPlayfieldListNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/Network/BaseAuthentcatedNetworkPacket.h"
#include "ProjectWorldBuilderModule/Data/WorldBuilderPacketData.h"
#include "ProjectWorldBuilderModule/Data/Playfield.h"

ClientPlayfieldListNetworkPacket::ClientPlayfieldListNetworkPacket(const BinaryStream* datastream) :
  BaseNetworkPacket(WorldBuilderPacketData::PacketData_ClientPlayfieldList, datastream)
{
}

ClientPlayfieldListNetworkPacket::ClientPlayfieldListNetworkPacket(std::vector< Playfield* > playfieldList) :
  BaseNetworkPacket(WorldBuilderPacketData::PacketData_ClientPlayfieldList),
  m_PlayfieldList(playfieldList)
{
}

BinaryStream* ClientPlayfieldListNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseNetworkPacket::GetDataStream();

  datastream->WriteUInt16( (uint16) m_PlayfieldList.size() );
  for (uint32 index = 0; index < m_PlayfieldList.size(); index++)
  {
    Playfield* playfield = m_PlayfieldList[index];
    playfield->CreateBinaryStream( datastream, false );
  }
 
  return datastream;
}

void ClientPlayfieldListNetworkPacket::Execute()
{
}
