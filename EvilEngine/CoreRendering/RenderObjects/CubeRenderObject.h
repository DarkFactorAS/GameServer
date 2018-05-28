#ifndef EVILENGINE_CORERENDERING_RENDEROBJECT_CUBERENDEROBJECT
#define EVILENGINE_CORERENDERING_RENDEROBJECT_CUBERENDEROBJECT 1

//#if defined CLIENT

#include "RenderObject.h"

class CubeRenderObject : public RenderObject
{
public:
  CubeRenderObject();
  CubeRenderObject( const Vector3& position, const Vector3& size, uint32_t resourceId );

#if defined DEBUG
  virtual String            GetStaticClassName() DF_OVERRIDE { return "CubeRenderObject"; }
#endif

protected:
  virtual void Init() DF_OVERRIDE;
};

//#endif

#endif /// EVILENGINE_CORERENDERING_RENDEROBJECT_CUBERENDEROBJECT