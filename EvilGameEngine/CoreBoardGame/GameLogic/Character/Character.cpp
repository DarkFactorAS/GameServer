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
#include "Character.h"
#include "EvilEngine/CoreLib/BasicTypes/BinaryStream/BinaryStream.h"

uint32 Character::s_CharacterId = 1000000;

Character::Character() : 
  GameObject(),
  m_WalkDistance( 0 ),
  m_CharacterId( ++s_CharacterId ),
  m_CharacterType( 0 ),
  m_Opponent( 0 )
{
}

Character::Character( String characterName, uint32 characterType, uint32 characterIconId, const Color& color ) :
  GameObject( ++s_Id, e_Character, characterName, characterIconId, color ),
  m_WalkDistance( 0 ),
  m_CharacterId( ++s_CharacterId ),
  m_CharacterType( characterType ),
  m_Opponent( 0 )
{
}

Character::Character( String characterName, uint32 characterType, uint32 characterIconId, const Color& color, const Vector3& position, const Vector3& size ) :
  GameObject( ++s_Id, e_Character, characterName, characterIconId, color, position, size ),
  m_WalkDistance( 0 ),
  m_CharacterId( ++s_CharacterId ),
  m_CharacterType( characterType ),
  m_Opponent( 0 )
{
}

Character::Character( const Character* other ) :
  GameObject( other ),
  m_WalkDistance( other->m_WalkDistance ),
  m_CharacterId( ++s_CharacterId ),
  m_CharacterType( other->m_CharacterType ),
  m_Opponent( other->m_Opponent )
{
}

GameObject* Character::Clone() const
{
  return new Character( this );
}

/************************************************************************************************
* ReadData:
* Read the data from a binary blob and initialize all members with it
*
* @param  (BinaryStream)   data       - The binarystream we want to write to
*
* @author Thor Richard Hansen
*************************************************************************************************/
void Character::ReadData( const BinaryStream* data )
{
  GameObject::ReadData( data );
  m_CharacterId = data->ReadUInt32();
}

/************************************************************************************************
* WriteData:
* Write the data to a binary blob. This binary blob is used to pass data on the network.
*
* @param  (BinaryStream)   data        - The binarystream we want to write to
*
* @author Thor Richard Hansen
*************************************************************************************************/
void Character::WriteData( BinaryStream& data ) const
{
  GameObject::WriteData( data );
  data.WriteUInt32( m_CharacterId );
}
