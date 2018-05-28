/*************************************************************************************************
* Project     : EvilEngine Multi-platform 2014-2016
* File        : LoginAccountNetworkPacket
*
* Description : Network packet save playfield on the server
*
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "ServerLoadPlayfieldNetworkPacket.h"
//#include "EvilEngine/CoreLib/BasicTypes/Definitions/VolatileArgs.h"
//#include "EvilEngine/CoreVersion/version.h"

#include "ProjectWorldBuilderModule/Data/Playfield.h"
#include "ProjectWorldBuilderModule/Data/PlayfieldTile.h"

#include "ProjectWorldBuilderModule/Data/WorldBuilderPacketData.h"
#include "ProjectWorldBuilderModule/Data/Playfield.h"
#include "ProjectWorldBuilderModule/WorldBuilderModule/ProjectWorldBuilderServerModule.h"
#include "ProjectWorldBuilderModule/NetworkPackets/ClientNetworkPackets/ClientPlayfieldLoadedNetworkPacket.h"

ServerLoadPlayfieldNetworkPacket::ServerLoadPlayfieldNetworkPacket(uint32 playfieldId) :
  BaseWorldBuilderNetworkPacket(WorldBuilderPacketData::PacketData_ServerLoadPlayfield),
  m_PlayfieldId(playfieldId)
{
}

ServerLoadPlayfieldNetworkPacket::ServerLoadPlayfieldNetworkPacket(const BinaryStream* datastream) :
  BaseWorldBuilderNetworkPacket(WorldBuilderPacketData::PacketData_ServerLoadPlayfield, datastream)
{
  m_PlayfieldId = datastream->ReadUInt32();
}

BinaryStream* ServerLoadPlayfieldNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseWorldBuilderNetworkPacket::GetDataStream();
  datastream->WriteUInt32(m_PlayfieldId);
  return datastream;
}

void ServerLoadPlayfieldNetworkPacket::Execute()
{
  ProjectWorldBuilderServerModule* module = safe_cast<ProjectWorldBuilderServerModule*> (GetEngineModule(ProjectWorldBuilderServerModule::PROJECT_MODULETYPE_WORLDBUILDER));
  if (module != NULL)
  {
    Account* account = GetAccount();
    if (account != NULL)
    {
      Playfield* playfield = module->LoadPlayfield(account->GetAccountId(), m_PlayfieldId);
      if (playfield != NULL)
      {
        SendPacketToClient(new ClientPlayfieldLoadedNetworkPacket(playfield));
      }
      else
      {
        SendPlayfieldErrorToClient( WorldBuilderPacketData::ErrorCode_FailedToLoadPlayfield);
      }
    }
  }
}
