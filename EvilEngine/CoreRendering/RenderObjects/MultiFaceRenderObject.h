#ifndef EVILENGINE_CORERENDERING_RENDEROBJECT_MESHRENDEROBJECT
#define EVILENGINE_CORERENDERING_RENDEROBJECT_MESHRENDEROBJECT 1

#include "RenderObject.h"

class MultiFaceRenderObject : public RenderObject
{
public:
  MultiFaceRenderObject();
  MultiFaceRenderObject( uint32 numFaces );
  MultiFaceRenderObject( uint32 numFaces, uint32 resourceId );
  ~MultiFaceRenderObject();

#if defined DEBUG
  virtual String            GetStaticClassName() DF_OVERRIDE { return "MultiFaceRenderObject"; }
#endif

protected:
  void                      InitFaces( uint32 numFaces );
};

#endif /// EVILENGINE_CORERENDERING_RENDEROBJECT_MESHRENDEROBJECT