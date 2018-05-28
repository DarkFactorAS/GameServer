/*************************************************************************************************
* Project     : EvilEngine Multiplatform 2014
 * File        : SlideGuiView
 *
 * Description : Slides a panel from outside of the screen to be visible on screen. This class 
 *               handles the smooth sliding/moving
 *
 * Author      : Thor Richard Hansen
 *************************************************************************************************/

#include "PreCompile.h"
#include "SlideGuiView.h"

#define SPACER 0.01f

SlideGuiView::SlideGuiView( int offscreenPosition, float slideSpeed ) :
  m_CurrentActions( 0 ),
  m_WantedActions( 0 ),
  m_IsLoaded( false ),
  m_Width( 0.0f ),
  m_SlideSpeed( slideSpeed ),
  m_OffscreenPosition( offscreenPosition )
{

}

void SlideGuiView::Open()
{
  if ( m_SlideDirection != SlideDirection_Opening && m_SlideDirection != SlideDirection_Open )
  {
    m_SlideDirection = SlideDirection_Opening;
  }
}

void SlideGuiView::SetOpen()
{
  m_SlideDirection = SlideDirection_Open;
}

void SlideGuiView::Close()
{
  if ( m_SlideDirection != SlideDirection_Closing && m_SlideDirection != SlideDirection_Closed )
  {
    m_SlideDirection = SlideDirection_Closing;
  }
  else
  {
    SetClosed();
  }
}

void SlideGuiView::SetClosed()
{
  m_SlideDirection  = SlideDirection_Closed;
  m_CurrentActions  = 0;
}

void SlideGuiView::FrameProcess( float deltaTime )
{
  if ( !m_IsLoaded )
  {
    return;
  }

  switch( m_SlideDirection )
  {
  case SlideDirection_Opening:
    {
      switch( m_OffscreenPosition )
      {
        case OffscreenPosition_Left:
          {
            float posX = m_Position.x + ( m_SlideSpeed * deltaTime );
            if ( m_Position.x > m_Width + SPACER - 1.0f )
            {
              SetPosition( m_Width + SPACER - 1.0f, m_Position.y );
              SetOpen();
            }
            else
            {
              SetPosition( posX, m_Position.y );
            }
          }
          break;
        case OffscreenPosition_Right:
          {
            float posX = m_Position.x - ( m_SlideSpeed * deltaTime );
            if ( m_Position.x < 1.0f - m_Width - 0.01f )
            {
              SetPosition( 1.0f - m_Width - SPACER, m_Position.y );
              SetOpen();
            }
            else
            {
              SetPosition( posX, m_Position.y );
            }
          }
          break;
      }
    }
    break;

  case SlideDirection_Closing:
    {
      switch( m_OffscreenPosition )
      {
        case OffscreenPosition_Left:
          {
            float posX = m_Position.x - ( m_SlideSpeed * deltaTime );
            if ( posX < -1.0f - m_Width )
            {
              SetPosition( -1.0f - m_Width, m_Position.y );
              SetClosed();
              if ( m_WantedActions != 0 )
              {
                Open();
              }
            }
            else
            {
              SetPosition( posX, m_Position.y );
            }
          }
          break;

        case OffscreenPosition_Right:
          {
            float posX = m_Position.x + ( m_SlideSpeed * deltaTime );
            if ( posX > 1.0f + m_Width )
            {
              SetPosition( 1.0f + m_Width, m_Position.y );
              SetClosed();
              if ( m_WantedActions != 0 )
              {
                Open();
              }
            }
            else
            {
              SetPosition( posX, m_Position.y );
            }
          }
          break;
      }
    }
    break;

  default:
    break;
  }
}
