
#ifndef EVILENGINE_COREGUITOOLKIT_BASEGUIELEMENTS_COREGUIFRAME
#define EVILENGINE_COREGUITOOLKIT_BASEGUIELEMENTS_COREGUIFRAME 1

#include "CoreGuiBase.h"

class QuadRenderObject;
class TextStringRenderObject;
class Vector3;
class Vector2;

class CoreGuiFrame : public CoreGuiBase
{
public:

  CoreGuiFrame( const Vector2& position, const Vector2& size, const Color& color );
  CoreGuiFrame( const String& objectName, const Vector2& position, const Vector2& size, const Color& color );

  static CoreGuiBase*         Initialize(){ return new CoreGuiFrame( Vector2::s_Vec2Zero, Vector2::s_Vec2One, Color::Black ); }

  virtual void                SetColor( float red, float green, float blue ) DF_OVERRIDE;
  virtual void                SetColor( const Color& color ) DF_OVERRIDE { SetColor( color.x, color.y, color.z ); }
  virtual void                SetAlpha( float alpha ) DF_OVERRIDE;
  virtual void                SetPosition( float x, float y ) DF_OVERRIDE;
  virtual void                SetSize( float x, float y ) DF_OVERRIDE;
  virtual CoreGuiBase*        GetWireGuiUnderMouse( float xPos, float yPos ) DF_OVERRIDE;
  virtual CoreGuiBase*        GetSolidGuiUnderMouse( float xPos, float yPos ) DF_OVERRIDE;

  static  void                AddFrame( CoreGuiBase* parent );

  static float                BORDER;

#if defined DEBUG
  virtual String              GetStaticClassName() DF_OVERRIDE { return "CoreGuiFrame"; }
#endif

private:

  void                        InitPosition();

  QuadRenderObject*           m_Left;
  QuadRenderObject*           m_Top;
  QuadRenderObject*           m_Right;
  QuadRenderObject*           m_Bottom;
};

#endif /// EVILENGINE_COREGUITOOLKIT_BASEGUIELEMENTS_COREGUIFRAME
