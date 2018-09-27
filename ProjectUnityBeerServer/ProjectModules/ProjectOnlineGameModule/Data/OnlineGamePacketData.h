#ifndef PROJECT_ONLINEGAME_DATA_NETWORKPACKETDATA
#define PROJECT_ONLINEGAME_DATA_NETWORKPACKETDATA 1
#pragma once

namespace OnlineGamePacketData
{
  enum PacketData
  {
    // 300-399 Game management data
    PacketData_ServerCreateOnlineGame = 600,
    PacketData_ServerRequestGameData = 601,
    PacketData_ServerLeaveOnlineGame = 602,
    PacketData_ServerPlayerStartRound = 603,

    PacketData_ServerGMStartRound = 623,

    PacketData_ClientCreatedOnlineGame = 650,
    PacketData_ClientReceiveGameData = 651,
    PacketData_ClientReceiveActionCards = 652,
  };

  enum PacketError
  {
    ErrorCode_NoError = 0,
    ErrorCode_CodeError = 400,
    ErrorCode_NotLoggedIn = 401,
  };

};

#endif /// PROJECT_ONLINEGAME_DATA_NETWORKPACKETDATA
