#ifndef EVILENGINE_CORERENDERING_RENDEROBJECT_HEXAGONRENDEROBJECT
#define EVILENGINE_CORERENDERING_RENDEROBJECT_HEXAGONRENDEROBJECT 1

#include "RenderListObject.h"

class HexagonRenderObject : public RenderListObject
{
public:

  enum RenderType
  {
    GroundRender  = 1,
    WallRender    = 2,
  };

                                            HexagonRenderObject( uint32 resourceId, uint32 wallResourceId );
                                            HexagonRenderObject( const Vector3& position, const Vector3& size, uint32 resourceId, uint32 wallResourceId );
  virtual                                   ~HexagonRenderObject(){}

  void                                      SetWallColor( const Color& color );
  virtual void                              SetResourceId( uint32 resourceId )          DF_OVERRIDE;

protected:

  virtual void                              Init()                                      DF_OVERRIDE;
  void                                      InitRender( const Vector3& position, const Vector3& size, uint32 resourceId, uint32 wallResourceId );

};

#endif /// EVILENGINE_CORERENDERING_RENDEROBJECT_HEXAGONRENDEROBJECT
