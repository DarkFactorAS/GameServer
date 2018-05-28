
#ifndef EVILENGINE_COREGUITOOLKIT_BASEGUIELEMENTS_COREGUISCROLLBAR
#define EVILENGINE_COREGUITOOLKIT_BASEGUIELEMENTS_COREGUISCROLLBAR

#include "CoreGuiBase.h"

class CoreGuiImageView;

class CoreGuiScrollbar : public CoreGuiBase
{
public:

  enum SCROLL_DIRECTION
  {
     SCROLL_HORIZONTAL,
     SCROLL_VERTICAL,
  };

  CoreGuiScrollbar( const Vector2& position, const Vector2& size, const Color& color, SCROLL_DIRECTION scrollDirection );
  ~CoreGuiScrollbar();

  virtual CoreGuiBase*      OnMouseButtonDown(int buttonId, float xPos, float yPos) DF_OVERRIDE;
  virtual CoreGuiBase*      OnMouseButtonUp(int buttonId, float xPos, float yPos) DF_OVERRIDE;
  virtual bool              OnMouseButtonWheel(float delta, const Vector2& mousePos);
  virtual bool              OnMouseDrag(float  xPos, float  yPos) DF_OVERRIDE;
  virtual bool              GainFocus(){ return true; };

  void                      SetTotalSize( float totalHeight );

  Signal1< void, int /* percent */ > SignalScroll;

private:
  bool                      m_IsMouseDown;
  float                     m_TotalSize;
  float                     m_Scrollpos;
  float                     m_MouseStartPos;

  SCROLL_DIRECTION          m_ScrollDirection;
  CoreGuiImageView*         m_Slider;
};

#endif /// EVILENGINE_COREGUITOOLKIT_BASEGUIELEMENTS_COREGUISCROLLBAR
