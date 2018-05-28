
#include "Precompile.h"
#include "SlotBase.h"
#include "SignalBase.h"
#include "SignalTarget.h"

SlotBase::SlotBase( SignalBase* targetSignal, SignalTarget* object, int argCount ) :
  m_Signal(targetSignal), m_Object(object), m_ArgCount(argCount)
{
  m_PrevInSignal = NULL;
  m_NextInSignal = NULL;
  m_PrevInTarget = NULL;
  m_NextInTarget = NULL;

  if ( m_Object != NULL ) 
  {
    m_Object->RegisterSlot( this );
  }

}

SlotBase::~SlotBase()
{
  if ( m_Signal != NULL ) 
  {
    m_Signal->DisconnectInternal( this, false );
  }
  if ( m_Object != NULL ) 
  {
    m_Object->UnregisterSlot( this );
  }
}

SlotBase* SlotBase::GetLastInSignal()
{
  if ( m_NextInSignal == NULL )
  {
    return this;
  }
  return m_NextInSignal->GetLastInSignal();
}