#ifndef PROJECT_GAMEMANAMANGEMENT_DATA_NETWORKPACKETDATA
#define PROJECT_GAMEMANAMANGEMENT_DATA_NETWORKPACKETDATA 1
#pragma once

namespace GameEnginePacketData
{
  enum PacketData
  {
    // 300-399 Game management data
    PacketData_ServerCreateGame = 500,
    PacketData_ServerGameList = 501,
    PacketData_ServerJoinLobbyGame = 502,
    PacketData_ServerLeaveLobbyGame = 503,
    PacketData_ServerJoinQuickGame = 504,
    PacketData_ServerLeaveQuickGame = 505,

    PacketData_ClientGameError = 550,
    PacketData_ClientCreatedLobbyGame = 551,
    PacketData_ClientGameList = 552,
    PacketData_ClientPlayerJoinedLobbyGame = 553,
    PacketData_ClientPlayerLeftLobbyGame = 554,
    PacketData_ClientCreatedQuickGame = 555,
  };

  enum PacketError
  {
    ErrorCode_NoError = 0,
    ErrorCode_CodeError = 400,
    ErrorCode_NotLoggedIn = 401,
    ErrorCode_FailedToCreateGame = 402,
    ErrorCode_FailedToLoadGame = 403,
    ErrorCode_GameNotFound = 404,
    ErrorCode_FailedToDeleteGame = 405,
    ErrorCode_FailedToJoinGame = 406,
  };

};

#endif /// PROJECT_GAMEMANAMANGEMENT_DATA_NETWORKPACKETDATA
