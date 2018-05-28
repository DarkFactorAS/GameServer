/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
* File        : SquareTile
*
* Description : Overloaded from BaseTile. This class will handle and draw a tile shaped as a 
*               rectangle/square.
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "QuadTile.h"

#include "EvilEngine/CoreRendering/RenderObjects/QuadRenderObject.h"

SquareTile::SquareTile( uint32 resourceId ) :
  BaseTile( TILETYPE_QUAD, resourceId )
{
#if defined CLIENT
  m_RenderObject->AddRenderObject( new QuadRenderObject( resourceId ) );
#endif
}
