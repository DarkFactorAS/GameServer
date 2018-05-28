
#ifndef EVILENGINE_COREGUITOOLKIT_BASEGUIELEMENTS_COREGUIINPUTFIELD
#define EVILENGINE_COREGUITOOLKIT_BASEGUIELEMENTS_COREGUIINPUTFIELD

#include "../../CoreLib/BasicTypes/String/CoreString.h"
#include "CoreGuiBase.h"

class CoreGuiTextfield;
class CoreGuiImageView;
class CoreGuiFrame;

class CoreGuiInputfield : public CoreGuiBase
{
public:

  CoreGuiInputfield( const String& fieldValue, const Vector2& position, const Vector2& size );
  CoreGuiInputfield( const String& fieldValue, const Vector2& position, const Vector2& size, int textAlignment );
  CoreGuiInputfield( const String& fieldName, const String& fieldValue, const Vector2& position, const Vector2& size, int textAlignment );
  ~CoreGuiInputfield ();

  static CoreGuiBase*         Initialize(){ return new CoreGuiInputfield( "",Vector2::s_Vec2Zero, Vector2::s_Vec2One ); }
  virtual void                SetMember( const String& key, const String& value ) DF_OVERRIDE;

  virtual void                SetColor( float red, float green, float blue ) DF_OVERRIDE;
  virtual void                SetAlpha( float alpha ) DF_OVERRIDE;

  virtual CoreGuiBase*        OnMouseButtonDown( int buttonId, float xPos, float yPos ) DF_OVERRIDE;
  virtual bool                OnKeyDown( int keyCode ) DF_OVERRIDE;
  virtual bool                OnKeyUp( int keyCode ) DF_OVERRIDE;
  CoreGuiBase*                GetSolidGuiUnderMouse( float xPos, float yPos ) DF_OVERRIDE;

  virtual bool                GainFocus() DF_OVERRIDE;
  virtual void                LooseFocus() DF_OVERRIDE;
  virtual bool                CanTakeFocus() DF_OVERRIDE{ return true; }
  virtual bool                RequiresKeyboard() DF_OVERRIDE{ return true; }

  const String&               GetText() const { return m_Text; }
  void                        SetText( const String& text );
  void                        SetTooltip( const String& text );
  void                        SetValue( uint32 val );
  void                        SetValue( float val );
  const String&               GetValue() const { return m_Text; }
  float                       GetFloatValue();
  void                        SetPassword( bool isPasswordField );
  void                        SetPasswordReplace( const String& replaceChar ){ m_ReplacePwd = replaceChar; }

  virtual bool                OnDraw( const Matrix& projection ) DF_OVERRIDE;

  void                        SetBackgroundColor( const Color& color );
  void                        SetBorderColor( const Color& color ) DF_OVERRIDE;
  void                        SetActiveBorderColor(const Color& color);
  void                        SetTextColor( const Color& color );
  void                        SetTooltipColor(const Color& color);
  void                        SetActiveTextColor( const Color& color );
  void                        SetEnabled( bool isEnabled );
  void                        SetTextAlpha( float alpha );

#if defined DEBUG
  virtual String              GetStaticClassName() DF_OVERRIDE { return "CoreGuiInputfield"; }
#endif

private:
  void                        Init(const String& text, const Vector2& position, const Vector2& size, int textAlignment);

  void                        UpdateText();
  void                        UpdateCursor();

  static float                BORDER;
  
  bool                        m_IsPassword;
  String                      m_Text;
  String                      m_Tooltip;
  String                      m_DisplayText;
  CoreGuiTextfield*           m_Textfield;
  CoreGuiTextfield*           m_Cursor;
  CoreGuiImageView*           m_Background;
  CoreGuiFrame*               m_Frame;
  bool                        m_UpdateCursor;
  bool                        m_IsEnabled;
  bool                        m_UseTextAlpha;
  bool                        m_UseMultiline;

  Color                       m_FrameColor1;
  Color                       m_FrameColor2;
  Color                       m_TextColorInactive;
  Color                       m_TextColorActive;
  Color                       m_TextColorTip;
  String                      m_ReplacePwd;
};

#endif /// EVILENGINE_COREGUITOOLKIT_BASEGUIELEMENTS_COREGUITEXTFIELD
