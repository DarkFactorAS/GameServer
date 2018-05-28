
#include "Precompile.h"
#include "SignalBase.h"
#include "SlotBase.h"

SignalBase::SignalBase()
{
  m_FirstSlot = NULL;
}

SignalBase::SignalBase( const SignalBase& other )
{
  Copy( other );
}

SignalBase::~SignalBase()
{
  //for ( EmitGuard* guard = GetThisThreadEmitGuardPtr() ; guard != NULL ; guard = guard->m_Next ) {
  //  if ( guard->m_Signal == this ) {
  //    guard->m_Signal = NULL; // Make sure Emit() don't screw up.
  //  }
  //}
  DisconnectAll();
}

void SignalBase::DisconnectAll()
{
  while( m_FirstSlot != NULL ) 
  {
    DisconnectInternal( m_FirstSlot );
  }
}

SignalBase& SignalBase::operator=( const SignalBase& i_cOther )
{
  while( m_FirstSlot != NULL ) 
  {
    DisconnectInternal( m_FirstSlot );
  }
  for ( SlotBase* pcSlot = i_cOther.m_FirstSlot ; pcSlot != NULL ; pcSlot = pcSlot->m_NextInSignal ) 
  {
    ConnectInternal( pcSlot->Clone( this ) );
  }
  return *this;
}

void SignalBase::Copy( const SignalBase& other )
{
  for ( SlotBase* pcSlot = other.m_FirstSlot ; pcSlot != NULL ; pcSlot = pcSlot->m_NextInSignal ) 
  {
    if ( !IsSlotConnected( pcSlot ) )
    {
      ConnectInternal( pcSlot->Clone( this ) );
    }
  }
}

void SignalBase::ConnectInternal( SlotBase* slot ) const
{
  if ( m_FirstSlot != NULL ) 
  {
    m_FirstSlot->m_PrevInSignal = slot;
  }

  slot->m_NextInSignal = m_FirstSlot;
  slot->m_PrevInSignal = NULL;

  m_FirstSlot = slot;
}

void SignalBase::DisconnectInternal( SlotBase* slot, bool deleteSlot ) const
{
  if ( slot == NULL ) 
  {
    return;
  }

  if ( slot->m_PrevInSignal != NULL ) 
  {
    slot->m_PrevInSignal->m_NextInSignal = slot->m_NextInSignal;
  } 
  else 
  {
    m_FirstSlot = slot->m_NextInSignal;
  }
  if ( slot->m_NextInSignal != NULL ) 
  {
    slot->m_NextInSignal->m_PrevInSignal = slot->m_PrevInSignal;
  }
  slot->m_Signal = NULL;

  if ( deleteSlot ) 
  {
    delete slot;
    slot = NULL;
  }
}

bool SignalBase::IsSlotConnected( SlotBase* checkSlot ) const
{
  for ( SlotBase* slot = m_FirstSlot ; slot != NULL ; slot = slot->m_NextInSignal ) 
  {
    if ( slot->GetArgCount() == checkSlot->GetArgCount() && slot->m_Object == checkSlot->m_Object )
    {
      return true;
    }
  }
  return false;
}

/***********************************************************************************
* IsSlotLast:
* Check if the target object is the last in the call list ( Used to check if it is
* the one that is actually called in a VFConnector )
*
* @param  (SlotBase*)   slotBase - The slot to check 
* @return (bool)                 - Returns true if the connector is last in the list
*
* @author Thor Richard Hansen
***********************************************************************************/
#ifdef DEBUG
bool SignalBase::IsSlotLast(SlotBase* checkSlot) const
{
  for (SlotBase* slot = m_FirstSlot; slot != NULL; slot = slot->m_NextInSignal)
  {
    if (slot->GetArgCount() == checkSlot->GetArgCount() && slot->m_Object == checkSlot->m_Object)
    {
      return ( slot->m_NextInSignal == NULL );
    }
  }
  return false;
}
#endif