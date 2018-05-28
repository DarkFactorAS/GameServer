/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : QuadRenderObject
*
* Description : Creates a render object that is shaped like a quad/square.
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "PreCompile.h"
#include "RenderListObject.h"
#include "../OpenGLRender/CoreOpenGLRender.h"
#include "CoreLib/CoreMath/Matrix.h"
#include "CoreLib/CoreMath/Vector3.h"
#include <math.h>

RenderListObject::RenderListObject() : 
  RenderObject(),
  m_RenderList( 10, true )
{
  Init();
}

RenderListObject::~RenderListObject()
{
}

/************************************************************************************************
* Init:
* Initializes all the vertex points and texture points for this object. The vertex points is a
* list of 3D points in space that defines the shape. Texture points defines how the object is
* applying the texture on the different surfaces.
*
* @author Thor Richard Hansen
*************************************************************************************************/
void RenderListObject::Init()
{
}

void RenderListObject::SetColor( const Color& color )
{
  SetColor( color.x, color.y, color.z ); 
}
void RenderListObject::SetColor( float red,float green,float blue )
{
  RenderObject::SetColor( red, green, blue  );
  uint32 numElements = m_RenderList.GetSize();
  for ( uint32 index = 0; index < numElements; index++ )
  {
    RenderObject* renderObject = m_RenderList.Get( index );
    renderObject->SetColor( red, green, blue );
  }
}

void RenderListObject::SetAlpha( float alpha )
{
  RenderObject::SetAlpha( alpha );
  uint32 numElements = m_RenderList.GetSize();
  for ( uint32 index = 0; index < numElements; index++ )
  {
    RenderObject* renderObject = m_RenderList.Get( index );
    renderObject->SetAlpha( alpha );
  }
}

void RenderListObject::RemoveAllRenderObjects()
{
  m_RenderList.Clear();
}

void RenderListObject::AddRenderObject( RenderObject* renderObject )
{
  dfAssert( renderObject->GetParent() == NULL, "Trying to add a renderobject that already has a parent!" );
  renderObject->SetParent( this );
  m_RenderList.Add( renderObject );
}
void RenderListObject::AddRenderObject( uint32 instanceId, RenderObject* renderObject )
{
  dfAssert( renderObject->GetParent() == NULL, "Trying to add a renderobject that already has a parent!" );
  renderObject->SetParent( this );
  renderObject->SetInstance( instanceId );
  m_RenderList.Add( renderObject );
}

/************************************************************************************************
* OnDraw:
* Renders this object in the openGL engine
*
* @param  (const Matrix&)  projection - What projection to use : 2D (gui) or 3D ( world )
* @return (bool)                      - Returns false if the object cannot be rendered.
*
* @author Thor Richard Hansen
*************************************************************************************************/
bool RenderListObject::OnDraw( const Matrix& projection )
{
  bool canRender = true;
  uint32 numElements = m_RenderList.GetSize();
  for ( uint32 index = 0; index < numElements; index++ )
  {
    RenderObject* renderObject = m_RenderList.Get( index );
    if ( !renderObject->OnDraw( projection ) )
    {
      canRender = false;
    }
  }
  return canRender;
}

void RenderListObject::SetPosition( float x, float y, float z )
{
  RenderObject::SetPosition( x, y, z );
  uint32 numElements = m_RenderList.GetSize();
  for ( uint32 index = 0; index < numElements; index++ )
  {
    RenderObject* renderObject = m_RenderList.Get( index );
    renderObject->SetPosition( x, y, z );
  }
}
void RenderListObject::Move( float deltaX, float deltaY, bool isLocal )
{
  RenderObject::Move( deltaX, deltaY, isLocal );
  uint32 numElements = m_RenderList.GetSize();
  for ( uint32 index = 0; index < numElements; index++ )
  {
    RenderObject* renderObject = m_RenderList.Get( index );
    renderObject->Move( deltaX, deltaY, isLocal );
  }
}
void RenderListObject::SetRotation( float x, float y, float z )
{
  RenderObject::SetRotation( x, y, z );
  uint32 numElements = m_RenderList.GetSize();
  for ( uint32 index = 0; index < numElements; index++ )
  {
    RenderObject* renderObject = m_RenderList.Get( index );
    renderObject->SetRotation( x, y, z );
  }
}
void RenderListObject::SetSize( float x, float y, float z )
{
  RenderObject::SetSize( x, y, z );
  uint32 numElements = m_RenderList.GetSize();
  for ( uint32 index = 0; index < numElements; index++ )
  {
    RenderObject* renderObject = m_RenderList.Get( index );
    renderObject->SetSize( x, y, z );
  }
}

void RenderListObject::SetResourceId( uint32 resourceId )
{
  RenderObject::SetResourceId( resourceId );
  SetResourceId( 0, resourceId );
}

void RenderListObject::SetResourceId( int instanceId, uint32 resourceId )
{
  RenderObject* renderObject = GetRenderObject( instanceId );
  if ( renderObject != NULL )
  {
    renderObject->SetResourceId( resourceId );
  }
}

RenderObject* RenderListObject::GetRenderObject( uint32 instanceId )
{
  uint32 numElements = m_RenderList.GetSize();
  for ( uint32 index = 0; index < numElements; index++ )
  {
    RenderObject* renderObject = m_RenderList.Get( index );
    if ( renderObject->GetInstance() == instanceId )
    {
      return renderObject;
    }
  }
  return NULL;
}
