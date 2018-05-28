#ifndef EVILENGINE_CORERENDERING_RENDEROBJECT_RENDERLISTOBJECT
#define EVILENGINE_CORERENDERING_RENDEROBJECT_RENDERLISTOBJECT 1

#include "RenderObject.h"

#include "EvilEngine/CoreLib/BasicTypes/FixedMaps/FixedSizeVector.h"

class RenderListObject : public RenderObject
{
public:
  RenderListObject();
  virtual ~RenderListObject();

#if defined DEBUG
  virtual String                            GetStaticClassName()                                DF_OVERRIDE { return "RenderListObject"; }
#endif

  // Overloads from RenderObject
  virtual void                              SetColor(float red,float green,float blue )         DF_OVERRIDE;
  virtual void                              SetColor( const Color& color )                      DF_OVERRIDE;
  virtual bool                              OnDraw( const Matrix& projection )                  DF_OVERRIDE;
  virtual void                              SetAlpha( float alpha )                             DF_OVERRIDE;

  // Overloads from CoreBaseObject
  virtual void                              SetPosition( float x, float y, float z )            DF_OVERRIDE;
  virtual void                              Move( float deltaX, float deltaY, bool isLocal )    DF_OVERRIDE;
  virtual void                              SetRotation( float x, float y, float z )            DF_OVERRIDE;
  virtual void                              SetSize( float x, float y, float z )                DF_OVERRIDE;
  virtual void                              SetResourceId( uint32 resourceId )                  DF_OVERRIDE;
  virtual void                              SetResourceId( int instanceId, uint32 resourceId )  DF_OVERRIDE;

  void                                      RemoveAllRenderObjects();
  void                                      AddRenderObject( RenderObject* renderObject );
  void                                      AddRenderObject( uint32 instanceId, RenderObject* renderObject );
  RenderObject*                             GetRenderObject( uint32 instanceId );
  FixedSizeVector< RenderObject* >&         GetRenderListObject(){ return m_RenderList; }

protected:

  virtual void                              Init()                                              DF_OVERRIDE;

  FixedSizeVector< RenderObject* >          m_RenderList;
};

#endif /// EVILENGINE_CORERENDERING_RENDEROBJECT_RENDERLISTOBJECT