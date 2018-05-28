#ifndef EVILGAMEENGINE_COREGAMELOGIN_GUI_GMPASSWORDWINDOW
#define EVILGAMEENGINE_COREGAMELOGIN_GUI_GMPASSWORDWINDOW 1

#include "EvilEngine/CoreGuiToolkit/BaseGuiElements/CoreGuiXMLView.h"

class GMPasswordWindow : public CoreGuiXMLView
{
public:
  GMPasswordWindow( const String& windowName );

  virtual void  OnDataLoaded( Resource* /*resource*/ ) DF_OVERRIDE;

  Signal1< void, String > SignalPassword;

private:
  void          Cancel( int btnId );
  void          VerifyPassword( int btnId );
};

#endif /// EVILGAMEENGINE_COREGAMELOGIN_GUI_GMPASSWORDWINDOW
