/*************************************************************************************************
* Project     : EvilEngine Multi-platform 2014-2016
* File        : LoginAccountNetworkPacket
*
* Description : Network packet save playfield on the server
*
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "ServerDeletePlayfieldNetworkPacket.h"

#include "ProjectWorldBuilderModule/Data/Playfield.h"
#include "ProjectWorldBuilderModule/Data/PlayfieldTile.h"

#include "ProjectWorldBuilderModule/Data/WorldBuilderPacketData.h"
#include "ProjectWorldBuilderModule/Data/Playfield.h"
#include "ProjectWorldBuilderModule/WorldBuilderModule/ProjectWorldBuilderServerModule.h"
#include "ProjectWorldBuilderModule/NetworkPackets/ClientNetworkPackets/ClientPlayfieldDeletedNetworkPacket.h"

ServerDeletePlayfieldNetworkPacket::ServerDeletePlayfieldNetworkPacket(uint32 playfieldId) :
  BaseWorldBuilderNetworkPacket(WorldBuilderPacketData::PacketData_ServerDeletePlayfield),
  m_PlayfieldId(playfieldId)
{
}

ServerDeletePlayfieldNetworkPacket::ServerDeletePlayfieldNetworkPacket(const BinaryStream* datastream) :
  BaseWorldBuilderNetworkPacket(WorldBuilderPacketData::PacketData_ServerDeletePlayfield, datastream)
{
  m_PlayfieldId = datastream->ReadUInt32();
}

BinaryStream* ServerDeletePlayfieldNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseWorldBuilderNetworkPacket::GetDataStream();
  datastream->WriteUInt32(m_PlayfieldId);
  return datastream;
}

void ServerDeletePlayfieldNetworkPacket::Execute()
{
  ProjectWorldBuilderServerModule* module = safe_cast<ProjectWorldBuilderServerModule*> (GetEngineModule(ProjectWorldBuilderServerModule::PROJECT_MODULETYPE_WORLDBUILDER));
  if (module != NULL)
  {
    Account* account = GetAccount();
    if (account != NULL)
    {
      Playfield* playfield = module->LoadPlayfield( account->GetAccountId(), m_PlayfieldId);
      if (playfield == NULL)
      {
        SendPlayfieldErrorToClient(WorldBuilderPacketData::ErrorCode_PlayfieldNotFound);
        return;
      }

      uint32 errorCode = module->DeletePlayfield(account->GetAccountId(), m_PlayfieldId);
      if ( errorCode == 0 )
      {
        SendPacketToClient(new ClientPlayfieldDeletedNetworkPacket(Playfield::SourceType_Network, m_PlayfieldId, playfield->GetPlayfieldName()));
      }
      else
      {
        SendPlayfieldErrorToClient( WorldBuilderPacketData::ErrorCode_FailedToDeletePlayfield);
      }
    }
  }
}
