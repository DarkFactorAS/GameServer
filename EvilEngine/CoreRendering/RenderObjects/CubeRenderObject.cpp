/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : CubeRenderObject
*
* Description : Creates a render object that is shaped like a cube.
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "PreCompile.h"
#include "CubeRenderObject.h"
#include "../OpenGLRender/CoreOpenGLRender.h"
#include "../CoreShaderManager/CoreShaderManager.h"

CubeRenderObject::CubeRenderObject() : 
  RenderObject()
{
  Init();
}

/************************************************************************************************
* CubeRenderObject:
* Constructor
*
* @param  (const Vector3&)  position    - 3D world position of the object
* @param  (const Vector3&)  size        - 3D size of the object ( width, height, depth )
* @param  (int)             resourceId  - The resource/texture to use for this object
*
* @author Thor Richard Hansen
*************************************************************************************************/
CubeRenderObject::CubeRenderObject( const Vector3& position, const Vector3& size, uint32 resourceId ) :
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
void CubeRenderObject::Init()
{
  RenderObject::Init();

  //
  // 4 Points x 6 sides ( + 2 since we are doing DRAW_LINESTRIP )
  //
  SetVertices( 26 );

  // The data is set up like this
  // Vertex x,y,z        Normal x,y,z   Texture x,y
  AddPoint( -1, -1,  1,   1, 0, 0,       0, 0 );       // Front face
  AddPoint(  1, -1,  1,   1, 0, 0,       1, 0 );       // Front face
  AddPoint( -1,  1,  1,   1, 0, 0,       0, 1 );       // Front face
  AddPoint(  1,  1,  1,   1, 0, 0,       1, 1 );       // Front face

  AddPoint(  1,  1,  1,   0, 1, 0,       0, 1 );       // Right face
  AddPoint(  1, -1,  1,   0, 1, 0,       0, 0 );       // Right face
  AddPoint(  1,  1, -1,   0, 1, 0,       1, 1 );       // Right face
  AddPoint(  1, -1, -1,   0, 1, 0,       1, 0 );       // Right face

  AddPoint(  1, -1, -1,   1, 0, 0,       0, 0 );       // Back face
  AddPoint( -1, -1, -1,   1, 0, 0,       1, 0 );       // Back face
  AddPoint(  1,  1, -1,   1, 0, 0,       0, 1 );       // Back face
  AddPoint( -1,  1, -1,   1, 0, 0,       1, 1 );       // Back face

  AddPoint( -1,  1, -1,   1, 0, 0,       0, 1 );       // Left face
  AddPoint( -1, -1, -1,   1, 0, 0,       0, 0 );       // Left face
  AddPoint( -1,  1,  1,   1, 0, 0,       1, 1 );       // Left face
  AddPoint( -1, -1,  1,   1, 0, 0,       1, 0 );       // Left face

  AddPoint( -1, -1,  1,   1, 0, 0,       0, 1 );       // Bottom face
  AddPoint( -1, -1, -1,   1, 0, 0,       0, 0 );       // Bottom face
  AddPoint(  1, -1,  1,   1, 0, 0,       1, 1 );       // Bottom face
  AddPoint(  1, -1, -1,   1, 0, 0,       1, 0 );       // Bottom face

  AddPoint( -1, -1, -1,   1, 0, 0,       1, 0 );       // Connect
  AddPoint( -1,  1,  1,   1, 0, 0,       0, 0 );       // Connect

  AddPoint( -1,  1,  1,   0, 0, 1,       0, 0 );       // Top face
  AddPoint(  1,  1,  1,   0, 0, 1,       1, 0 );       // Top face
  AddPoint( -1,  1, -1,   0, 0, 1,       0, 1 );       // Top face
  AddPoint(  1,  1, -1,   0, 0, 1,       1, 1 );       // Top face
}
