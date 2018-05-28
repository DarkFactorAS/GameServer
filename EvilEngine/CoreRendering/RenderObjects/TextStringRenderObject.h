#ifndef EVILENGINE_CORERENDERING_RENDEROBJECTS_TEXTSTRINGRENDEROBJECT
#define EVILENGINE_CORERENDERING_RENDEROBJECTS_TEXTSTRINGRENDEROBJECT 1

#include "QuadRenderObject.h"
#include "EvilEngine/CoreLib/BasicTypes/String/CoreString.h"
#include "EvilEngine/CoreLib/BasicTypes/String/StringList.h"
#include "EvilEngine/CoreLib/BasicTypes/Mutex/ScopedMutex.h"
#include "EvilEngine/CoreLib/Signal/SignalTarget.h"
#include "../Bitmap.h"
#include <vector>

class Textline
{
public:
  Textline( String text, Vector3 adjustedPos, Vector3 adjustedSize, float scale, float textureScale, uint32 textureId, Bitmap* texture )
  {
    m_Text          = text;
    m_AdjustedPos   = adjustedPos;
    m_AdjustedSize  = adjustedSize;
    m_Scale         = scale;
    m_TextureScale  = textureScale;
    m_TextureId     = textureId;
    m_Texture       = texture;
  }

//private:
  Vector3             m_AdjustedPos;
  Vector3             m_AdjustedSize;
  float               m_Scale;
  float               m_TextureScale;
  String              m_Text;
  uint32              m_TextureId;
  Bitmap*             m_Texture;
};

class TextStringRenderObject : public QuadRenderObject, public SignalTarget
{
public:

  enum ALIGNMENT
  {
    ALIGNMENT_CENTER  = 0,
    ALIGNMENT_TOPLEFT,
    ALIGNMENT_TOPCENTER,
    ALIGNMENT_TOPRIGHT,
    ALIGNMENT_CENTERLEFT,
    ALIGNMENT_CENTERRIGHT,
  };

  TextStringRenderObject( const String& text, const Vector3& position, const Vector3& size, int fontId );
  ~TextStringRenderObject();

  virtual bool              OnDraw( const Matrix& projection )            DF_OVERRIDE;
  uint32                    CreateTexture();
  virtual void              SetPosition( float x, float y, float z )      DF_OVERRIDE;
  virtual void              SetSize( float x, float y, float z )          DF_OVERRIDE;

  void                      SetFont( int fontId ){ m_FontId = fontId; }
  void                      SetAlignment( int alignment ){ m_Alignment = alignment; }
  void                      SetText( const String& text );

  float                     GetTextWidth( size_t lineIndex );
  int                       GetTextAlignment() const{ return m_Alignment; }
  void                      SetTextScale( float scale );

  void                      SetWordWrap( int numCharacters );

#if defined DEBUG
  virtual String            GetStaticClassName() DF_OVERRIDE { return "TextStringRenderObject"; }
#endif

private:

  uint8*                    CopyImage( uint8* srcData, int srcWidth, int srcStartX, int srcStartY, int width, int height );

  void                      Release();

  int                       m_FontId;
  String                    m_Text;
  std::vector<Textline>     m_TextLines;
  int                       m_Alignment;
  float                     m_Scale;

  Mutex                     m_TextMutex;
  bool                      m_LoadedTextures;
  int                       m_WordWrap;
};

#endif /// EVILENGINE_CORERENDERING_RENDEROBJECTS_TEXTSTRINGRENDEROBJECT
