/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : CoreGuiImageView
* Description : Creates the normal quad to render a texture on. Mainly used for background and
*               clean surfaces
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/
#include "PreCompile.h"
#include "CoreGuiImageView.h"
#include "../../CoreRendering/RenderObjects/QuadRenderObject.h"

CoreGuiImageView::CoreGuiImageView( const Vector2& position, const Vector2& size, const Color& color ) :
  CoreGuiBase( "CoreGuiImageView", position, size )
{
  m_ImageRender = new QuadRenderObject( Vector3(position), Vector3(size), GuiResource_White );
  m_ImageRender->SetColor( color );
  AddRenderObject( m_ImageRender );
}

CoreGuiImageView::CoreGuiImageView( const Vector2& position, const Vector2& size, uint32 resourceId ) :
  CoreGuiBase( "CoreGuiImageView", position, size, resourceId )
{
  m_ImageRender = new QuadRenderObject( Vector3(position), Vector3(size), resourceId );
  AddRenderObject( m_ImageRender );
}

CoreGuiImageView::CoreGuiImageView( const String& objectName, const Vector2& position, const Vector2& size, uint32 resourceId ) :
  CoreGuiBase( objectName, position, size, resourceId )
{
  m_ImageRender = new QuadRenderObject( Vector3(position), Vector3(size), resourceId );
  AddRenderObject( m_ImageRender );
}

//CoreGuiImageView::~CoreGuiImageView()
//{
//}

void CoreGuiImageView::SetResourceId( uint32 resourceId )
{
  CoreGuiBase::SetResourceId( resourceId );
  m_ImageRender->SetResourceId( resourceId );
}

void CoreGuiImageView::SetPosition( float x, float y )
{
  CoreGuiBase::SetPosition( x, y );
  m_ImageRender->SetPosition( x, y );
  //m_ImageRender->SetPosition( x, y, ( GetLayer() + 1 ) * LAYER_SIZE_Z );
}

void CoreGuiImageView::SetSize( float x, float y )
{
  CoreGuiBase::SetSize( x, y );
  m_ImageRender->SetSize( x, y, 0 );
}

void CoreGuiImageView::SetColor( float red, float green, float blue )
{
  CoreGuiBase::SetColor( red, green, blue );
  m_ImageRender->SetColor( red, green, blue );
}

void CoreGuiImageView::SetAlpha( float alpha )
{
  CoreGuiBase::SetAlpha( alpha );
  m_ImageRender->SetAlpha( alpha );
}

void CoreGuiImageView::SetWireframe( bool isWireframe )
{
  CoreGuiBase::SetWireframe( isWireframe );
  m_ImageRender->SetWireframe( isWireframe );
}

void CoreGuiImageView::Move( float deltaX, float deltaY, bool isLocal )
{
  if ( ( deltaX != 0.0f || deltaY != 0.0f ) && ( !isLocal || !HasFlag( FLAGS_NOTSCALECHILD ) ) )
  {
    //m_ImageRender->SetPosition( m_Position.x + deltaX, m_Position.y + deltaY, m_Position.z );
    CoreGuiBase::Move( deltaX, deltaY, isLocal );
  }
}
