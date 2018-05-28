#ifndef EVILENGINE_CORELIB_SIGNAL_SIGNAL
#define EVILENGINE_CORELIB_SIGNAL_SIGNAL 1
#pragma once

#include "SignalBase.h"
#include "SlotBase.h"

template<typename RETURN_TYPE>
class Signal0 : public SignalBase
{
public:

  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Connect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)() ) const
  {
    SignalBase*   signalBase   = const_cast<SignalBase*>(static_cast<const SignalBase*>(this) );
    SignalTarget* signalTarget = const_cast<OBJ_CLASS*>( static_cast<const OBJ_CLASS*>(obj) );
    Slot0<RETURN_TYPE,OBJ_CLASS>* slotBase = new Slot0<RETURN_TYPE,OBJ_CLASS>( signalBase, signalTarget, callback );
    ConnectInternal( slotBase );
  }

  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Disconnect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)() ) const
  {
    obj = obj;
    for ( SlotBase* i = m_FirstSlot; i != nullptr; i = i->GetNextInSignal() )
    {
      Slot0<RETURN_TYPE, OBJ_CLASS>* slotBase = const_cast< Slot0< RETURN_TYPE, OBJ_CLASS >*> ( static_cast< Slot0< RETURN_TYPE, OBJ_CLASS >* >(i) );
      if ( slotBase != nullptr && slotBase->m_Callback == callback )
      {
        DisconnectInternal( slotBase );
        return;
      }
    }
  }

  // 0 Arguments
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Reconnect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)() ) const
  {
    Disconnect( obj, callback );
    Connect( obj, callback );
  }

  bool operator()()
  {
    bool hasReceiver = false;
    SlotBase* slot      = m_FirstSlot;
    SlotBase* nextSlot  = nullptr;
    while ( slot != 0 )
    {
      hasReceiver = true;
      nextSlot    = slot->GetNextInSignal();
      SlotBase0<RETURN_TYPE>* caller = const_cast< SlotBase0<RETURN_TYPE>* > ( static_cast< SlotBase0<RETURN_TYPE>* >( slot ) );
      if ( caller != nullptr )
      {
        caller->Call();
      }
      slot = nextSlot;
    }
    return hasReceiver;
  };
};

template<typename RETURN_TYPE, typename ARG1>
class Signal1 : public SignalBase
{
public:

  // 0 Arguments
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Connect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)() ) const
  {
    SignalBase*   signalBase   = const_cast<SignalBase*>(static_cast<const SignalBase*>(this) );
    SignalTarget* signalTarget = const_cast<OBJ_CLASS*>( static_cast<const OBJ_CLASS*>(obj) );
    Slot0<RETURN_TYPE,OBJ_CLASS>* slotBase = new Slot0<RETURN_TYPE,OBJ_CLASS>( signalBase, signalTarget, callback );
    ConnectInternal( slotBase );
  }

  // 0 Arguments
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Disconnect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)() ) const
  {
    obj = obj;
    for ( SlotBase* i = m_FirstSlot; i != nullptr; i = i->GetNextInSignal() )
    {
      Slot0<RETURN_TYPE, OBJ_CLASS>* slotBase = const_cast< Slot0< RETURN_TYPE, OBJ_CLASS >*> ( static_cast< Slot0< RETURN_TYPE, OBJ_CLASS >* >(i) );
      if ( slotBase != nullptr && slotBase->m_Callback == callback )
      {
        DisconnectInternal( slotBase );
        return;
      }
    }
  }

  // 0 Arguments
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Reconnect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)() ) const
  {
    Disconnect( obj, callback );
    Connect( obj, callback );
  }

  // 1 Argument
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Connect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)(ARG1) ) const
  {
    SignalBase*   signalBase   = const_cast<SignalBase*>(static_cast<const SignalBase*>(this) );
    SignalTarget* signalTarget = const_cast<OBJ_CLASS*>( static_cast<const OBJ_CLASS*>(obj) );
    Slot1<RETURN_TYPE,OBJ_CLASS, ARG1>* slotBase = new Slot1<RETURN_TYPE,OBJ_CLASS,ARG1>( signalBase, signalTarget, callback );
    ConnectInternal( slotBase );
  }

  // 1 Argument
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Disconnect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)(ARG1) ) const
  {
    obj = obj;
    for ( SlotBase* i = m_FirstSlot; i != nullptr; i = i->GetNextInSignal() )
    {
      Slot1<RETURN_TYPE, OBJ_CLASS,ARG1>* slotBase = const_cast< Slot1< RETURN_TYPE, OBJ_CLASS, ARG1 >*> ( static_cast< Slot1< RETURN_TYPE, OBJ_CLASS, ARG1 >* >(i) );
      if ( slotBase != nullptr && slotBase->m_Callback == callback )
      {
        DisconnectInternal( slotBase );
        return;
      }
    }
  }

  // 1 Argument
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Reconnect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)(ARG1) ) const
  {
    Disconnect( obj, callback );
    Connect( obj, callback );
  }

  bool operator()(ARG1 arg1)
  {
    bool hasReceiver = false;
    SlotBase* slot      = m_FirstSlot;
    SlotBase* nextSlot  = nullptr;
    while ( slot != 0 )
    {
      nextSlot    = slot->GetNextInSignal();
      hasReceiver = true;
      switch( slot->GetArgCount() )
      {
        case 0:
          {
            SlotBase0<RETURN_TYPE>* caller = const_cast< SlotBase0<RETURN_TYPE>* > ( static_cast< SlotBase0<RETURN_TYPE>* >( slot ) );
            caller->Call();
          }
          break;
        case 1:
          {
            SlotBase1<RETURN_TYPE, ARG1>* caller = const_cast< SlotBase1<RETURN_TYPE, ARG1>* > ( static_cast< SlotBase1<RETURN_TYPE, ARG1>* >( slot ) );
            caller->Call( arg1 );
          }
          break;
      }
      slot = nextSlot;
    }
    return hasReceiver;
  };
};

template<typename RETURN_TYPE, typename ARG1, typename ARG2>
class Signal2 : public SignalBase
{
public:

  // 0 Arguments
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Connect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)() ) const
  {
    SignalBase*   signalBase   = const_cast<SignalBase*>(static_cast<const SignalBase*>(this) );
    SignalTarget* signalTarget = const_cast<OBJ_CLASS*>( static_cast<const OBJ_CLASS*>(obj) );
    Slot0<RETURN_TYPE,OBJ_CLASS>* slotBase = new Slot0<RETURN_TYPE,OBJ_CLASS>( signalBase, signalTarget, callback );
    ConnectInternal( slotBase );
  }

  // 0 Arguments
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Disconnect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)() ) const
  {
    obj = obj;
    for ( SlotBase* i = m_FirstSlot; i != nullptr; i = i->GetNextInSignal() )
    {
      Slot0<RETURN_TYPE, OBJ_CLASS>* slotBase = const_cast< Slot0< RETURN_TYPE, OBJ_CLASS >*> ( static_cast< Slot0< RETURN_TYPE, OBJ_CLASS >* >(i) );
      if ( slotBase != nullptr && slotBase->m_Callback == callback )
      {
        DisconnectInternal( slotBase );
        return;
      }
    }
  }

  // 0 Arguments
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Reconnect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)() ) const
  {
    Disconnect( obj, callback );
    Connect( obj, callback );
  }

  // 1 Argument
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Connect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)(ARG1) ) const
  {
    SignalBase*   signalBase   = const_cast<SignalBase*>(static_cast<const SignalBase*>(this) );
    SignalTarget* signalTarget = const_cast<OBJ_CLASS*>( static_cast<const OBJ_CLASS*>(obj) );
    Slot1<RETURN_TYPE,OBJ_CLASS, ARG1>* slotBase = new Slot1<RETURN_TYPE,OBJ_CLASS,ARG1>( signalBase, signalTarget, callback );
    ConnectInternal( slotBase );
  }

  // 1 Argument
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Disconnect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)(ARG1) ) const
  {
    obj = obj;
    for ( SlotBase* i = m_FirstSlot; i != nullptr; i = i->GetNextInSignal() )
    {
      Slot1<RETURN_TYPE, OBJ_CLASS,ARG1>* slotBase = const_cast< Slot1< RETURN_TYPE, OBJ_CLASS, ARG1 >*> ( static_cast< Slot1< RETURN_TYPE, OBJ_CLASS, ARG1 >* >(i) );
      if ( slotBase != nullptr && slotBase->m_Callback == callback )
      {
        DisconnectInternal( slotBase );
        return;
      }
    }
  }

  // 1 Argument
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Reconnect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)(ARG1) ) const
  {
    Disconnect( obj, callback );
    Connect( obj, callback );
  }

  // 2 Arguments
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Connect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)(ARG1,ARG2) ) const
  {
    SignalBase*   signalBase   = const_cast<SignalBase*>(static_cast<const SignalBase*>(this) );
    SignalTarget* signalTarget = const_cast<OBJ_CLASS*>( static_cast<const OBJ_CLASS*>(obj) );
    Slot2<RETURN_TYPE,OBJ_CLASS, ARG1,ARG2>* slotBase = new Slot2<RETURN_TYPE,OBJ_CLASS,ARG1,ARG2>( signalBase, signalTarget, callback );
    ConnectInternal( slotBase );
  }

  // 2 Arguments
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Disconnect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)(ARG1,ARG2) ) const
  {
    obj = obj;
    for ( SlotBase* i = m_FirstSlot; i != nullptr; i = i->GetNextInSignal() )
    {
      Slot2<RETURN_TYPE, OBJ_CLASS,ARG1, ARG2>* slotBase = const_cast< Slot2< RETURN_TYPE, OBJ_CLASS, ARG1, ARG2 >*> ( static_cast< Slot2< RETURN_TYPE, OBJ_CLASS, ARG1, ARG2 >* >(i) );
      if ( slotBase != nullptr && slotBase->m_Callback == callback )
      {
        DisconnectInternal( slotBase );
        return;
      }
    }
  }

  // 2 Arguments
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Reconnect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)(ARG1,ARG2) ) const
  {
    Disconnect( obj, callback );
    Connect( obj, callback );
  }

  bool operator()(ARG1 arg1, ARG2 arg2)
  {
    bool hasReceiver    = false;
    SlotBase* slot      = m_FirstSlot;
    SlotBase* nextSlot  = nullptr;
    while ( slot != 0 )
    {
      nextSlot = slot->GetNextInSignal();
      hasReceiver = true;
      switch( slot->GetArgCount() )
      {
        case 0:
          {
            SlotBase0<RETURN_TYPE>* caller = const_cast< SlotBase0<RETURN_TYPE>* > ( static_cast< SlotBase0<RETURN_TYPE>* >( slot ) );
            caller->Call();
          }
          break;
        case 1:
          {
            SlotBase1<RETURN_TYPE, ARG1>* caller = const_cast< SlotBase1<RETURN_TYPE, ARG1>* > ( static_cast< SlotBase1<RETURN_TYPE, ARG1>* >( slot ) );
            caller->Call( arg1 );
          }
          break;
        case 2:
          {
            SlotBase2<RETURN_TYPE, ARG1, ARG2>* caller = const_cast< SlotBase2<RETURN_TYPE, ARG1, ARG2>* > ( static_cast< SlotBase2<RETURN_TYPE, ARG1, ARG2>* >( slot ) );
            caller->Call( arg1, arg2 );
          }
          break;
      }
      slot = nextSlot;
    }

    return hasReceiver;
  };
};

template<typename RETURN_TYPE, typename ARG1, typename ARG2, typename ARG3 >
class Signal3 : public SignalBase
{
public:

  // 0 Arguments
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Connect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)() ) const
  {
    SignalBase*   signalBase   = const_cast<SignalBase*>(static_cast<const SignalBase*>(this) );
    SignalTarget* signalTarget = const_cast<OBJ_CLASS*>( static_cast<const OBJ_CLASS*>(obj) );
    Slot0<RETURN_TYPE,OBJ_CLASS>* slotBase = new Slot0<RETURN_TYPE,OBJ_CLASS>( signalBase, signalTarget, callback );
    ConnectInternal( slotBase );
  }

  // 0 Arguments
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Disconnect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)() ) const
  {
    obj = obj;
    for ( SlotBase* i = m_FirstSlot; i != nullptr; i = i->GetNextInSignal() )
    {
      Slot0<RETURN_TYPE, OBJ_CLASS>* slotBase = const_cast< Slot0< RETURN_TYPE, OBJ_CLASS >*> ( static_cast< Slot0< RETURN_TYPE, OBJ_CLASS >* >(i) );
      if ( slotBase != nullptr && slotBase->m_Callback == callback )
      {
        DisconnectInternal( slotBase );
        return;
      }
    }
  }

  // 0 Arguments
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Reconnect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)() ) const
  {
    Disconnect( obj, callback );
    Connect( obj, callback );
  }

  // 1 Argument
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Connect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)(ARG1) ) const
  {
    SignalBase*   signalBase   = const_cast<SignalBase*>(static_cast<const SignalBase*>(this) );
    SignalTarget* signalTarget = const_cast<OBJ_CLASS*>( static_cast<const OBJ_CLASS*>(obj) );
    Slot1<RETURN_TYPE,OBJ_CLASS, ARG1>* slotBase = new Slot1<RETURN_TYPE,OBJ_CLASS,ARG1>( signalBase, signalTarget, callback );
    ConnectInternal( slotBase );
  }

  // 1 Argument
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Disconnect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)(ARG1) ) const
  {
    obj = obj;
    for ( SlotBase* i = m_FirstSlot; i != nullptr; i = i->GetNextInSignal() )
    {
      Slot1<RETURN_TYPE, OBJ_CLASS,ARG1>* slotBase = const_cast< Slot1< RETURN_TYPE, OBJ_CLASS, ARG1 >*> ( static_cast< Slot1< RETURN_TYPE, OBJ_CLASS, ARG1 >* >(i) );
      if ( slotBase != nullptr && slotBase->m_Callback == callback )
      {
        DisconnectInternal( slotBase );
        return;
      }
    }
  }

  // 1 Argument
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Reconnect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)(ARG1) ) const
  {
    Disconnect( obj, callback );
    Connect( obj, callback );
  }

  // 2 Arguments
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Connect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)(ARG1,ARG2) ) const
  {
    SignalBase*   signalBase   = const_cast<SignalBase*>(static_cast<const SignalBase*>(this) );
    SignalTarget* signalTarget = const_cast<OBJ_CLASS*>( static_cast<const OBJ_CLASS*>(obj) );
    Slot2<RETURN_TYPE,OBJ_CLASS, ARG1,ARG2>* slotBase = new Slot2<RETURN_TYPE,OBJ_CLASS,ARG1,ARG2>( signalBase, signalTarget, callback );
    ConnectInternal( slotBase );
  }

  // 2 Arguments
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Disconnect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)(ARG1,ARG2) ) const
  {
    obj = obj;
    for ( SlotBase* i = m_FirstSlot; i != nullptr; i = i->GetNextInSignal() )
    {
      Slot2<RETURN_TYPE, OBJ_CLASS,ARG1, ARG2>* slotBase = const_cast< Slot2< RETURN_TYPE, OBJ_CLASS, ARG1, ARG2 >*> ( static_cast< Slot2< RETURN_TYPE, OBJ_CLASS, ARG1, ARG2 >* >(i) );
      if ( slotBase != nullptr && slotBase->m_Callback == callback )
      {
        DisconnectInternal( slotBase );
        return;
      }
    }
  }

  // 2 Arguments
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Reconnect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)(ARG1,ARG2) ) const
  {
    Disconnect( obj, callback );
    Connect( obj, callback );
  }

  // 3 Arguments
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Connect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)(ARG1,ARG2,ARG3) ) const
  {
    SignalBase*   signalBase   = const_cast<SignalBase*>(static_cast<const SignalBase*>(this) );
    SignalTarget* signalTarget = const_cast<OBJ_CLASS*>( static_cast<const OBJ_CLASS*>(obj) );
    Slot3<RETURN_TYPE,OBJ_CLASS, ARG1,ARG2,ARG3>* slotBase = new Slot3<RETURN_TYPE,OBJ_CLASS,ARG1,ARG2,ARG3>( signalBase, signalTarget, callback );
    ConnectInternal( slotBase );
  }

  // 3 Arguments
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Disconnect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)(ARG1,ARG2,ARG3) ) const
  {
    obj = obj;
    for ( SlotBase* i = m_FirstSlot; i != nullptr; i = i->GetNextInSignal() )
    {
      Slot3<RETURN_TYPE, OBJ_CLASS,ARG1,ARG2,ARG3>* slotBase = const_cast< Slot3< RETURN_TYPE, OBJ_CLASS, ARG1, ARG2, ARG3 >*> ( static_cast< Slot3< RETURN_TYPE, OBJ_CLASS, ARG1, ARG2, ARG3 >* >(i) );
      if ( slotBase != nullptr && slotBase->m_Callback == callback )
      {
        DisconnectInternal( slotBase );
        return;
      }
    }
  }

  // 3 Arguments
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Reconnect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)(ARG1,ARG2,ARG3) ) const
  {
    Disconnect( obj, callback );
    Connect( obj, callback );
  }

  bool operator()(ARG1 arg1, ARG2 arg2, ARG3 arg3)
  {
    bool hasReceiver    = false;
    SlotBase* slot      = m_FirstSlot;
    SlotBase* nextSlot  = nullptr;
    while ( slot != 0 )
    {
      hasReceiver = true;
      nextSlot    = slot->GetNextInSignal();
      switch( slot->GetArgCount() )
      {
      case 0:
        {
          SlotBase0<RETURN_TYPE>* caller = const_cast< SlotBase0<RETURN_TYPE>* > ( static_cast< SlotBase0<RETURN_TYPE>* >( slot ) );
          caller->Call();
        }
        break;
      case 1:
        {
          SlotBase1<RETURN_TYPE, ARG1>* caller = const_cast< SlotBase1<RETURN_TYPE, ARG1>* > ( static_cast< SlotBase1<RETURN_TYPE, ARG1>* >( slot ) );
          caller->Call( arg1 );
        }
        break;
      case 2:
        {
          SlotBase2<RETURN_TYPE, ARG1, ARG2>* caller = const_cast< SlotBase2<RETURN_TYPE, ARG1, ARG2>* > ( static_cast< SlotBase2<RETURN_TYPE, ARG1, ARG2>* >( slot ) );
          caller->Call( arg1, arg2 );
        }
        break;
      case 3:
        {
          SlotBase3<RETURN_TYPE, ARG1, ARG2, ARG3>* caller = const_cast< SlotBase3<RETURN_TYPE, ARG1, ARG2, ARG3>* > ( static_cast< SlotBase3<RETURN_TYPE, ARG1, ARG2, ARG3>* >( slot ) );
          caller->Call( arg1, arg2, arg3 );
        }
        break;
      }
      slot = nextSlot;
    }

    return hasReceiver;
  };
};

template<typename RETURN_TYPE, typename ARG1, typename ARG2, typename ARG3, typename ARG4>
class Signal4 : public SignalBase
{
public:

  // 0 Arguments
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Connect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)() ) const
  {
    SignalBase*   signalBase   = const_cast<SignalBase*>(static_cast<const SignalBase*>(this) );
    SignalTarget* signalTarget = const_cast<OBJ_CLASS*>( static_cast<const OBJ_CLASS*>(obj) );
    Slot0<RETURN_TYPE,OBJ_CLASS>* slotBase = new Slot0<RETURN_TYPE,OBJ_CLASS>( signalBase, signalTarget, callback );
    ConnectInternal( slotBase );
  }

  // 0 Arguments
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Disconnect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)() ) const
  {
    obj = obj;
    for ( SlotBase* i = m_FirstSlot; i != nullptr; i = i->GetNextInSignal() )
    {
      Slot0<RETURN_TYPE, OBJ_CLASS>* slotBase = const_cast< Slot0< RETURN_TYPE, OBJ_CLASS >*> ( static_cast< Slot0< RETURN_TYPE, OBJ_CLASS >* >(i) );
      if ( slotBase != nullptr && slotBase->m_Callback == callback )
      {
        DisconnectInternal( slotBase );
        return;
      }
    }
  }

  // 0 Arguments
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Reconnect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)() ) const
  {
    Disconnect( obj, callback );
    Connect( obj, callback );
  }

  // 1 Argument
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Connect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)(ARG1) ) const
  {
    SignalBase*   signalBase   = const_cast<SignalBase*>(static_cast<const SignalBase*>(this) );
    SignalTarget* signalTarget = const_cast<OBJ_CLASS*>( static_cast<const OBJ_CLASS*>(obj) );
    Slot1<RETURN_TYPE,OBJ_CLASS, ARG1>* slotBase = new Slot1<RETURN_TYPE,OBJ_CLASS,ARG1>( signalBase, signalTarget, callback );
    ConnectInternal( slotBase );
  }

  // 1 Argument
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Disconnect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)(ARG1) ) const
  {
    obj = obj;
    for ( SlotBase* i = m_FirstSlot; i != nullptr; i = i->GetNextInSignal() )
    {
      Slot1<RETURN_TYPE, OBJ_CLASS,ARG1>* slotBase = const_cast< Slot1< RETURN_TYPE, OBJ_CLASS, ARG1 >*> ( static_cast< Slot1< RETURN_TYPE, OBJ_CLASS, ARG1 >* >(i) );
      if ( slotBase != nullptr && slotBase->m_Callback == callback )
      {
        DisconnectInternal( slotBase );
        return;
      }
    }
  }

  // 1 Argument
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Reconnect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)(ARG1) ) const
  {
    Disconnect( obj, callback );
    Connect( obj, callback );
  }

  // 2 Arguments
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Connect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)(ARG1,ARG2) ) const
  {
    SignalBase*   signalBase   = const_cast<SignalBase*>(static_cast<const SignalBase*>(this) );
    SignalTarget* signalTarget = const_cast<OBJ_CLASS*>( static_cast<const OBJ_CLASS*>(obj) );
    Slot2<RETURN_TYPE,OBJ_CLASS, ARG1,ARG2>* slotBase = new Slot2<RETURN_TYPE,OBJ_CLASS,ARG1,ARG2>( signalBase, signalTarget, callback );
    ConnectInternal( slotBase );
  }

  // 2 Arguments
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Disconnect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)(ARG1,ARG2) ) const
  {
    obj = obj;
    for ( SlotBase* i = m_FirstSlot; i != nullptr; i = i->GetNextInSignal() )
    {
      Slot2<RETURN_TYPE, OBJ_CLASS,ARG1, ARG2>* slotBase = const_cast< Slot2< RETURN_TYPE, OBJ_CLASS, ARG1, ARG2 >*> ( static_cast< Slot2< RETURN_TYPE, OBJ_CLASS, ARG1, ARG2 >* >(i) );
      if ( slotBase != nullptr && slotBase->m_Callback == callback )
      {
        DisconnectInternal( slotBase );
        return;
      }
    }
  }

  // 2 Arguments
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Reconnect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)(ARG1,ARG2) ) const
  {
    Disconnect( obj, callback );
    Connect( obj, callback );
  }

  // 3 Arguments
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Connect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)(ARG1,ARG2,ARG3) ) const
  {
    SignalBase*   signalBase   = const_cast<SignalBase*>(static_cast<const SignalBase*>(this) );
    SignalTarget* signalTarget = const_cast<OBJ_CLASS*>( static_cast<const OBJ_CLASS*>(obj) );
    Slot3<RETURN_TYPE,OBJ_CLASS, ARG1,ARG2,ARG3>* slotBase = new Slot3<RETURN_TYPE,OBJ_CLASS,ARG1,ARG2,ARG3>( signalBase, signalTarget, callback );
    ConnectInternal( slotBase );
  }

  // 3 Arguments
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Disconnect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)(ARG1,ARG2,ARG3) ) const
  {
    obj = obj;
    for ( SlotBase* i = m_FirstSlot; i != nullptr; i = i->GetNextInSignal() )
    {
      Slot3<RETURN_TYPE, OBJ_CLASS,ARG1,ARG2,ARG3>* slotBase = const_cast< Slot3< RETURN_TYPE, OBJ_CLASS, ARG1, ARG2, ARG3 >*> ( static_cast< Slot3< RETURN_TYPE, OBJ_CLASS, ARG1, ARG2, ARG3 >* >(i) );
      if ( slotBase != nullptr && slotBase->m_Callback == callback )
      {
        DisconnectInternal( slotBase );
        return;
      }
    }
  }

  // 3 Arguments
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Reconnect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)(ARG1,ARG2,ARG3) ) const
  {
    Disconnect( obj, callback );
    Connect( obj, callback );
  }

  // 4 Arguments
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Connect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)(ARG1,ARG2,ARG3,ARG4) ) const
  {
    SignalBase*   signalBase   = const_cast<SignalBase*>(static_cast<const SignalBase*>(this) );
    SignalTarget* signalTarget = const_cast<OBJ_CLASS*>( static_cast<const OBJ_CLASS*>(obj) );
    Slot4<RETURN_TYPE,OBJ_CLASS, ARG1,ARG2,ARG3,ARG4>* slotBase = new Slot4<RETURN_TYPE,OBJ_CLASS,ARG1,ARG2,ARG3,ARG4>( signalBase, signalTarget, callback );
    ConnectInternal( slotBase );
  }

  // 4 Arguments
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Disconnect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)(ARG1,ARG2,ARG3,ARG4) ) const
  {
    obj = obj;
    for ( SlotBase* i = m_FirstSlot; i != nullptr; i = i->GetNextInSignal() )
    {
      Slot4<RETURN_TYPE, OBJ_CLASS,ARG1,ARG2,ARG3,ARG4>* slotBase = const_cast< Slot4< RETURN_TYPE, OBJ_CLASS, ARG1, ARG2, ARG3, ARG4 >*> ( static_cast< Slot4< RETURN_TYPE, OBJ_CLASS, ARG1, ARG2, ARG3, ARG4 >* >(i) );
      if ( slotBase != nullptr && slotBase->m_Callback == callback )
      {
        DisconnectInternal( slotBase );
        return;
      }
    }
  }

  // 4 Arguments
  template <typename OBJ_CLASS,typename CALLBACK_TYPE>
  void Reconnect( CALLBACK_TYPE* obj, RETURN_TYPE ( OBJ_CLASS::*callback)(ARG1,ARG2,ARG3,ARG4) ) const
  {
    Disconnect( obj, callback );
    Connect( obj, callback );
  }

  bool operator()(ARG1 arg1, ARG2 arg2, ARG3 arg3, ARG4 arg4)
  {
    bool hasReceiver    = false;
    SlotBase* slot      = m_FirstSlot;
    SlotBase* nextSlot  = nullptr;
    while ( slot != 0 )
    {
      hasReceiver = true;
      nextSlot    = slot->GetNextInSignal();
      switch( slot->GetArgCount() )
      {
      case 0:
        {
          SlotBase0<RETURN_TYPE>* caller = const_cast< SlotBase0<RETURN_TYPE>* > ( static_cast< SlotBase0<RETURN_TYPE>* >( slot ) );
          caller->Call();
        }
        break;
      case 1:
        {
          SlotBase1<RETURN_TYPE, ARG1>* caller = const_cast< SlotBase1<RETURN_TYPE, ARG1>* > ( static_cast< SlotBase1<RETURN_TYPE, ARG1>* >( slot ) );
          caller->Call( arg1 );
        }
        break;
      case 2:
        {
          SlotBase2<RETURN_TYPE, ARG1, ARG2>* caller = const_cast< SlotBase2<RETURN_TYPE, ARG1, ARG2>* > ( static_cast< SlotBase2<RETURN_TYPE, ARG1, ARG2>* >( slot ) );
          caller->Call( arg1, arg2 );
        }
        break;
      case 3:
        {
          SlotBase3<RETURN_TYPE, ARG1, ARG2, ARG3>* caller = const_cast< SlotBase3<RETURN_TYPE, ARG1, ARG2, ARG3>* > ( static_cast< SlotBase3<RETURN_TYPE, ARG1, ARG2, ARG3>* >( slot ) );
          caller->Call( arg1, arg2, arg3 );
        }
        break;
      case 4:
        {
          SlotBase4<RETURN_TYPE, ARG1, ARG2, ARG3, ARG4>* caller = const_cast< SlotBase4<RETURN_TYPE, ARG1, ARG2, ARG3, ARG4>* > ( static_cast< SlotBase4<RETURN_TYPE, ARG1, ARG2, ARG3, ARG4>* >( slot ) );
          caller->Call( arg1, arg2, arg3, arg4 );
        }
        break;
      }
      slot = nextSlot;
    }

    return hasReceiver;
  };
};

#endif /// EVILENGINE_CORELIB_SIGNAL_SIGNAL
