#ifndef EVILENGINE_COREGUITOOLKIT_BASEGUIELEMENTS_COREGUIXMLVIEW
#define EVILENGINE_COREGUITOOLKIT_BASEGUIELEMENTS_COREGUIXMLVIEW 1

#include "CoreGuiBase.h"
#include "ConnectButtonTemplate.h"
#include "EvilEngine/CoreLib/Signal/SignalTarget.h"
#include "EvilEngine/CoreGuiToolkit/BaseGuiElements/CoreGuiButton.h"
#include "EvilEngine/CoreEngine/CoreResourceManager/Resource.h"

class CoreGuiXMLView : public ConnectButtonTemplate, public SignalTarget
{
public:

  CoreGuiXMLView( const char* xmlResourceName );

  template <typename OBJ_CLASS>
  void SetCallback( void ( OBJ_CLASS::*callback)() ) const
  {
    m_Callback.Connect( callback );
  }

  virtual bool                OnDraw( const Matrix& projection ) DF_OVERRIDE;

#if defined DEBUG
  virtual String              GetStaticClassName() DF_OVERRIDE { return "CoreGuiXMLView"; }
#endif

protected:

  virtual void                OnDataLoaded( Resource* /*resource*/ ){};
  void                        InternalOnDataLoaded( Resource* resource );

  bool                        m_IsLoaded;
  String                      m_XMLResourceName;
  Signal0< void >             m_Callback;
};

#endif /// EVILENGINE_COREGUITOOLKIT_BASEGUIELEMENTS_COREGUIXMLVIEW