
#ifndef EVILENGINE_COREGUITOOLKIT_BASEGUIELEMENTS_COREGUIIMAGEVIEW
#define EVILENGINE_COREGUITOOLKIT_BASEGUIELEMENTS_COREGUIIMAGEVIEW 1

#include "CoreGuiBase.h"

class QuadRenderObject;
class Vector3;
class Color;

class CoreGuiImageView : public CoreGuiBase
{
public:

  CoreGuiImageView( const Vector2& position, const Vector2& size, const Color& color );
  CoreGuiImageView( const Vector2& position, const Vector2& size, uint32 resourceId );
  CoreGuiImageView( const String& name, const Vector2& position, const Vector2& size, uint32 resourceId );
  //~CoreGuiImageView();

  static CoreGuiBase*       Initialize(){ return new CoreGuiImageView( "",Vector2::s_Vec2Zero, Vector2::s_Vec2One, 0 ); }

  virtual void              SetWireframe( bool isWireframe );
  virtual void              SetPosition( float x, float y );
  virtual void              SetSize( float x, float y );
  virtual void              SetColor( const Color& color ){ SetColor( color.x, color.y, color.z ); }
  virtual void              SetColor( float red, float green, float blue );
  virtual void              SetAlpha( float alpha );
  virtual void              Move( float deltaX, float deltaY, bool isLocal );

  virtual void              SetResourceId( uint32 resourceId );

//#if defined DEBUG
//  virtual String              GetClassName() DF_OVERRIDE { return "CoreGuiImageView"; }
//#endif

private:
  QuadRenderObject*         m_ImageRender;
};

#endif /// EVILENGINE_COREGUITOOLKIT_BASEGUIELEMENTS_COREGUIIMAGEVIEW
