#ifndef EVILENGINE_CORELIB_SIGNAL_SIGNALTARGET
#define EVILENGINE_CORELIB_SIGNAL_SIGNALTARGET 1
#pragma once

class SignalBase;
class SlotBase;

class SignalTarget
{
public:
  SignalTarget();
  SignalTarget( const SignalTarget& );
  virtual ~SignalTarget();

  SignalTarget& operator=( const SignalTarget& )
  {
    return *this;
  }

  void DisconnectAllSignals();

private:
  friend class SlotBase;

  void RegisterSlot( SlotBase* slot );
  void UnregisterSlot( SlotBase* slot );

  SlotBase*     m_FirstSlot;
};

#endif /// EVILENGINE_CORELIB_SIGNAL_SIGNALTARGET
