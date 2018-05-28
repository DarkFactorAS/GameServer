#ifndef EVILENGINE_CORELIB_SIGNAL_SIGNALBASE
#define EVILENGINE_CORELIB_SIGNAL_SIGNALBASE 1
#pragma once

#include "SlotBase.h"

class SignalBase
{
public:
  SignalBase();
  SignalBase( const SignalBase& other );

  void DisconnectAll();
  bool Empty() const { return m_FirstSlot == 0; }
  void Copy( const SignalBase& other );
  SignalBase& operator=( const SignalBase& other );

protected:
  ~SignalBase();

  void ConnectInternal( SlotBase* slot ) const;
  void DisconnectInternal( SlotBase* slot, bool deleteSlot = true ) const;
  bool IsSlotConnected( SlotBase* checkSlot ) const;

#ifdef DEBUG
  bool IsSlotLast(SlotBase* checkSlot) const;
#endif

  struct EmitGuard 
  {
    EmitGuard( const SignalBase* targetSignal, SlotBase* slotIterator ) : m_SlotIterator(slotIterator) 
    {
      m_Signal = targetSignal;
      m_Next = GetThisThreadEmitGuardPtr();
      SetThisThreadEmitGuardPtr( this );
    }
    ~EmitGuard() 
    {
      SetThisThreadEmitGuardPtr( m_Next );
    }
    SlotBase*         m_SlotIterator;
    const SignalBase* m_Signal;
    EmitGuard*        m_Next;
  };

  struct PrevSlotGuard 
  {
    PrevSlotGuard( EmitGuard* guard ) 
    {
      m_Guard = guard;
      m_Guard->m_SlotIterator = m_Guard->m_SlotIterator->GetNextInSignal();
    }
    ~PrevSlotGuard() 
    {
      if ( m_Guard->m_Signal != 0 && m_Guard->m_SlotIterator != 0 ) {
        m_Guard->m_SlotIterator = m_Guard->m_SlotIterator->GetPrevInSignal();
      }
    }
    EmitGuard* m_Guard;
  };

  static  EmitGuard*  GetThisThreadEmitGuardPtr();
  static  void        SetThisThreadEmitGuardPtr( SignalBase::EmitGuard* guard );
  SlotBase*           GetFirstSlot() const { return m_FirstSlot; }

  mutable SlotBase*   m_FirstSlot;

private:
  friend class  SlotBase;
  friend struct EmitGuard;
};

#endif /// EVILENGINE_CORELIB_SIGNAL_SIGNALBASE