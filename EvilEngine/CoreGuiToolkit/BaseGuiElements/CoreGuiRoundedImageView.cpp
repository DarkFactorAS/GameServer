/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : CoreGuiImageView
* Description : Creates the normal quad to render a texture on. Mainly used for background and
*               clean surfaces
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/
#include "PreCompile.h"
#include "CoreGuiRoundedImageView.h"
#include "CoreGuiFrame.h"
#include "../../CoreRendering/RenderObjects/QuadRenderObject.h"
#include "EvilEngine/CoreRendering/OpenGLRender/CoreOpenGLRender.h"

CoreGuiRoundedImageView::CoreGuiRoundedImageView( const Vector2& position, const Vector2& size, const Color& color ) :
  CoreGuiBase( "CoreGuiImageView", position, size )
{
  Init( Vector3( position ), Vector3( size ), GuiResource_White );
  SetColor( color );
}

CoreGuiRoundedImageView::CoreGuiRoundedImageView( const Vector2& position, const Vector2& size, uint32 resourceId ) :
  CoreGuiBase( "CoreGuiImageView", position, size, resourceId )
{
  Init( Vector3( position ), Vector3( size ), resourceId );
}

CoreGuiRoundedImageView::CoreGuiRoundedImageView( const String& objectName, const Vector2& position, const Vector2& size, uint32 resourceId ) :
  CoreGuiBase( objectName, position, size, resourceId )
{
  Init( Vector3( position ), Vector3( size ), resourceId );
}

void CoreGuiRoundedImageView::Init( const Vector3& position, const Vector3& size, uint32 resourceId )
{
  RemoveAllRenderObjects();
  RemoveAllChildren();

  QuadRenderObject* renderObj = NULL;

  float scale         = 0.1f;
  float scaleMin      = 0.05f;
  float borderSize    = 0.15f;
  float zPosition     = m_Position.z;

  float borderX       = size.y * borderSize;
  if ( borderX > size.x * 0.5f )
  {
    borderX = size.x * 0.5f;
  }

  Vector2 border      = Vector2( borderX, size.y );

  Vector3 left   = Vector3( position.x - size.x, position.y, zPosition );
  Vector3 center = Vector3( position.x, position.y, zPosition );
  Vector3 right  = Vector3( position.x + size.x, position.y, zPosition );

  Vector3 endSize = Vector3( border.x,  border.y, size.z );
  Vector3 centerSize = Vector3( size.x - endSize.x,  border.y, size.z );

  float alpha = GetAlpha();

  //AddChild( new CoreGuiFrame( left,   endSize, Color::Yellow ) );
  //AddChild( new CoreGuiFrame( center, centerSize, Color::Black ) );
  //AddChild( new CoreGuiFrame( right,  endSize, Color::Yellow ) );

  // Left
  renderObj = new QuadRenderObject( left, endSize, resourceId );
  renderObj->SetTextureBufferOffset( 1.0f - scale, scaleMin, 1.0f, 1.0f - scaleMin );
  renderObj->SetColor( m_Color );
  renderObj->SetAlpha( alpha );
  AddRenderObject( renderObj );

  // Center
  renderObj = new QuadRenderObject( center, centerSize, resourceId );
  renderObj->SetTextureBufferOffset( scale, scaleMin, 1.0f - scale, 1.0f - scaleMin );
  renderObj->SetColor( m_Color );
  renderObj->SetAlpha( alpha );
  AddRenderObject( renderObj );

  // Right
  renderObj = new QuadRenderObject( right, endSize, resourceId );
  renderObj->SetTextureBufferOffset( 0.0f, scaleMin, scale, 1.0f - scaleMin );
  renderObj->SetColor( m_Color );
  renderObj->SetAlpha( alpha );
  AddRenderObject( renderObj );
}

void CoreGuiRoundedImageView::SetResourceId( uint32 resourceId )
{
  CoreGuiBase::SetResourceId( resourceId );
  Init( m_Position, m_Size, resourceId );
}

void CoreGuiRoundedImageView::SetPosition( float x, float y )
{
  CoreGuiBase::SetPosition( x, y );
  Init( Vector2(x, y ), m_Size, m_ResourceId );
}

void CoreGuiRoundedImageView::SetSize( float x, float y )
{
  CoreGuiBase::SetSize( x, y );
  Init( m_Position, Vector2(x, y ), m_ResourceId );
}

void CoreGuiRoundedImageView::SetColor( float red, float green, float blue )
{
  CoreGuiBase::SetColor( red, green, blue );
  for ( size_t index = 0; index < m_RenderObjects.size(); index++ )
  {
    RenderObject* renderObject = m_RenderObjects[ index ];
    renderObject->SetColor( red, green, blue );
  }
  Init( m_Position, m_Size, m_ResourceId );
}

void CoreGuiRoundedImageView::SetAlpha( float alpha )
{
  CoreGuiBase::SetAlpha( alpha );
  for ( size_t index = 0; index < m_RenderObjects.size(); index++ )
  {
    RenderObject* renderObject = m_RenderObjects[ index ];
    renderObject->SetAlpha( alpha );
  }
  Init( m_Position, m_Size, m_ResourceId );
}

void CoreGuiRoundedImageView::SetWireframe( bool isWireframe )
{
  CoreGuiBase::SetWireframe( isWireframe );
  //m_ImageRender->SetWireframe( isWireframe );
}
