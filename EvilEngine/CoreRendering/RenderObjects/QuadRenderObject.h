#ifndef EVILENGINE_CORERENDERING_RENDEROBJECT_QUADRENDEROBJECT
#define EVILENGINE_CORERENDERING_RENDEROBJECT_QUADRENDEROBJECT 1

#include "RenderObject.h"

class QuadRenderObject : public RenderObject
{
public:
  QuadRenderObject();
  QuadRenderObject( uint32 resourceId );
  QuadRenderObject( const Vector3& position, const Vector3& size, uint32 resourceId );
  QuadRenderObject( const Vector2& position, const Vector2& size, uint32 resourceId );

#if defined DEBUG
  virtual String            GetStaticClassName() DF_OVERRIDE { return "QuadRenderObject"; }
#endif

  void                      SetTextureBufferLeft( float scale );
  void                      SetTextureBufferRight( float scale );
  void                      SetTextureBufferOffset( float left, float top, float right, float bottom );

protected:
  virtual void              Init() DF_OVERRIDE;
};

#endif /// EVILENGINE_CORERENDERING_RENDEROBJECT_QUADRENDEROBJECT