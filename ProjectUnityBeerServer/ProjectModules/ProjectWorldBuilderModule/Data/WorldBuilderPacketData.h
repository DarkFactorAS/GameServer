#ifndef PROJECT_WORLDBUILDERMODULE_NETWORKPACKETS_WORLDBUILDERPACKETDATA
#define PROJECT_WORLDBUILDERMODULE_NETWORKPACKETS_WORLDBUILDERPACKETDATA 1
#pragma once

namespace WorldBuilderPacketData
{
  enum PacketData
  {
    // 400-499 World builder data
    PacketData_ServerPlayfieldList = 400,
    PacketData_ServerSavePlayfield = 401,
    PacketData_ServerLoadPlayfield = 402,
    PacketData_ServerDeletePlayfield = 403,

    PacketData_ClientPlayfieldError = 450,
    PacketData_ClientPlayfieldList = 451,
    PacketData_ClientLoadPlayfield = 452,
    PacketData_ClientPlayfieldSaved = 453,
    PacketData_ClientPlayfieldDeleted = 454,
  };

  enum PacketError
  {
    ErrorCode_NoError = 0,
    ErrorCode_CodeError = 400,
    ErrorCode_NotLoggedIn = 401,
    ErrorCode_PlayfieldNameMustBeUnique = 402,
    ErrorCode_FailedToLoadPlayfield = 403,
    ErrorCode_PlayfieldNotFound = 404,
    ErrorCode_FailedToDeletePlayfield = 405,
    ErrorCode_InvalidPlayfieldSize = 407,
    ErrorCode_PlayfieldnameTooShort = 408,
    ErrorCode_PlayfieldnameTooLong = 409,
  };

};

#endif /// PROJECT_WORLDBUILDERMODULE_NETWORKPACKETS_WORLDBUILDERPACKETDATA
