
#ifndef EVILENGINE_COREGUITOOLKIT_BASEGUIELEMENTS_COREGUITEXTFIELD
#define EVILENGINE_COREGUITOOLKIT_BASEGUIELEMENTS_COREGUITEXTFIELD

#include "../../CoreLib/BasicTypes/String/CoreString.h"
#include "CoreGuiBase.h"

class TextStringRenderObject;
class QuadRenderObject;
class CoreGuiFrame;

class CoreGuiTextfield : public CoreGuiBase
{
public:
  
  CoreGuiTextfield ( const String& text, const Vector2& position, const Vector2& size );
  CoreGuiTextfield ( const String& text, const Vector2& position, const Vector2& size, int textAlignment );
  CoreGuiTextfield ( const String& text, const Vector2& position, const Vector2& size, int textAlignment, const Color& color );
  ~CoreGuiTextfield ();

  static CoreGuiBase*         Initialize(){ return new CoreGuiTextfield( "",Vector2::s_Vec2Zero, Vector2::s_Vec2One ); }
  virtual void                SetMember( const String& key, const String& value ) DF_OVERRIDE;

  virtual void                SetSize( float x, float y ) DF_OVERRIDE;
  virtual void                SetPosition( float x, float y ) DF_OVERRIDE;
  virtual void                SetColor( const Color& color ) DF_OVERRIDE;
  virtual void                SetColor( float red, float green, float blue ) DF_OVERRIDE;
  virtual void                SetAlpha( float alpha ) DF_OVERRIDE;

  void                        SetText( const String& text );
  float                       GetTextWidth( size_t lineIndex );
  void                        SetTextScale( float scale );
  int                         GetTextAlignment();
  const String&               GetText() const{ return m_Text; }
  void                        SetAlignment( int textAlignment );

  static float                HEIGHT;
  static float                BORDER;

  void                        SetWordWrap( int numCharacters );

#if defined DEBUG
  virtual String              GetStaticClassName() DF_OVERRIDE { return "CoreGuiTextfield"; }
#endif

private:

  void                        Init( const String& text, const Vector2& position, const Vector2& size, int textAlignment, const Color& color );

  String                      m_Text;
  TextStringRenderObject*     m_Textfield;
};

#endif /// EVILENGINE_COREGUITOOLKIT_BASEGUIELEMENTS_COREGUITEXTFIELD
