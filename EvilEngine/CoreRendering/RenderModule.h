/*************************************************************************************
 * Project     : EvilEngine Multiplatform 2014
 * File        : RenderModule
 * Description : Abstract interface class for CoreOpenGLRender. This interface is
 *               used to run non 3D render objects in the render engine ( like GUI 
 *               and effects/particles )
 *
 * Author      : Thor Richard Hansen
 *************************************************************************************/

#ifndef EVILENGINE_CORERENDERING_RENDERMODULE
#define EVILENGINE_CORERENDERING_RENDERMODULE 1

#include "../CoreLib/CoreMath/Matrix.h"

class RenderModule
{
public:

  RenderModule( uint32 moduleId ) : m_ModuleId( moduleId ){};

  virtual ~RenderModule(){};
#ifndef SERVER
  virtual void OnDraw( const Matrix& projection ) = 0;
  virtual void ResizeScreen( int width, int height ) = 0;
#endif
  uint32 m_ModuleId;
};

#endif /// EVILENGINE_CORERENDERING_RENDERMODULE