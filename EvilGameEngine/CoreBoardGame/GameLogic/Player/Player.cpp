/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : Player
*
* Description : One player in a specific game. This is the 3D representation of a player in the 
*               game world. 
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "Player.h"
#include "EvilEngine/CoreLib/BasicTypes/BinaryStream/BinaryStream.h"

#if defined CLIENT
  #include "EvilEngine/CoreRendering/RenderObjects/CubeRenderObject.h"
  #include "EvilEngine/CoreRendering/RenderObjects/HexagonRenderObject.h"
  #include "EvilEngine/CoreRendering/Camera/RenderCamera.h"
#endif

/// Cannot use Color:Green directly since that can be initalized after this
Color         Player::Player1Color  = Color( 0.1f, 0.35f, 0.1f );  // Dark green
Color         Player::Player2Color  = Color(0.0f, 0.17f, 0.53f );   // Blue
//Color         Player::Player2Color  = Color(0x08004C);   // Purple
Vector3       Player::PlayerSize    = Vector3( 0.04f, 0.04f, 0.025f );
const float   Player::Jumpspeed     = 0.5f;
const float   Player::Teleportspeed = 1.0f;
const float   Player::JumpWaitspeed = 0.1f;

Player::Player() : 
  Character(),
  m_AccountId( 0 ),
  m_IsPlayer( false ),
  m_IsClient( false ),
  m_PlayerFlags( 0 ),
  m_PlayerGuiFlags( 0 )
{
}

Player::Player( String playerName, uint32 characterType, uint32 iconId, const Color& color ) :
  Character( playerName, characterType, iconId, color ),
  m_AccountId( 0 ),
  m_IsPlayer( false ),
  m_IsClient( false ),
  m_PlayerFlags( 0 ), 
  m_PlayerGuiFlags( 0 )
{
}

Player::Player( String playerName, uint32 characterType, uint32 /*iconId*/, const Color& color, Vector3 position, Vector3 size, uint32 resourceId ) :
  Character( playerName, characterType, resourceId, color, position, size ),
  m_AccountId( 0 ),
  m_IsPlayer( false ),
  m_IsClient( false ),
  m_PlayerFlags( 0 ),
  m_PlayerGuiFlags( 0 )
{
//#if defined CLIENT
//  m_RenderObject->SetResourceId( resourceId );
//#else
  VOLATILE_ARG( resourceId );
//#endif
}

/************************************************************************************************
* CreateFromData:
* Read the data from a binary blob and then create an instance of Player with that data.
*
* @param  (BinaryStream)   data       - The binarystream we want to write to
* @return (Player*)                    - The newly created Player with the blob data
*
* @author Thor Richard Hansen
*************************************************************************************************/
Player* Player::CreateFromData( const BinaryStream* data )
{
  Player* player = new Player();
  player->ReadData( data );
  player->SetSize( PlayerSize );
  return player;
}

/************************************************************************************************
* ReadData:
* Read the data from a binary blob and initialize all members with it
*
* @param  (BinaryStream)   data       - The binarystream we want to write to
*
* @author Thor Richard Hansen
*************************************************************************************************/
void Player::ReadData( const BinaryStream* data )
{
  Character::ReadData( data );
  m_AccountId   = data->ReadUInt32();
  m_PlayerFlags = data->ReadUInt32();
  m_PlayerGuiFlags = m_PlayerFlags;
}

/************************************************************************************************
* WriteData:
* Write the data to a binary blob. This binary blob is used to pass data on the network.
*
* @param  (BinaryStream)   data        - The binarystream we want to write to
*
* @author Thor Richard Hansen
*************************************************************************************************/
void Player::WriteData( BinaryStream& data ) const
{
  Character::WriteData( data );
  data.WriteUInt32( m_AccountId ); 
  data.WriteUInt32( m_PlayerFlags );
}
