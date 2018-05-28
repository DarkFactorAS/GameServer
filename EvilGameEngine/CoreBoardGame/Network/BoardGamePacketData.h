#ifndef EVILENGINE_COREBOARDGAME_NETWORK_PacketData
#define EVILENGINE_COREBOARDGAME_NETWORK_PacketData 1
#pragma once

namespace BoardGamePacketData
{
  enum PacketData
  {
    // 130-139 Game management
    PacketData_GameList                 = 130,
    PacketData_GameDelete               = 131,
    PacketData_GameCreate               = 132,
    PacketData_PlayfieldInfo            = 133,
    PacketData_GameCreateWithRandomPlayer = 134,
    PacketData_SearchPlayer             = 137,
    PacketData_GameCreateWithPlayer     = 138,

    // 140-169 Game specific packets
    PacketData_GameData                 = 141,
    PacketData_GameEndTurn              = 142,
    PacketData_GamePlaceTile            = 143,
    PacketData_GameMovePlayer           = 144,
    PacketData_GameActivePlayer         = 145,
    PacketData_ChangeGamePhase          = 146,
    PacketData_GameMoveTile             = 147,
    PacketData_GamePlaceObjective       = 150,
    PacketData_GamePickupObjective      = 151,
    PacketData_GamePlayerWon            = 152,
    PacketData_GameFinish               = 153,
    //PacketData_GameLeave                = 154,
    PacketData_GameStartEvent           = 155,
    PacketData_GameSwapEvent            = 156,
    PacketData_GameHandleEvent          = 157,
    PacketData_GameRollOnMonster        = 158,

    // 170-179 Editor
    PacketData_PlayfieldList            = 170,
    PacketData_PlayfieldLoad            = 171,
    PacketData_PlayfieldSave            = 172,
    PacketData_PlayfieldDelete          = 173,
  };

  enum ErrorCode
  {
    ErrorCode_CodeError                 = 100,

    ErrorCode_ErrorWithPlayfield        = 101,
    ErrorCode_ErrorWithMissionId        = 102,
    ErrorCode_ErrorWithSavingGame       = 103,

    ErrorCode_UnknownGame               = 110,
    ErrorCode_NotActivePlayer           = 111,
    ErrorCode_CouldNotsaveGame          = 112,
    ErrorCode_CouldNotPlaceTile         = 113,
    ErrorCode_InvalidDeckTile           = 114,
    ErrorCode_CouldNotCreateGame        = 115,
  };
};

#endif /// EVILENGINE_COREBOARDGAME_NETWORK_PacketData
