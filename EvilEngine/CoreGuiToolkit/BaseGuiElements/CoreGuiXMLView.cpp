
#include "Precompile.h"
#include "CoreGuiXMLView.h"
#include "../GuiCreator/GuiResource.h"
#include "EvilEngine/CoreEngine/CoreResourceManager/CoreResourceManager.h"

CoreGuiXMLView::CoreGuiXMLView( const char* xmlResourceName ) :
  m_IsLoaded( false ),
  m_XMLResourceName( xmlResourceName )
{
  CoreResourceManager::GetInstance()->AsyncLoadResource( new GuiResource( this, xmlResourceName ), 0, this, &CoreGuiXMLView::InternalOnDataLoaded );
}

bool CoreGuiXMLView::OnDraw( const Matrix& projection )
{
  if ( !m_IsLoaded )
  {
    return false;
  }
  return CoreGuiBase::OnDraw( projection );
}

void CoreGuiXMLView::InternalOnDataLoaded( Resource* resource )
{
  OnDataLoaded( resource );
  m_IsLoaded = true;
}