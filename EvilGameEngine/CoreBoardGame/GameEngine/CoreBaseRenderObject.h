#ifndef EVILENGINE_CORERENDERING_COREBASERENDEROBJECT
#define EVILENGINE_CORERENDERING_COREBASERENDEROBJECT 1
#pragma once

#include "EvilEngine/CoreLib/CoreMath/CoreMath.h"
#include "EvilEngine/CoreLib/Signal/Signal.h"

#include "EvilEngine/CoreEngine/CoreBaseObject/CoreBaseObject.h"

#if defined CLIENT
  #include "EvilEngine/CoreRendering/RenderObjects/RenderObject.h"
  #include "EvilEngine/CoreRendering/RenderObjects/RenderListObject.h"
#endif

class CoreBaseRenderObject : public CoreBaseObject
{
public:

  CoreBaseRenderObject();
  CoreBaseRenderObject( uint32_t resourceId );
  CoreBaseRenderObject( const Vector3& position, const Vector3& size, uint32_t resourceId );
  virtual                                   ~CoreBaseRenderObject();

#if defined CLIENT
  RenderObject*                             GetRenderObject(){ return m_RenderObject; }
  void                                      SetRenderObject( RenderListObject* renderObject ){ m_RenderObject = renderObject; }
  void                                      RemoveAllRenderObjects();
  void                                      AddRenderObject( RenderObject* renderObject );
  virtual bool                              OnDraw( const Matrix& projection );
#endif /// CLIENT

  virtual void                              SetPosition( float x, float y, float z ) DF_OVERRIDE; 
  virtual void                              SetPosition( const Vector3& position ) DF_OVERRIDE { SetPosition( position.x, position.y, position.z ); }
  virtual void                              SetPosition( float x, float y ) DF_OVERRIDE { SetPosition( x, y, m_Position.z ); }

  virtual void                              SetSize( const Vector3& size ) DF_OVERRIDE { SetSize( size.x, size.y, size.z ); }
  virtual void                              SetSize( float x, float y, float z ) DF_OVERRIDE;

  virtual void                              SetRotation( float x, float y, float z ) DF_OVERRIDE;
  virtual void                              SetResourceId( uint32 resourceId ) DF_OVERRIDE;


  virtual void                              SetColor( const Color& color ) DF_OVERRIDE { SetColor( color.x, color.y, color.z ); }
  virtual void                              SetColor( float red, float green, float blue ) DF_OVERRIDE;
  virtual void                              SetAlpha( float alpha ) DF_OVERRIDE;

  virtual void                              SetFlag( int flag )       DF_OVERRIDE;
  virtual void                              ClearFlag( int flag )     DF_OVERRIDE;

protected:

#if defined CLIENT
  RenderListObject*                         m_RenderObject;
#endif /// CLIENT
};

#endif /// EVILENGINE_CORERENDERING_COREBASERENDEROBJECT