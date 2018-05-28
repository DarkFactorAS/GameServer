#ifndef EVILENGINE_COREGAMELOGIN_NETWORKPACKETS_CLIENTACCOUNTERRORNETWORKPACKET
#define EVILENGINE_COREGAMELOGIN_NETWORKPACKETS_CLIENTACCOUNTERRORNETWORKPACKET 1

#include "EvilGameEngine/CoreGameEngine/CoreGameNetwork/Packets/BaseNetworkPacket.h"
#include "EvilGameEngine/CoreGameLogin/Network/GameLoginPacketData.h"

class Account;

class ClientAccountErrorNetworkPacket : public BaseNetworkPacket
{
public:

  ClientAccountErrorNetworkPacket( const BinaryStream* datastream);
  ClientAccountErrorNetworkPacket( uint32 networkPacketId, uint32 packetError, const String& errorMessage );
    
  static  BaseNetworkPacket*    Create( const BinaryStream* datastream ){ return new ClientAccountErrorNetworkPacket( datastream ); }

  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;
  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("ClientAccountError"); }
  virtual void                  Execute()         DF_OVERRIDE;
  
private:

  uint32                        m_PacketId;
  uint32                        m_PacketError;
  String                        m_ErrorMessage;

};

#endif /// EVILENGINE_COREGAMELOGIN_NETWORKPACKETS_CLIENTACCOUNTERRORNETWORKPACKET