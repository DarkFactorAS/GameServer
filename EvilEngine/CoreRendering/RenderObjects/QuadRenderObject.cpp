/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : QuadRenderObject
*
* Description : Creates a render object that is shaped like a quad/square.
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "PreCompile.h"
#include "QuadRenderObject.h"
#include "../OpenGLRender/CoreOpenGLRender.h"
#include "CoreLib/CoreMath/Matrix.h"
#include "CoreLib/CoreMath/Vector3.h"
#include <math.h>

QuadRenderObject::QuadRenderObject() : 
  RenderObject()
{
  Init();
}

/************************************************************************************************
* QuadRenderObject:
* Constructor. Creates a quad render object with zero position and size.
*
* @param  (int)             resourceId  - The resource/texture to use for this object
*
* @author Thor Richard Hansen
*************************************************************************************************/
QuadRenderObject::QuadRenderObject( uint32 resourceId ) : 
  RenderObject( Vector3(), Vector3(), resourceId )
{
  Init();
}

/************************************************************************************************
* QuadRenderObject:
* Constructor. This is a wrapper for the gui usage of quads. This will probably be removed
* once I cleaned up the gui toolkit
*
* @param  (const Vector3&)  position    - 3D world position of the object
* @param  (const Vector3&)  size        - 3D size of the object ( width, height, depth )
* @param  (int)             resourceId  - The resource/texture to use for this object
*
* @author Thor Richard Hansen
*************************************************************************************************/
QuadRenderObject::QuadRenderObject( const Vector2& position, const Vector2& size, uint32 resourceId ) : 
  RenderObject( Vector3(position), Vector3(size), resourceId )
{
  Init();
}

/************************************************************************************************
* QuadRenderObject:
* Constructor
*
* @param  (const Vector3&)  position    - 3D world position of the object
* @param  (const Vector3&)  size        - 3D size of the object ( width, height, depth )
* @param  (int)             resourceId  - The resource/texture to use for this object
*
* @author Thor Richard Hansen
*************************************************************************************************/
QuadRenderObject::QuadRenderObject( const Vector3& position, const Vector3& size, uint32 resourceId ) : 
  RenderObject( position, size, resourceId )
{
  Init();
}

/************************************************************************************************
* Init:
* Initializes all the vertex points and texture points for this object. The vertex points is a
* list of 3D points in space that defines the shape. Texture points defines how the object is
* applying the texture on the different surfaces.
*
* @author Thor Richard Hansen
*************************************************************************************************/
void QuadRenderObject::Init()
{
  RenderObject::Init();

  //
  //    A------B
  //    |     /|
  //    |    / |
  //    |   /  |
  //    |  /   |
  //    | /    |
  //    |/     |
  //    C------D
  //

  SetVertices( 4 );
  // Vertex x,y,z        Normal x,y,z   Texture x,y
  AddPoint( -1,  1,  0,   1, 0, 0,       0, 0 );  // C
  AddPoint(  1,  1,  0,   1, 0, 0,       1, 0 );  // D 
  AddPoint( -1, -1,  0,   1, 0, 0,       0, 1 );  // A     
  AddPoint(  1, -1,  0,   1, 0, 0,       1, 1 );  // B
}

/************************************************************************************************
* SetTextureBufferLeft:
* Moves the texture to only fill part of the area ( to align the text to the left )
*
* @param  (float)      scale - How much to scale the texture. 1.0 is full scale, 0.5 is half
*
* @author Thor Richard Hansen
*************************************************************************************************/
void QuadRenderObject::SetTextureBufferLeft( float right )
{
  // Two triangle strips : 0,0 -> 0,1 -> scale,0 -> scale,1
  //float texture[] = { 0.0f, 0.0f, 0.0f, 1.0f, scale, 0.0f, scale, 1.0f };
  //SetTextureBuffer( texture );

  m_Buffer[ ( RENDER_BUFFER_SIZE * 1 ) + 6 ] = right;
  m_Buffer[ ( RENDER_BUFFER_SIZE * 3 ) + 6 ] = right;
}

void QuadRenderObject::SetTextureBufferRight( float left )
{
  // Two triangle strips : scale,0 -> scale,1 -> 1,0 -> 1,1
  //float texture[] = { scale, 0.0f, scale, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f };
  //SetTextureBuffer( texture );

  m_Buffer[ ( RENDER_BUFFER_SIZE * 0 ) + 6 ] = left;
  m_Buffer[ ( RENDER_BUFFER_SIZE * 2 ) + 6 ] = left;
}

void QuadRenderObject::SetTextureBufferOffset( float left, float top, float right, float bottom )
{
  // Two triangle strips : right,bottom -> right,top -> left,bottom -> left,top

  m_Buffer[ ( RENDER_BUFFER_SIZE * 0 ) + 6 ] = right;
  m_Buffer[ ( RENDER_BUFFER_SIZE * 0 ) + 7 ] = bottom;

  m_Buffer[ ( RENDER_BUFFER_SIZE * 1 ) + 6 ] = left;
  m_Buffer[ ( RENDER_BUFFER_SIZE * 1 ) + 7 ] = bottom;

  m_Buffer[ ( RENDER_BUFFER_SIZE * 2 ) + 6 ] = right;
  m_Buffer[ ( RENDER_BUFFER_SIZE * 2 ) + 7 ] = top;

  m_Buffer[ ( RENDER_BUFFER_SIZE * 3 ) + 6 ] = left;
  m_Buffer[ ( RENDER_BUFFER_SIZE * 3 ) + 7 ] = top;

}
