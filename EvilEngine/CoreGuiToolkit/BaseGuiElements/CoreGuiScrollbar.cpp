/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014-2016
* File        : CoreGuiScrollbar
* Description : Creates the behavior of a scrollbar as a gui object.
*               
* Author      : Thor Richard Hansen
*************************************************************************************************/

#include "Precompile.h"
#include "CoreGuiScrollbar.h"
#include "EvilEngine/CoreGuiToolkit/BaseGuiElements/CoreGuiImageView.h"

CoreGuiScrollbar::CoreGuiScrollbar( const Vector2& position, const Vector2& size, const Color& color, SCROLL_DIRECTION scrollDirection ) :
  CoreGuiBase(position,size),
  m_IsMouseDown( false ),
  m_ScrollDirection( scrollDirection )
{
  SetName("Scrollbar");

  float sliderWidth = size.x - ( 0.4f * size.x );
  float sliderHeight = size.y;// - ( 0.1f * size.y );

  CoreGuiBase* frame = AddChild( new CoreGuiImageView( Vector2(position.x,position.y), Vector2(sliderWidth, sliderHeight ), 0 ));
  frame->ShowBorders(true);
  frame->SetBorderColor( Color::Black );

  m_Slider = new CoreGuiImageView( Vector2(position.x,position.y), Vector2( sliderWidth, sliderHeight ), color );
  m_Slider->ShowBorders(true);
  m_Slider->SetBorderColor(Color::DarkGrey);
  AddChild( m_Slider );

  
  
  m_MouseStartPos = position.y;
  
  //ShowBorders(true);
  //SetBorderColor(Color::Black);

}

CoreGuiScrollbar::~CoreGuiScrollbar()
{
}

CoreGuiBase* CoreGuiScrollbar::OnMouseButtonDown(int /*buttonId*/, float xPos, float yPos)
{
  if (IsIntersecting(xPos, yPos))
  {
    m_IsMouseDown   = true;
    m_MouseStartPos = ( m_ScrollDirection == SCROLL_HORIZONTAL ) ? xPos : yPos;
    return this;
  }
  return NULL;
}

bool CoreGuiScrollbar::OnMouseDrag( float  /*xPos*/ , float  yPos  )
{
  if ( m_IsMouseDown )
  {
    //if ( yPos > )

    Move( 0, yPos, false );
    return true;
  }
  return false;
}

CoreGuiBase* CoreGuiScrollbar::OnMouseButtonUp(int /*buttonId*/, float xPos, float yPos)
{
  if (m_IsMouseDown)
  {
    m_IsMouseDown = false;

    const Vector2& pos = GetPosition();
    const Vector2& sliderSize = m_Slider->GetSize();
    float maxScrollDelta = (m_ScrollDirection == SCROLL_HORIZONTAL) ? pos.x - sliderSize.x : pos.y - sliderSize.y;
    float scrollDelta = ( m_ScrollDirection == SCROLL_HORIZONTAL ) ? xPos - m_MouseStartPos : yPos - m_MouseStartPos;

    if ( scrollDelta > maxScrollDelta )
    {
      scrollDelta = 0.0f;
    }

    SignalScroll( 10 );

    return this;
  }
  return NULL;
}

bool CoreGuiScrollbar::OnMouseButtonWheel(float delta, const Vector2& /*mousePos*/)
{
  //const Vector2& pos        = GetPosition();
  const Vector2& size       = GetSize();
  const Vector2& sliderSize = m_Slider->GetSize();
  float scrollSize          = ( m_ScrollDirection == SCROLL_HORIZONTAL ) ? sliderSize.x : sliderSize.y; 
  float scrollDelta         = ( m_ScrollDirection == SCROLL_HORIZONTAL ) ? abs(size.x  - sliderSize.x) : abs(size.y - sliderSize.y);

  // each tick is 10%
  if ( delta > 0.0f )
  {
    m_Scrollpos += scrollSize * 0.1f;
    if ( m_Scrollpos > scrollDelta  * 2.0f )
    {
      m_Scrollpos = scrollDelta * 2.0f;
    }
  }
  else if ( delta < 0.0f )
  {
    m_Scrollpos -= scrollSize * 0.1f;
    if ( m_Scrollpos < 0.0f )
    {
      m_Scrollpos = 0.0f;
    }
  }
  SignalScroll( int(100 * m_Scrollpos / size.y ) );

  m_Slider->SetPosition(m_Slider->GetPosition().x, GetPosition().y - GetSize().y + m_Slider->GetSize().y + m_Scrollpos);
  return true;
}

void CoreGuiScrollbar::SetTotalSize( float totalSize )
{
  m_TotalSize = totalSize;

  if ( m_TotalSize > 0.0f )
  {
    m_Slider->SetVisible(true);

    Vector2 size  = GetSize();
    Vector2 pos   = GetPosition();

    switch( m_ScrollDirection )
    {
      case SCROLL_HORIZONTAL:
        {
          float percent = size.x / m_TotalSize;
          m_Slider->SetSize(size.x * percent, size.y);
          m_Slider->SetPosition(pos.x + size.x - ( size.x * percent ), pos.y);
        }
        break;
      case SCROLL_VERTICAL:
        {
          float percent = size.y / m_TotalSize;
          m_Slider->SetSize( m_Slider->GetSize().x, size.y * percent);
          m_Slider->SetPosition( m_Slider->GetPosition().x, pos.y - size.y + (size.y * percent));
        }
        break;
    }
  }
  else
  {
    m_Slider->SetVisible(false);
  }
}
