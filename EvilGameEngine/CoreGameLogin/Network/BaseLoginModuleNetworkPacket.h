#ifndef EVILENGINE_COREGAMELOGIN_NETWORK_BASELOGINNETWORKPACKET
#define EVILENGINE_COREGAMELOGIN_NETWORK_BASELOGINNETWORKPACKET 1

#include "EvilEngine/CoreLib/BasicTypes/BinaryStream/BinaryStream.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameEngine.h"
#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/Packets/BaseNetworkPacket.h"

class BaseLoginModuleNetworkPacket : public BaseNetworkPacket
{
public:

  BaseLoginModuleNetworkPacket( uint32 packetType, const BinaryStream* datastream );
  BaseLoginModuleNetworkPacket( uint32 packetType );
    
  virtual String                GetPacketName() = 0;
  void                          SendLoginErrorToClient(uint32 packetTypeId, uint32 errorId);
};

#endif /// EVILENGINE_COREGAMELOGIN_NETWORK_BASELOGINNETWORKPACKET