#pragma once

#include "EvilGameEngine/CoreGameLogin/Network/BaseAuthentcatedNetworkPacket.h"

class LoginGMNetworkPacket : public BaseAuthentcatedNetworkPacket
{
public:

  LoginGMNetworkPacket( const BinaryStream* datastream);
  LoginGMNetworkPacket( String password);
    
  static  BaseNetworkPacket*    Create( const BinaryStream* datastream ){ return new LoginGMNetworkPacket( datastream ); }

  virtual BinaryStream*         GetDataStream()   DF_OVERRIDE;
  virtual String                GetPacketName()   DF_OVERRIDE { return StaticStr("LoginGM"); }
  virtual void                  Execute()         DF_OVERRIDE;

private:

  String m_Password;

};
