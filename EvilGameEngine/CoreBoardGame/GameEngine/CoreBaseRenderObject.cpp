
#include "Precompile.h"
#include "CoreBaseRenderObject.h"
#include "EvilEngine/CoreLib/BasicTypes/Definitions/VolatileArgs.h"

#if defined CLIENT
  #include "EvilEngine/CoreRendering/RenderObjects/RenderObject.h"
#endif

CoreBaseRenderObject::CoreBaseRenderObject() : 
  CoreBaseObject()
{
#if defined CLIENT
  m_RenderObject = new RenderListObject();
  m_RenderObject->SetParent( this );
#endif
}

CoreBaseRenderObject::CoreBaseRenderObject( uint32_t resourceId ) : 
  CoreBaseObject( resourceId )
{
#if defined CLIENT
  m_RenderObject = new RenderListObject();
  m_RenderObject->SetParent( this );
#endif
};

CoreBaseRenderObject::CoreBaseRenderObject( const Vector3& position, const Vector3& size, uint32_t resourceId ) :
  CoreBaseObject( position, size, resourceId )
{
#if defined CLIENT
  m_RenderObject = new RenderListObject();
  m_RenderObject->SetParent( this );
#endif
}

CoreBaseRenderObject::~CoreBaseRenderObject()
{
#if defined CLIENT
  delete m_RenderObject;
  m_RenderObject = NULL;
#endif
}

#if defined CLIENT
void CoreBaseRenderObject::RemoveAllRenderObjects()
{
  m_RenderObject->RemoveAllRenderObjects();
}
#endif

#if defined CLIENT
void CoreBaseRenderObject::AddRenderObject( RenderObject* renderObject )
{
  m_RenderObject->AddRenderObject( renderObject );
}
#endif

/************************************************************************************************
* OnDraw:
* Draw the ground of the tile
*
* @param  (const Matrix&)  projection  - What kind of projection that is used. The projection
*                                        can either be flat 2D gui or 3D world
*
* @author Thor Richard Hansen
*************************************************************************************************/
#if defined CLIENT
bool CoreBaseRenderObject::OnDraw( const Matrix& projection )
{
  return m_RenderObject->OnDraw( projection );
}
#endif

/************************************************************************************************
* SetPosition:
* Set the world position of the renderobject/gameobject
*
* @param (float) x - X position in the world
* @param (float) y - Y position in the world
* @param (float) z - Z position in the world
*
* @author Thor Richard Hansen
*************************************************************************************************/
void CoreBaseRenderObject::SetPosition( float x, float y, float z )
{
  CoreBaseObject::SetPosition( x, y, z );
#if defined CLIENT
  m_RenderObject->SetPosition( x, y, z );
#endif
}

/************************************************************************************************
* SetSize:
* Set the size of the renderobject/gameobject.
*
* @param (float) x - How wide the tile is
* @param (float) y - How deep the tile is
* @param (float) z - How high the tile is
*
* @author Thor Richard Hansen
*************************************************************************************************/
void CoreBaseRenderObject::SetSize( float x, float y, float z )
{
  CoreBaseObject::SetSize( x, y, z );
#if defined CLIENT
  m_RenderObject->SetSize( x, y, z );
#endif
}


/************************************************************************************************
* SetRotation:
* Set the rotiation of the tile.
*
* @param (float) x - Rotation around the x axis.
* @param (float) y - Rotation around the y axis.
* @param (float) z - Rotation around the z axis.
*
* @author Thor Richard Hansen
*************************************************************************************************/
void CoreBaseRenderObject::SetRotation( float x, float y, float z )
{
  CoreBaseObject::SetRotation( x, y, z );
#if defined CLIENT
  m_RenderObject->SetRotation( x, y, z );
#endif
}

void CoreBaseRenderObject::SetFlag( int flag )
{
  CoreBaseObject::SetFlag( flag );
#if defined CLIENT
  m_RenderObject->SetFlag( flag );
#endif
}

void CoreBaseRenderObject::ClearFlag( int flag )
{
  CoreBaseObject::ClearFlag( flag );
#if defined CLIENT
  m_RenderObject->ClearFlag( flag );
#endif
}

/************************************************************************************************
* SetColor:
* Set the color of the tile.
*
* @param (float) red   - 
* @param (float) green - 
* @param (float) blue  - 
*
* @author Thor Richard Hansen
*************************************************************************************************/
void CoreBaseRenderObject::SetColor( float red, float green, float blue )
{
  CoreBaseObject::SetColor( red, green, blue );
#if defined CLIENT
  m_RenderObject->SetColor( red, green, blue );
#endif
}

void CoreBaseRenderObject::SetAlpha( float alpha )
{
  CoreBaseObject::SetAlpha( alpha );
#if defined CLIENT
  m_RenderObject->SetAlpha( alpha );
#endif
}

void CoreBaseRenderObject::SetResourceId( uint32 resourceId )
{
  CoreBaseObject::SetResourceId( resourceId );
#if defined CLIENT
  m_RenderObject->SetResourceId( resourceId );
#endif
}
