#ifndef EVILENGINE_COREGAMEENGINE_COREGAMENETWORK_GAMEENGINEPACKETDATA
#define EVILENGINE_COREGAMEENGINE_COREGAMENETWORK_GAMEENGINEPACKETDATA 1

namespace GameEnginePacketData
{
  enum EngineGameData
  {
    // System packets
    PacketData_RequestServerVersion   = 1,
    PacketData_ErrorMessage           = 2,
    PacketData_ReceiveServerVersion   = 3,
  };

  enum EngineGamePacketError
  {
    // 0-99 System errors
    ErrorCode_OK                      = 0,
    ErrorCode_Version_ServerIsTooOld  = 1,
    ErrorCode_Version_ClientIsTooOld  = 2,
    ErrorCode_WrongGameKey            = 3,
    ErrorCode_ConnectionFailed        = 5,
    ErrorCode_CodeError               = 6,
    ErrorCode_DatabaseOffline         = 7,
  };
}

#endif /// EVILENGINE_COREGAMEENGINE_COREGAMENETWORK_GAMEENGINEPACKETDATA
