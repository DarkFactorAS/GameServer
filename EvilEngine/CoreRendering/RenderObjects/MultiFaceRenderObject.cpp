/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : QuadRenderObject
*
* Description : Creates a render object that is shaped like a quad/square.
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "PreCompile.h"
#include "MultiFaceRenderObject.h"
#include "../OpenGLRender/CoreOpenGLRender.h"
#include "CoreLib/CoreMath/Matrix.h"
#include "CoreLib/CoreMath/Vector3.h"
#include "../CoreShaderManager/CoreShaderManager.h"

#include <math.h>

MultiFaceRenderObject::MultiFaceRenderObject() : 
  RenderObject()
{
  Init();
}

MultiFaceRenderObject::MultiFaceRenderObject( uint32 numFaces ) : 
  RenderObject()
{
  InitFaces( numFaces );
}

/************************************************************************************************
* QuadRenderObject:
* Constructor. Creates a quad render object with zero position and size.
*
* @param  (int)             resourceId  - The resource/texture to use for this object
*
* @author Thor Richard Hansen
*************************************************************************************************/
MultiFaceRenderObject::MultiFaceRenderObject( uint32 numFaces, uint32 resourceId ) : 
  RenderObject( Vector3(), Vector3(), resourceId )
{
  InitFaces( numFaces );
}

MultiFaceRenderObject::~MultiFaceRenderObject()
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
void MultiFaceRenderObject::InitFaces( uint32 numFaces )
{
  RenderObject::Init();

  //
  // 4 Points x 1 side
  //
  SetVertices( numFaces );

  //// Vertex x,y,z        Normal x,y,z   Texture x,y
  //AddPoint( -1, -1,  0,   1, 0, 0,       0, 0 );      
  //AddPoint(  1, -1,  0,   1, 0, 0,       1, 0 );       
  //AddPoint( -1,  1,  0,   1, 0, 0,       0, 1 );       
  //AddPoint(  1,  1,  0,   1, 0, 0,       1, 1 );
}
