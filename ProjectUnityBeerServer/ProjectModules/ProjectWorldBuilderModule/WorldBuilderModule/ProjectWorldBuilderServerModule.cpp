
#include "PreCompile.h"
#include "ProjectWorldBuilderServerModule.h"

#include "EvilEngine/CoreLib/Utils/Base64Util.h"
#include "EvilEngine/CoreDatabase/CoreDatabase.h"

#include "ProjectWorldBuilderModule/Data/Playfield.h"
#include "ProjectWorldBuilderModule/Data/PlayfieldTile.h"
#include "ProjectWorldBuilderModule/Data/WorldBuilderPacketData.h"
#include "ProjectWorldBuilderModule/NetworkPackets/ServerNetworkPackets/ServerSendPlayfieldListNetworkPacket.h"
#include "ProjectWorldBuilderModule/NetworkPackets/ServerNetworkPackets/ServerSavePlayfieldNetworkPacket.h"
#include "ProjectWorldBuilderModule/NetworkPackets/ServerNetworkPackets/ServerLoadPlayfieldNetworkPacket.h"
#include "ProjectWorldBuilderModule/NetworkPackets/ServerNetworkPackets/ServerDeletePlayfieldNetworkPacket.h"

ProjectWorldBuilderServerModule::ProjectWorldBuilderServerModule() :
  CoreGameEngineModule(PROJECT_MODULETYPE_WORLDBUILDER)
{
  // Server account packets
  RegisterPacketType(WorldBuilderPacketData::PacketData_ServerPlayfieldList, ServerSendPlayfieldListNetworkPacket::Create);
  RegisterPacketType(WorldBuilderPacketData::PacketData_ServerSavePlayfield, ServerSavePlayfieldNetworkPacket::Create);
  RegisterPacketType(WorldBuilderPacketData::PacketData_ServerLoadPlayfield, ServerLoadPlayfieldNetworkPacket::Create);
  RegisterPacketType(WorldBuilderPacketData::PacketData_ServerDeletePlayfield, ServerDeletePlayfieldNetworkPacket::Create);

  AddLoginError(WorldBuilderPacketData::ErrorCode_CodeError, "Code Error");
  AddLoginError(WorldBuilderPacketData::ErrorCode_NotLoggedIn, "User not logged in");
  AddLoginError(WorldBuilderPacketData::ErrorCode_PlayfieldNameMustBeUnique, "Playfieldname must be unique");
  AddLoginError(WorldBuilderPacketData::ErrorCode_FailedToLoadPlayfield, "Failed to load playfield");
  AddLoginError(WorldBuilderPacketData::ErrorCode_PlayfieldNotFound, "Playfield not found");
  AddLoginError(WorldBuilderPacketData::ErrorCode_FailedToDeletePlayfield, "Failed to delete playfield");
  AddLoginError(WorldBuilderPacketData::ErrorCode_InvalidPlayfieldSize, "Invalid playfieldsize");
  AddLoginError(WorldBuilderPacketData::ErrorCode_PlayfieldnameTooShort, "Playfieldname too short");
  AddLoginError(WorldBuilderPacketData::ErrorCode_PlayfieldnameTooLong, "Playfieldname too long");
}

void ProjectWorldBuilderServerModule::LoadPlayfieldSQL(const String& sql, std::vector<Playfield*>& playfieldList)
{
  const SQLResultSet& result = CoreDatabase::GetInstance()->ExecuteSelect(sql);
  while (result.Next())
  {
    Playfield* playfield = new Playfield();
    playfield->SetPlayfieldId(result.GetUInt32());
    playfield->SetOwnerAccountId(result.GetUInt32());
    playfield->SetPlayfieldName(result.GetString());
    playfield->SetPlayfieldDescription(result.GetString());
    playfield->SetFlags(result.GetUInt32());
    playfield->SetNumPlayers(result.GetUInt8());
    playfield->SetNumGoals(result.GetUInt8());
    playfield->SetBoardSizeX(result.GetUInt8());
    playfield->SetBoardSizeY(result.GetUInt8());

    playfieldList.push_back(playfield);
  }
}

std::vector< Playfield* > ProjectWorldBuilderServerModule::LoadPlayfieldList(uint32 accountId, bool loadPublic)
{
  std::vector< Playfield* > playfieldList;

#if DATABASE
  CoreDatabase* database = CoreDatabase::GetInstance();
  if (database == NULL)
  {
    return playfieldList;
  }

  if ( accountId == 0 )
  {
    return playfieldList;
  }

  // Load playfields created by this account
  String sql = String::FormatString("select Id, OwnerAccountId, Name, Description, Flags, NumPlayers, NumGoals, BoardSizeX, BoardSizeY from playfield where OwnerAccountId = %d order by LastUpdated desc", accountId);
  LoadPlayfieldSQL(sql, playfieldList);

  // Load public playfields
  if ( loadPublic )
  {
    String sqlPublic = String("select Id, OwnerAccountId, Name, Description, Flags, NumPlayers, NumGoals, BoardSizeX, BoardSizeY from playfield where flags = 1 order by TimesPlayed desc, LastUpdated desc");
    LoadPlayfieldSQL(sqlPublic, playfieldList);
  }

#else
  VOLATILE_ARG(accountId);
  VOLATILE_ARG(loadPublic);
#endif

  return playfieldList;
}

void ProjectWorldBuilderServerModule::SetLastPlayfieldError(uint32 /*errorCode*/)
{

}

Playfield* ProjectWorldBuilderServerModule::LoadPlayfield(uint32 accountId, uint32 playfieldId)
{
  Playfield* playfield = NULL;
#ifdef DATABASE

  CoreDatabase* database = CoreDatabase::GetInstance();
  if (database == NULL)
  {
    SetLastPlayfieldError(WorldBuilderPacketData::ErrorCode_CodeError);
    return NULL;
  }

  String sql = "Select Id,OwnerAccountId, Name, Description, Flags, NumPlayers, NumGoals, BoardSizeX, BoardSizeY, Data from playfield where ( OwnerAccountId=%u or Flags = %u ) and Id = %u";
  const SQLResultSet& result = database->ExecuteSelect(sql, accountId, Playfield::PlayfieldFlag_Open, playfieldId);
  if (result.Next())
  {
    playfield = new Playfield();

    playfield->SetPlayfieldId( result.GetUInt32() );
    playfield->SetOwnerAccountId( result.GetUInt32() );
    playfield->SetPlayfieldName( result.GetString() );
    playfield->SetPlayfieldDescription( result.GetString() );
    playfield->SetFlags( result.GetUInt32() );
    playfield->SetNumPlayers( result.GetUInt8() );
    playfield->SetNumGoals( result.GetUInt8() );
    playfield->SetBoardSizeX( result.GetUInt8() );
    playfield->SetBoardSizeY(result.GetUInt8());

    // Data tiles from datablob
    String base64data = result.GetString();
    std::string unencodedData = Base64Util::Decode( base64data.c_str() );
    BinaryStream* binaryStreamData = new BinaryStream(unencodedData.c_str(), unencodedData.length() );
    for (uint32 x = 0; x < Playfield::PlayBoardXSize; x++ )
    {
      for (uint32 y = 0; y < Playfield::PlayBoardYSize; y++)
      {
        uint16 tileType = binaryStreamData->ReadUInt16();
        uint16 rotation = binaryStreamData->ReadUInt16();

        playfield->AddTile( new PlayfieldTile(tileType, rotation, new Vector2((float)x,(float)y)));
      }
    }

    if (!binaryStreamData->IsOK())
    {
      delete playfield;
      playfield = NULL;
    }
  }
#else
  VOLATILE_ARG(accountId);
  VOLATILE_ARG(playfieldId);
#endif

  return playfield;
}

uint32 ProjectWorldBuilderServerModule::VerifyPlayfieldData(Playfield* playfield)
{
  if (playfield == NULL)
  {
    return WorldBuilderPacketData::ErrorCode_CodeError;
  }

  // Sanitycheck playfield size - aka number of playfield tiles. Should "always be 12x16"
  if ( playfield->GetBoardSizeX() != Playfield::BoardSizes::PlayBoardSize || playfield->GetBoardSizeY() != (Playfield::BoardSizes::PlayBoardSize + Playfield::BoardSizes::StartBoardSize))
  {
    return WorldBuilderPacketData::ErrorCode_InvalidPlayfieldSize;
  }

  // - Minimum 3 letters max 30 letters for playfieldname
  if (playfield->GetPlayfieldName().length() < 3)
  {
    return WorldBuilderPacketData::ErrorCode_PlayfieldnameTooShort;
  }

  if (playfield->GetPlayfieldName().length() > 30)
  {
    return WorldBuilderPacketData::ErrorCode_PlayfieldnameTooLong;
  }
  
  return WorldBuilderPacketData::ErrorCode_NoError;
}

uint32 ProjectWorldBuilderServerModule::SavePlayfieldToDb(uint32 accountId, Playfield* playfield)
{
  uint32 errorCode = VerifyPlayfieldData(playfield);
  if (errorCode != WorldBuilderPacketData::ErrorCode_NoError)
  {
    return errorCode;
  }

#ifdef DATABASE

  CoreDatabase* database = CoreDatabase::GetInstance();
  if (database == NULL)
  {
    return WorldBuilderPacketData::ErrorCode_CodeError;
  }

  // Playfield must have unique name
  String dupeSql = "select count(*) as c from playfield where Name like '%s' and Id <> %d ";
  const SQLResultSet& result = database->ExecuteSelect(dupeSql, playfield->GetPlayfieldName().c_str(), playfield->GetPlayfieldId() );
  if (result.Next())
  {
    if (result.GetUInt32() > 0)
    {
      return WorldBuilderPacketData::ErrorCode_PlayfieldNameMustBeUnique;
    }
  }

  // Missing playfield description
  if (playfield->GetPlayfieldId() == 0)
  {
    // TODO : Change this to a store procedure to make sure to get correct Id back
    String sql = "Insert into playfield (OwnerAccountId, Name, Description, LastUpdated, TimesPlayed, Flags, NumPlayers, NumGoals, BoardSizeX, BoardSizeY, Data ) Values (%d, '%s', '%s', now(), 0, 0, %d, %d, %d, %d, NULL )";
    bool dbOk = database->ExecuteInsert(sql,
      accountId,
      playfield->GetPlayfieldName().c_str(),
      playfield->GetPlayfieldDescription().c_str(),
      playfield->GetNumPlayers(),
      playfield->GetNumGoals(),
      playfield->GetBoardSizeX(),
      playfield->GetBoardSizeY()
      );

    if (!dbOk)
    {
      return WorldBuilderPacketData::ErrorCode_CodeError;
    }

    // Get the ID back
    const SQLResultSet& resultLastId = database->ExecuteSelect("SELECT LAST_INSERT_ID()");
    if (resultLastId.Next())
    {
      playfield->SetPlayfieldId(resultLastId.GetUInt32());
    }
  }
  else
  {
    // TODO : Player must own this playfield

    String sql = "Update playfield set Name = '%s' where Id = %d and OwnerAccountId = %d ";
    database->ExecuteUpdate(sql, 
      playfield->GetPlayfieldName().c_str(),
      playfield->GetPlayfieldId(),
      accountId
    );
  }

  // Pack tiles into a datablob for db storage
  BinaryStream* binaryStream = new BinaryStream();
  uint xSize = playfield->GetBoardSizeX();
  uint ySize = playfield->GetBoardSizeY();
  for (uint16 x = 0; x < xSize; x++)
  {
    for (uint16 y = 0; y < ySize; y++)
    {
      PlayfieldTile* tile = playfield->GetTileAt(x, y);
      if (tile != NULL)
      {
        binaryStream->WriteUInt16(tile->m_TileType);
        binaryStream->WriteUInt16(tile->m_Rotation);
      }
      else
      {
        binaryStream->WriteUInt32(0);
      }
    }
  }

  // Update Blob in database
  std::string base64String = Base64Util::Encode((const unsigned char*)binaryStream->GetData(), binaryStream->GetSize());
  database->ExecuteBlobUpdate( String::FormatString("update playfield set Data=? where id = %d and OwnerAccountId = %d", playfield->GetPlayfieldId(), accountId), base64String.c_str() );
#else
  VOLATILE_ARG(accountId);
#endif

  return WorldBuilderPacketData::ErrorCode_NoError;
}

uint32 ProjectWorldBuilderServerModule::DeletePlayfield(uint32 accountId, uint32 playfieldId)
{
#ifdef DATABASE

  CoreDatabase* database = CoreDatabase::GetInstance();
  if (database == NULL)
  {
    return WorldBuilderPacketData::ErrorCode_CodeError;
  }

  int numUpdatedRows = database->ExecuteUpdate("Delete from playfield where OwnerAccountId = %d and Id = %d", accountId, playfieldId);
  if (numUpdatedRows == 0)
  {
    return WorldBuilderPacketData::ErrorCode_PlayfieldNotFound;
  }

#else
  VOLATILE_ARG(accountId);
  VOLATILE_ARG(playfieldId);
#endif

  return WorldBuilderPacketData::ErrorCode_NoError;
}

void ProjectWorldBuilderServerModule::AddLoginError(WorldBuilderPacketData::PacketError errorCode, const String& errorMessage)
{
  AddLoginError((uint32) errorCode, errorMessage );
}

void ProjectWorldBuilderServerModule::AddLoginError(uint32 errorCode, const String& errorMessage)
{
  // TODO > Check if the string is the same as well If it is different assert here!
  std::map< uint32, String >::iterator itFound = m_ErrorMessages.find(errorCode);
  if (itFound != m_ErrorMessages.end())
  {
    LogWarningFMT("NetworkBase::AddLoginError", "Error message [%d] is already registered (%s)", errorCode, errorMessage.c_str());
  }
  m_ErrorMessages[errorCode] = errorMessage;
}

const String& ProjectWorldBuilderServerModule::GetLoginError(uint32 errorCode)
{
  std::map< uint32, String >::iterator itFound = m_ErrorMessages.find(errorCode);
  if (itFound != m_ErrorMessages.end())
  {
    return itFound->second;
  }
  LogWarningFMT("NetworkBase::GetLoginError", "Error message [%d] not found in map", errorCode);
  return String::zero;
}
