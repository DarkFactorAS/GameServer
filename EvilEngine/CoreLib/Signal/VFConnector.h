#ifndef EVILENGINE_CORELIB_SIGNAL_VFCONNECTOR
#define EVILENGINE_CORELIB_SIGNAL_VFCONNECTOR 1
#pragma once

#include "SignalBase.h"
#include "SlotBase.h"

template<typename RETURN_TYPE>
class VFConnector0 : public Signal0<RETURN_TYPE>
{
public:

  bool operator()()
  {
    SlotBase* slot = Signal0<RETURN_TYPE>::GetFirstSlot();
    if ( slot != 0 )
    {
      slot = slot->GetLastInSignal();
      SlotBase0<RETURN_TYPE>* caller = const_cast< SlotBase0<RETURN_TYPE>* > ( static_cast< SlotBase0<RETURN_TYPE>* >( slot ) );
      if ( caller != nullptr )
      {
        caller->Call();
        return true;
      }
    }
    return false;
  };
};

template<typename RETURN_TYPE, typename ARG1>
class VFConnector1 : public Signal1<RETURN_TYPE,ARG1>
{
public:

  bool operator()(ARG1 arg1)
  {
    SlotBase* slot = Signal1<RETURN_TYPE,ARG1>::GetFirstSlot();
    if ( slot != 0 )
    {
      slot    = slot->GetLastInSignal();
      switch( slot->GetArgCount() )
      {
        case 0:
          {
            SlotBase0<RETURN_TYPE>* caller = const_cast< SlotBase0<RETURN_TYPE>* > ( static_cast< SlotBase0<RETURN_TYPE>* >( slot ) );
            caller->Call();
            return true;
          }

        case 1:
          {
            SlotBase1<RETURN_TYPE, ARG1>* caller = const_cast< SlotBase1<RETURN_TYPE, ARG1>* > ( static_cast< SlotBase1<RETURN_TYPE, ARG1>* >( slot ) );
            caller->Call( arg1 );
            return true;
          }
      }
    }
    return false;
  };
};

template<typename RETURN_TYPE, typename ARG1, typename ARG2>
class VFConnector2 : public Signal2<RETURN_TYPE,ARG1,ARG2>
{
public:

  bool operator()(ARG1 arg1, ARG2 arg2)
  {
    SlotBase* slot = Signal2<RETURN_TYPE,ARG1,ARG2>::GetFirstSlot();
    if ( slot != 0 )
    {
      slot = slot->GetLastInSignal();
      switch( slot->GetArgCount() )
      {
        case 0:
          {
            SlotBase0<RETURN_TYPE>* caller = const_cast< SlotBase0<RETURN_TYPE>* > ( static_cast< SlotBase0<RETURN_TYPE>* >( slot ) );
            caller->Call();
            return true;
          }

        case 1:
          {
            SlotBase1<RETURN_TYPE, ARG1>* caller = const_cast< SlotBase1<RETURN_TYPE, ARG1>* > ( static_cast< SlotBase1<RETURN_TYPE, ARG1>* >( slot ) );
            caller->Call( arg1 );
            return true;
          }

        case 2:
          {
            SlotBase2<RETURN_TYPE, ARG1, ARG2>* caller = const_cast< SlotBase2<RETURN_TYPE, ARG1, ARG2>* > ( static_cast< SlotBase2<RETURN_TYPE, ARG1, ARG2>* >( slot ) );
            caller->Call( arg1, arg2 );
            return true;
          }
      }
    }
    return false;
  };
};

template<typename RETURN_TYPE, typename ARG1, typename ARG2, typename ARG3 >
class VFConnector3 : public Signal3< RETURN_TYPE, ARG1, ARG2, ARG3 >
{
public:

  bool operator()(ARG1 arg1, ARG2 arg2, ARG3 arg3)
  {
    SlotBase* slot = Signal3<RETURN_TYPE,ARG1,ARG2,ARG3>::GetFirstSlot();
    while ( slot != 0 )
    {
      slot = slot->GetLastInSignal();
      switch( slot->GetArgCount() )
      {
        case 0:
          {
            SlotBase0<RETURN_TYPE>* caller = const_cast< SlotBase0<RETURN_TYPE>* > ( static_cast< SlotBase0<RETURN_TYPE>* >( slot ) );
            caller->Call();
            return true;
          }

        case 1:
          {
            SlotBase1<RETURN_TYPE, ARG1>* caller = const_cast< SlotBase1<RETURN_TYPE, ARG1>* > ( static_cast< SlotBase1<RETURN_TYPE, ARG1>* >( slot ) );
            caller->Call( arg1 );
            return true;
          }

        case 2:
          {
            SlotBase2<RETURN_TYPE, ARG1, ARG2>* caller = const_cast< SlotBase2<RETURN_TYPE, ARG1, ARG2>* > ( static_cast< SlotBase2<RETURN_TYPE, ARG1, ARG2>* >( slot ) );
            caller->Call( arg1, arg2 );
            return true;
          }

        case 3:
          {
            SlotBase3<RETURN_TYPE, ARG1, ARG2, ARG3>* caller = const_cast< SlotBase3<RETURN_TYPE, ARG1, ARG2, ARG3>* > ( static_cast< SlotBase3<RETURN_TYPE, ARG1, ARG2, ARG3>* >( slot ) );
            caller->Call( arg1, arg2, arg3 );
            return true;
          }
      }
    }
    return false;
  };
};

template<typename RETURN_TYPE, typename ARG1, typename ARG2, typename ARG3, typename ARG4>
class VFConnector4 : public Signal4<RETURN_TYPE, ARG1, ARG2, ARG3, ARG4 >
{
public:

  bool operator()(ARG1 arg1, ARG2 arg2, ARG3 arg3, ARG4 arg4)
  {
    SlotBase* slot = Signal4<RETURN_TYPE,ARG1,ARG2,ARG3,ARG4>::GetFirstSlot();
    while ( slot != 0 )
    {
      slot = slot->GetLastInSignal();
      switch( slot->GetArgCount() )
      {
        case 0:
          {
            SlotBase0<RETURN_TYPE>* caller = const_cast< SlotBase0<RETURN_TYPE>* > ( static_cast< SlotBase0<RETURN_TYPE>* >( slot ) );
            caller->Call();
            return true;
          }

        case 1:
          {
            SlotBase1<RETURN_TYPE, ARG1>* caller = const_cast< SlotBase1<RETURN_TYPE, ARG1>* > ( static_cast< SlotBase1<RETURN_TYPE, ARG1>* >( slot ) );
            caller->Call( arg1 );
            return true;
          }

        case 2:
          {
            SlotBase2<RETURN_TYPE, ARG1, ARG2>* caller = const_cast< SlotBase2<RETURN_TYPE, ARG1, ARG2>* > ( static_cast< SlotBase2<RETURN_TYPE, ARG1, ARG2>* >( slot ) );
            caller->Call( arg1, arg2 );
            return true;
          }

        case 3:
          {
            SlotBase3<RETURN_TYPE, ARG1, ARG2, ARG3>* caller = const_cast< SlotBase3<RETURN_TYPE, ARG1, ARG2, ARG3>* > ( static_cast< SlotBase3<RETURN_TYPE, ARG1, ARG2, ARG3>* >( slot ) );
            caller->Call( arg1, arg2, arg3 );
            return true;
          }

        case 4:
          {
            SlotBase4<RETURN_TYPE, ARG1, ARG2, ARG3, ARG4>* caller = const_cast< SlotBase4<RETURN_TYPE, ARG1, ARG2, ARG3, ARG4>* > ( static_cast< SlotBase4<RETURN_TYPE, ARG1, ARG2, ARG3, ARG4>* >( slot ) );
            caller->Call( arg1, arg2, arg3, arg4 );
            return true;
          }
      }
    }
    return false;
  };
};

#endif /// EVILENGINE_CORELIB_SIGNAL_VFCONNECTOR
