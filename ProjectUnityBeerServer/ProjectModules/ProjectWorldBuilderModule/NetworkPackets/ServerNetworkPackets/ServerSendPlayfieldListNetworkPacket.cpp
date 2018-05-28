/*************************************************************************************************
* Project     : EvilEngine Multi-platform 2014-2016
* File        : LoginAccountNetworkPacket
*
* Description : Network packet save playfield on the server
*
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "ServerSendPlayfieldListNetworkPacket.h"
//#include "EvilEngine/CoreLib/BasicTypes/Definitions/VolatileArgs.h"
//#include "EvilEngine/CoreVersion/version.h"

#include "ProjectWorldBuilderModule/Data/Playfield.h"
#include "ProjectWorldBuilderModule/Data/PlayfieldTile.h"

#include "ProjectWorldBuilderModule/Data/WorldBuilderPacketData.h"
#include "ProjectWorldBuilderModule/Data/Playfield.h"
#include "ProjectWorldBuilderModule/WorldBuilderModule/ProjectWorldBuilderServerModule.h"
#include "ProjectWorldBuilderModule/NetworkPackets/ClientNetworkPackets/ClientPlayfieldListNetworkPacket.h"

ServerSendPlayfieldListNetworkPacket::ServerSendPlayfieldListNetworkPacket(const BinaryStream* datastream) :
  BaseWorldBuilderNetworkPacket(WorldBuilderPacketData::PacketData_ServerPlayfieldList, datastream)
{
  m_Flags = datastream->ReadUInt32();
}

void ServerSendPlayfieldListNetworkPacket::Execute()
{
  ProjectWorldBuilderServerModule* module = safe_cast<ProjectWorldBuilderServerModule*> (GetEngineModule(ProjectWorldBuilderServerModule::PROJECT_MODULETYPE_WORLDBUILDER));
  if (module != NULL)
  {
    Account* account = GetAccount();
    if (account != NULL)
    {
      bool loadPublic = (m_Flags & Playfield::PlayfieldFlag_Open ) == Playfield::PlayfieldFlag_Open;
      std::vector< Playfield* > playfieldList = module->LoadPlayfieldList(account->GetAccountId(), loadPublic);
      SendPacketToClient(new ClientPlayfieldListNetworkPacket(playfieldList));
      return;
    }
  }

  // Send error!
}
