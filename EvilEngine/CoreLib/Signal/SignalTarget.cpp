
#include "Precompile.h"
#include "SignalTarget.h"
#include "SignalBase.h"
#include "SlotBase.h"

SignalTarget::SignalTarget()
{
  m_FirstSlot = NULL;
}
SignalTarget::SignalTarget( const SignalTarget& )
{
  m_FirstSlot = NULL;
}

SignalTarget::~SignalTarget()
{
  DisconnectAllSignals();
}

void SignalTarget::DisconnectAllSignals()
{
  while( m_FirstSlot != NULL ) 
  {
    delete m_FirstSlot; 
  }
}


void SignalTarget::RegisterSlot( SlotBase* slot )
{
  if ( m_FirstSlot != NULL )
  {
    m_FirstSlot->m_PrevInTarget = slot;
  }

  slot->m_NextInTarget = m_FirstSlot;
  slot->m_PrevInTarget = NULL;

  m_FirstSlot = slot;
  slot->m_Object  = this;
}

void SignalTarget::UnregisterSlot( SlotBase* slot )
{
  if ( slot->m_PrevInTarget != NULL ) 
  {
    slot->m_PrevInTarget->m_NextInTarget = slot->m_NextInTarget;
  } 
  else 
  {
    m_FirstSlot = slot->m_NextInTarget;
  }
  
  if ( slot->m_NextInTarget != NULL ) 
  {
    slot->m_NextInTarget->m_PrevInTarget = slot->m_PrevInTarget;
  }  
  slot->m_Object = NULL;
}
