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
#include "GameObject.h"
#include "EvilEngine/CoreLib/BasicTypes/BinaryStream/BinaryStream.h"
#include "EvilGameEngine/CoreBoardGame/Playfield/GridTilePlayfield.h"

#if defined CLIENT
  #include "EvilEngine/CoreRendering/RenderObjects/CubeRenderObject.h"
  #include "EvilEngine/CoreRendering/RenderObjects/HexagonRenderObject.h"
  #include "EvilEngine/CoreRendering/Camera/RenderCamera.h"
  #include "EvilEngine/CoreGuiToolkit/CoreGuiResources.h"
#endif

Vector3 GameObject::GameObjectSize   = Vector3( 0.04f, 0.04f, 0.025f );
uint32 GameObject::s_Id = 0;

GameObject::GameObject() : 
  CoreBaseRenderObject(),
  m_Id( 0 ),
  m_Type( 0 ),
  m_IconId( 0 ),
  m_IsDiscovered( 0 ),
  m_Playfield( NULL )
{
}

GameObject::GameObject( uint32 id, uint32 type, String name, uint32 iconId, const Color& color ) :
  CoreBaseRenderObject( Vector3::s_Vec3Zero, GameObjectSize, iconId ),
  m_Id( id ),
  m_Type( type ),
  m_Name( name ),
  m_IconId( iconId ),
  m_Color( color ),
  m_IsDiscovered( 0 ),
  m_Playfield( NULL )
{
  if ( m_Id == 0 )
  {
    m_Id = ++s_Id;
  }

#if defined CLIENT
  HexagonRenderObject* hexRender = new HexagonRenderObject( Vector3::s_Vec3Zero, GameObjectSize, iconId, GuiResource_White );
  hexRender->SetWallColor( color );
  m_RenderObject->AddRenderObject( 0, hexRender );
#endif
}

GameObject::GameObject( uint32 id, uint32 type, String name, uint32 iconId, const Color& color, const Vector3& position, const Vector3& size ) :
  CoreBaseRenderObject( position, size, iconId ),
  m_Id( id ),
  m_Type( type ),
  m_Name( name ),
  m_IconId( iconId ),
  m_Color( color ),
  m_IsDiscovered( 0 ),
  m_Playfield( NULL )
{
  if ( m_Id == 0 )
  {
    m_Id = ++s_Id;
  }
  CreateRenderObject();
}

GameObject::GameObject( const GameObject* otherObj ) :
  CoreBaseRenderObject( otherObj->GetPosition(), otherObj->GetSize(), otherObj->GetIconId() ),
  m_Id( ++s_Id ),
  m_Type( otherObj->GetType() ),
  m_Name( otherObj->GetName() ),
  m_IconId( otherObj->GetIconId() ),
  m_Color( otherObj->GetColor() ),
  m_IsDiscovered( 0 ),
  m_Playfield( NULL )
{
  if ( m_Id == 0 )
  {
    m_Id = ++s_Id;
  }
  CreateRenderObject();
}

GameObject::~GameObject()
{
}

void GameObject::SetIconId(uint32 iconId) 
{ 
  m_IconId = iconId; 
  SetResourceId(iconId);
}



void GameObject::CreateRenderObject()
{
#if defined CLIENT
  HexagonRenderObject* hexRender = new HexagonRenderObject( m_Position, m_Size, m_IconId, GuiResource_White );
  hexRender->SetWallColor( m_Color );
  m_RenderObject->AddRenderObject( 0, hexRender );
#endif
}

GameObject* GameObject::Clone() const
{
  return new GameObject( this );
}

/************************************************************************************************
* MoveCameraToMe:
* Move the camera instantly to look at the player instantly
*
* @author Thor Richard Hansen
*************************************************************************************************/
void GameObject::MoveCameraToMe() const
{
#if defined CLIENT
  RenderCamera* camera = RenderCamera::GetInstance();
  if ( camera != NULL )
  {
    RenderCamera::GetInstance()->MoveToPosition( GetPosition() );
  }
#endif
}

/************************************************************************************************
* MoveCameraToMe:
* Move the camera instantly to look at the player instantly
*
* @author Thor Richard Hansen
*************************************************************************************************/
void GameObject::SlideCameraToMe() const
{
#if defined CLIENT
  RenderCamera* camera = RenderCamera::GetInstance();
  if ( camera != NULL )
  {
    RenderCamera::GetInstance()->SlideToPosition( GetPosition() );
  }
#endif
}

/************************************************************************************************
* ReadData:
* Read the data from a binary blob and initialize all members with it
*
* @param  (BinaryStream)   data       - The binarystream we want to write to
*
* @author Thor Richard Hansen
*************************************************************************************************/
void GameObject::ReadData( const BinaryStream* data )
{
  m_Id              = data->ReadUInt32();
  m_Name            = data->ReadString();
  m_IconId          = data->ReadUInt32();
  uint32 color      = data->ReadUInt32();
  m_Color           = Color(color);
  uint16 gridPosX   = data->ReadUInt16();
  uint16 gridPosY   = data->ReadUInt16();
  m_GridPosition.SetUInt16( gridPosX, gridPosY );
}

/************************************************************************************************
* WriteData:
* Write the data to a binary blob. This binary blob is used to pass data on the network.
*
* @param  (BinaryStream)   data        - The binarystream we want to write to
*
* @author Thor Richard Hansen
*************************************************************************************************/
void GameObject::WriteData( BinaryStream& data ) const
{
  data.WriteUInt32( m_Id ); 
  data.WriteString( m_Name );
  data.WriteUInt32( m_IconId );
  data.WriteUInt32( m_Color.ToUInt32() );
  data.WriteUInt16( (uint16)m_GridPosition.GetIntX() );
  data.WriteUInt16( (uint16)m_GridPosition.GetIntY() );
}
