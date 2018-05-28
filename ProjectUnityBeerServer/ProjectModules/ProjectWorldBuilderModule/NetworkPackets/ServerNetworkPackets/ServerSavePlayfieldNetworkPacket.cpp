/*************************************************************************************************
* Project     : EvilEngine Multi-platform 2014-2016
* File        : LoginAccountNetworkPacket
*
* Description : Network packet save playfield on the server
*
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "ServerSavePlayfieldNetworkPacket.h"
//#include "EvilEngine/CoreLib/BasicTypes/Definitions/VolatileArgs.h"
//#include "EvilEngine/CoreVersion/version.h"

#include "ProjectWorldBuilderModule/Data/Playfield.h"
#include "ProjectWorldBuilderModule/Data/PlayfieldTile.h"

#include "ProjectWorldBuilderModule/Data/WorldBuilderPacketData.h"
#include "ProjectWorldBuilderModule/Data/Playfield.h"
#include "ProjectWorldBuilderModule/WorldBuilderModule/ProjectWorldBuilderServerModule.h"
#include "ProjectWorldBuilderModule/NetworkPackets/ClientNetworkPackets/ClientPlayfieldSavedNetworkPacket.h"

ServerSavePlayfieldNetworkPacket::ServerSavePlayfieldNetworkPacket(Playfield* playfield) :
  BaseWorldBuilderNetworkPacket(WorldBuilderPacketData::PacketData_ServerSavePlayfield),
  m_Playfield(playfield)
{
}

ServerSavePlayfieldNetworkPacket::ServerSavePlayfieldNetworkPacket(const BinaryStream* datastream) :
  BaseWorldBuilderNetworkPacket(WorldBuilderPacketData::PacketData_ServerSavePlayfield, datastream)
{
  uint32 blobSize = datastream->ReadUInt32();
  m_Data = new BinaryStream(datastream, blobSize);
  m_Playfield = Playfield::CreatePlayfieldFromStream(datastream);
}

BinaryStream* ServerSavePlayfieldNetworkPacket::GetDataStream()
{
  BinaryStream* datastream = BaseWorldBuilderNetworkPacket::GetDataStream();
  BinaryStream* playfieldData = m_Playfield->CreateBinaryStream( new BinaryStream(), true);
  datastream->WriteUInt32( playfieldData->GetSize() );
  datastream->WriteRaw( playfieldData );
  delete playfieldData;

  return datastream;
}

void ServerSavePlayfieldNetworkPacket::Execute()
{
  ProjectWorldBuilderServerModule* module = safe_cast<ProjectWorldBuilderServerModule*> (GetEngineModule(ProjectWorldBuilderServerModule::PROJECT_MODULETYPE_WORLDBUILDER));
  if (module != NULL)
  {
    uint32 errorCode = WorldBuilderPacketData::ErrorCode_NotLoggedIn;

    Account* account = GetAccount();
    if (account != NULL)
    {
      errorCode = module->SavePlayfieldToDb(account->GetAccountId(), m_Playfield);
    }

    if (errorCode == 0)
    {
      SendPacketToClient(new ClientPlayfieldSavedNetworkPacket( Playfield::SourceType_Network, m_Playfield->GetPlayfieldId(), m_Playfield->GetPlayfieldToken()));
    }
    else
    {
      SendPlayfieldErrorToClient(errorCode);
    }
  }
}
