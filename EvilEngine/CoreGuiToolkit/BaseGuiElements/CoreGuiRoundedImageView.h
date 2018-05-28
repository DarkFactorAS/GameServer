
#ifndef EVILENGINE_COREGUITOOLKIT_BASEGUIELEMENTS_COREGUIROUNDEDIMAGEVIEW
#define EVILENGINE_COREGUITOOLKIT_BASEGUIELEMENTS_COREGUIROUNDEDIMAGEVIEW 1

#include "CoreGuiBase.h"

class QuadRenderObject;
class Vector3;
class Color;

class CoreGuiRoundedImageView : public CoreGuiBase
{
public:

  CoreGuiRoundedImageView( const Vector2& position, const Vector2& size, const Color& color );
  CoreGuiRoundedImageView( const Vector2& position, const Vector2& size, uint32 resourceId );
  CoreGuiRoundedImageView( const String& name, const Vector2& position, const Vector2& size, uint32 resourceId );

  static CoreGuiBase*       Initialize(){ return new CoreGuiRoundedImageView( "",Vector2::s_Vec2Zero, Vector2::s_Vec2One, 0 ); }

  void                      Init( const Vector3& pos, const Vector3& size, uint32 resourceId );

  virtual void              SetWireframe( bool isWireframe );
  virtual void              SetPosition( float x, float y );
  virtual void              SetSize( float x, float y );
  virtual void              SetColor( const Color& color ){ SetColor( color.x, color.y, color.z ); }
  virtual void              SetColor( float red, float green, float blue );
  virtual void              SetAlpha( float alpha );

  virtual void              SetResourceId( uint32 resourceId );

  //#if defined DEBUG
  //  virtual String              GetClassName() DF_OVERRIDE { return "CoreGuiImageView"; }
  //#endif
};

#endif /// EVILENGINE_COREGUITOOLKIT_BASEGUIELEMENTS_COREGUIROUNDEDIMAGEVIEW
