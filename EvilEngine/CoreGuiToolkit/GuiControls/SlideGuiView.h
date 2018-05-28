#ifndef EVILENGINE_COREGUITOOLKIT_GUICONTROLS_SLIDEGUIWINDOW
#define EVILENGINE_COREGUITOOLKIT_GUICONTROLS_SLIDEGUIWINDOW

#include "EvilEngine/CoreGuiToolkit/CoreGuiToolkitInclude.h"

class SlideGuiView: public CoreGuiBase
{
public:

  enum SlideDirection
  {
    SlideDirection_Closed = 0,
    SlideDirection_Closing,
    SlideDirection_Opening,
    SlideDirection_Open,
  };

  enum OffscreenPosition
  {
    OffscreenPosition_Left,
    OffscreenPosition_Top,
    OffscreenPosition_Right,
    OffscreenPosition_Bottom,
  };

  SlideGuiView( int offscreenPosition, float slideSpeed );

  virtual void                        FrameProcess( float deltaTime ) DF_OVERRIDE;
  void                                UpdateActions( uint32 actions );

protected:

  virtual void                        Close();
  virtual void                        SetClosed();
  virtual void                        SetOpen();
  virtual void                        Open();


  uint32                              m_CurrentActions;
  uint32                              m_WantedActions;

  uint32                              m_SlideDirection;
  bool                                m_IsLoaded;
  float                               m_Width;
  float                               m_SlideSpeed;
  int                                 m_OffscreenPosition;
};

#endif /// EVILENGINE_COREGUITOOLKIT_GUICONTROLS_SLIDEGUIWINDOW
