
#include "CoreGuiBase.h"
#include "CoreGuiButton.h"

class ConnectButtonTemplate : public CoreGuiBase
{
public:
  template <typename OBJ_CLASS>
  void ConnectButton( const String& buttonName, void ( OBJ_CLASS::*callback)(int) )
  {
    CoreGuiButton* button = (CoreGuiButton*) GetChildWithName( buttonName );
    if ( button != NULL )
    {
      button->m_SignalButtonClicked.Connect( this, callback );
    }
  }
};