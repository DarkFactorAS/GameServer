#ifndef EVILENGINE_CORELIB_SIGNAL_SLOTBASE
#define EVILENGINE_CORELIB_SIGNAL_SLOTBASE 1
#pragma once

class SignalBase;
class SignalTarget;

class SlotBase
{
public:
  SlotBase( SignalBase* targetSignal, SignalTarget* object, int argCount );
  virtual ~SlotBase();

  virtual SlotBase* Clone( SignalBase* targetSignal ) = 0;

  int             GetArgCount() const { return m_ArgCount; }

  SignalTarget* GetSignalTarget() const { return m_Object; }
  SlotBase*     GetNextInSignal() { return m_NextInSignal; }
  SlotBase*     GetPrevInSignal() { return m_PrevInSignal; }
  SlotBase*     GetLastInSignal();

protected:
  friend class SignalTarget;
  friend class SignalBase;

  SignalBase*   m_Signal;
  SignalTarget* m_Object;
  int           m_ArgCount;

  SlotBase* m_PrevInSignal;
  SlotBase* m_NextInSignal;

  SlotBase* m_PrevInTarget;
  SlotBase* m_NextInTarget;

private:
  // Disabled operators:
  SlotBase( const SlotBase& );
  SlotBase& operator=( const SlotBase& );
};

/************************************************************************************************
* SlotBase0:
* Template class to have a base class for a Slot taking no argument. This template class is
* generating a virtual call function that the class specific Slot1 will use when running Call
*
* @typename  (RETURN_TYPE)   - The return type for the function ( void, int, float, etc )
*
* @author Thor Richard Hansen
*************************************************************************************************/
template <typename RETURN_TYPE>
class SlotBase0 : public SlotBase
{
public:
  SlotBase0( SignalBase* targetSignal, SignalTarget* object ) : SlotBase( targetSignal, object, 0 ){}
  virtual void Call( void ){};
};

/************************************************************************************************
* Slot0:
* Template class used to create a callback slot of a specific class with no argument
* and a specific return type. This template is used for all slots with no argument.
*
* @typename  (RETURN_TYPE)   - The return type for the function ( void, int, float, etc )
* @typename  (OBJ_CLASS)     - The class type for the class that connects to the signal
*
* @author Thor Richard Hansen
*************************************************************************************************/
template <typename RETURN_TYPE, typename OBJ_CLASS>
class Slot0 : public SlotBase0<RETURN_TYPE>
{
public:

  Slot0( SignalBase* targetSignal, SignalTarget* object, RETURN_TYPE(OBJ_CLASS::*callback)() ) :
    SlotBase0<RETURN_TYPE>( targetSignal, object )
  {
    m_Callback = callback;
  }

  virtual void Call()
  {
    OBJ_CLASS* tmp = static_cast<OBJ_CLASS*>(this->m_Object);
    return ((tmp)->*m_Callback)();
  }

  virtual SlotBase* Clone( SignalBase* owningSignal )
  {
    Slot0<RETURN_TYPE,OBJ_CLASS>* slotBase = new Slot0<RETURN_TYPE,OBJ_CLASS>( owningSignal, static_cast<OBJ_CLASS*>(this->m_Object), m_Callback );
    return slotBase;
  }

  virtual bool IsSlotConnected( RETURN_TYPE ( OBJ_CLASS::*callback)() )
  {
    return ( m_Callback == callback );
  }

  RETURN_TYPE(OBJ_CLASS::*m_Callback)();
};

/************************************************************************************************
* SlotBase1:
* Template class to have a base class for a Slot taking 1 argument. This template class is
* generating a virtual call function that the class specific Slot1 will use when running Call
*
* @typename  (RETURN_TYPE)   - The return type for the function ( void, int, float, etc )
* @typename  (ARG1)          - The argument type for the function ( void, int, float, etc )
*
* @author Thor Richard Hansen
*************************************************************************************************/
template <typename RETURN_TYPE, typename ARG1>
class SlotBase1 : public SlotBase
{
public:
  SlotBase1( SignalBase* targetSignal, SignalTarget* object ) : SlotBase( targetSignal, object, 1 ){}
  virtual void Call( ARG1 ){};
  virtual SlotBase* Clone( SignalBase* /*targetSignal*/ ){ return 0; };
};

/************************************************************************************************
* Slot1:
* Template class used to create a callback slot of a specific class with one specific argument
* and a specific return type. This template is used for all slots with 1 argument.
*
* @typename  (RETURN_TYPE)   - The return type for the function ( void, int, float, etc )
* @typename  (OBJ_CLASS)     - The class type for the class that connects to the signal
* @typename  (ARG1)          - The argument type for the function ( void, int, float, etc )
*
* @author Thor Richard Hansen
*************************************************************************************************/
template <typename RETURN_TYPE, typename OBJ_CLASS, typename ARG1>
class Slot1 : public SlotBase1<RETURN_TYPE, ARG1>
{
public:

  Slot1( SignalBase* targetSignal, SignalTarget* object, RETURN_TYPE(OBJ_CLASS::*callback)(ARG1) ) :
    SlotBase1<RETURN_TYPE,ARG1>( targetSignal, object )
  {
    m_Callback = callback;
  }

  virtual void Call( ARG1 arg )
  {
    OBJ_CLASS* tmp = static_cast<OBJ_CLASS*>(this->m_Object);
    return ((tmp)->*m_Callback)( arg );
  }

  virtual SlotBase* Clone( SignalBase* owningSignal )
  {
    Slot1<RETURN_TYPE,OBJ_CLASS,ARG1>* slotBase = new Slot1<RETURN_TYPE,OBJ_CLASS,ARG1>( owningSignal, static_cast<OBJ_CLASS*>(this->m_Object), m_Callback );
    return slotBase;
  }

  virtual bool IsSlotConnected( RETURN_TYPE ( OBJ_CLASS::*callback)(ARG1) )
  {
    return ( m_Callback == callback );
  }

  RETURN_TYPE(OBJ_CLASS::*m_Callback)(ARG1);
};

/************************************************************************************************
* SlotBase2:
* Template class to have a base class for a Slot taking 2 arguments. This template class is
* generating a virtual call function that the class specific Slot1 will use when running Call
*
* @typename  (RETURN_TYPE)   - The return type for the function ( void, int, float, etc )
* @typename  (ARG1)          - The argument type for the function ( void, int, float, etc )
*
* @author Thor Richard Hansen
*************************************************************************************************/
template <typename RETURN_TYPE, typename ARG1, typename ARG2>
class SlotBase2 : public SlotBase
{
public:
  SlotBase2( SignalBase* targetSignal, SignalTarget* object ) : SlotBase( targetSignal, object, 2 ){}
  virtual void Call( ARG1, ARG2 ){};
  virtual SlotBase* Clone( SignalBase* /*targetSignal*/ ){ return 0; };
};

/************************************************************************************************
* Slot2:
* Template class used to create a callback slot of a specific class with two arguments
* and a specific return type. This template is used for all slots with 2 arguments.
*
* @typename  (RETURN_TYPE)   - The return type for the function ( void, int, float, etc )
* @typename  (OBJ_CLASS)     - The class type for the class that connects to the signal
* @typename  (ARG1)          - The argument type for the function ( void, int, float, etc )
*
* @author Thor Richard Hansen
*************************************************************************************************/
template <typename RETURN_TYPE, typename OBJ_CLASS, typename ARG1, typename ARG2>
class Slot2 : public SlotBase2<RETURN_TYPE, ARG1, ARG2>
{
public:

  Slot2( SignalBase* targetSignal, SignalTarget* object, RETURN_TYPE(OBJ_CLASS::*callback)(ARG1,ARG2) ) :
    SlotBase2<RETURN_TYPE,ARG1, ARG2>( targetSignal, object )
  {
    m_Callback = callback;
  }

  virtual void Call( ARG1 arg, ARG2 arg2 )
  {
    OBJ_CLASS* tmp = static_cast<OBJ_CLASS*>(this->m_Object);
    return ((tmp)->*m_Callback)( arg, arg2 );
  }

  virtual SlotBase* Clone( SignalBase* owningSignal )
  {
    Slot2<RETURN_TYPE,OBJ_CLASS,ARG1,ARG2>* slotBase = new Slot2<RETURN_TYPE,OBJ_CLASS,ARG1,ARG2>( owningSignal, static_cast<OBJ_CLASS*>(this->m_Object), m_Callback );
    return slotBase;
  }

  virtual bool IsSlotConnected( RETURN_TYPE ( OBJ_CLASS::*callback)(ARG1,ARG2) )
  {
    return ( m_Callback == callback );
  }

  RETURN_TYPE(OBJ_CLASS::*m_Callback)(ARG1,ARG2);
};

/************************************************************************************************
* SlotBase3:
* Template class to have a base class for a Slot taking 3 arguments. This template class is
* generating a virtual call function that the class specific Slot1 will use when running Call
*
* @typename  (RETURN_TYPE)   - The return type for the function ( void, int, float, etc )
* @typename  (ARG1)          - The argument type for the function ( void, int, float, etc )
* @typename  (ARG2)          - The argument type for the function ( void, int, float, etc )
* @typename  (ARG3)          - The argument type for the function ( void, int, float, etc )
* @typename  (ARG4)          - The argument type for the function ( void, int, float, etc )
*
* @author Thor Richard Hansen
*************************************************************************************************/
template <typename RETURN_TYPE, typename ARG1, typename ARG2, typename ARG3>
class SlotBase3 : public SlotBase
{
public:
  SlotBase3( SignalBase* targetSignal, SignalTarget* object ) : SlotBase( targetSignal, object, 3 ){}
  virtual void Call( ARG1, ARG2, ARG3 ){};
  virtual SlotBase* Clone( SignalBase* /*targetSignal*/ ){ return 0; };
};

/************************************************************************************************
* Slot4:
* Template class used to create a callback slot of a specific class with two arguments
* and a specific return type. This template is used for all slots with 4 arguments.
*
* @typename  (RETURN_TYPE)   - The return type for the function ( void, int, float, etc )
* @typename  (OBJ_CLASS)     - The class type for the class that connects to the signal
* @typename  (ARG1)          - The argument type for the function ( void, int, float, etc )
* @typename  (ARG2)          - The argument type for the function ( void, int, float, etc )
* @typename  (ARG3)          - The argument type for the function ( void, int, float, etc )
* @typename  (ARG4)          - The argument type for the function ( void, int, float, etc )
*
* @author Thor Richard Hansen
*************************************************************************************************/
template <typename RETURN_TYPE, typename OBJ_CLASS, typename ARG1, typename ARG2, typename ARG3>
class Slot3 : public SlotBase3<RETURN_TYPE, ARG1, ARG2, ARG3>
{
public:

  Slot3( SignalBase* targetSignal, SignalTarget* object, RETURN_TYPE(OBJ_CLASS::*callback)(ARG1,ARG2,ARG3) ) :
    SlotBase3<RETURN_TYPE,ARG1, ARG2,ARG3>( targetSignal, object )
  {
    m_Callback = callback;
  }

  virtual void Call( ARG1 arg, ARG2 arg2, ARG3 arg3 )
  {
    OBJ_CLASS* tmp = static_cast<OBJ_CLASS*>(this->m_Object);
    return ((tmp)->*m_Callback)( arg, arg2, arg3 );
  }

  virtual SlotBase* Clone( SignalBase* owningSignal )
  {
    Slot3<RETURN_TYPE,OBJ_CLASS,ARG1,ARG2,ARG3>* slotBase = new Slot3<RETURN_TYPE,OBJ_CLASS,ARG1,ARG2,ARG3>( owningSignal, static_cast<OBJ_CLASS*>(this->m_Object), m_Callback );
    return slotBase;
  }

  virtual bool IsSlotConnected( RETURN_TYPE ( OBJ_CLASS::*callback)(ARG1,ARG2,ARG3) )
  {
    return ( m_Callback == callback );
  }

  RETURN_TYPE(OBJ_CLASS::*m_Callback)(ARG1,ARG2,ARG3);
};

/************************************************************************************************
* SlotBase3:
* Template class to have a base class for a Slot taking 3 arguments. This template class is
* generating a virtual call function that the class specific Slot1 will use when running Call
*
* @typename  (RETURN_TYPE)   - The return type for the function ( void, int, float, etc )
* @typename  (ARG1)          - The argument type for the function ( void, int, float, etc )
* @typename  (ARG2)          - The argument type for the function ( void, int, float, etc )
* @typename  (ARG3)          - The argument type for the function ( void, int, float, etc )
* @typename  (ARG4)          - The argument type for the function ( void, int, float, etc )
*
* @author Thor Richard Hansen
*************************************************************************************************/
template <typename RETURN_TYPE, typename ARG1, typename ARG2, typename ARG3, typename ARG4>
class SlotBase4 : public SlotBase
{
public:
  SlotBase4( SignalBase* targetSignal, SignalTarget* object ) : SlotBase( targetSignal, object, 4 ){}
  virtual void Call( ARG1, ARG2, ARG3, ARG4 ){};
  virtual SlotBase* Clone( SignalBase* /*targetSignal*/ ){ return 0; };
};

/************************************************************************************************
* Slot4:
* Template class used to create a callback slot of a specific class with two arguments
* and a specific return type. This template is used for all slots with 4 arguments.
*
* @typename  (RETURN_TYPE)   - The return type for the function ( void, int, float, etc )
* @typename  (OBJ_CLASS)     - The class type for the class that connects to the signal
* @typename  (ARG1)          - The argument type for the function ( void, int, float, etc )
* @typename  (ARG2)          - The argument type for the function ( void, int, float, etc )
* @typename  (ARG3)          - The argument type for the function ( void, int, float, etc )
* @typename  (ARG4)          - The argument type for the function ( void, int, float, etc )
*
* @author Thor Richard Hansen
*************************************************************************************************/
template <typename RETURN_TYPE, typename OBJ_CLASS, typename ARG1, typename ARG2, typename ARG3, typename ARG4>
class Slot4 : public SlotBase4<RETURN_TYPE, ARG1, ARG2, ARG3, ARG4>
{
public:

  Slot4( SignalBase* targetSignal, SignalTarget* object, RETURN_TYPE(OBJ_CLASS::*callback)(ARG1,ARG2,ARG3,ARG4) ) :
    SlotBase4<RETURN_TYPE,ARG1, ARG2,ARG3,ARG4>( targetSignal, object )
  {
    m_Callback = callback;
  }

  virtual void Call( ARG1 arg, ARG2 arg2, ARG3 arg3, ARG4 arg4 )
  {
    OBJ_CLASS* tmp = static_cast<OBJ_CLASS*>(this->m_Object);
    return ((tmp)->*m_Callback)( arg, arg2, arg3, arg4 );
  }

  virtual SlotBase* Clone( SignalBase* owningSignal )
  {
    Slot4<RETURN_TYPE,OBJ_CLASS,ARG1,ARG2,ARG3,ARG4>* slotBase = new Slot4<RETURN_TYPE,OBJ_CLASS,ARG1,ARG2,ARG3,ARG4>( owningSignal, static_cast<OBJ_CLASS*>(this->m_Object), m_Callback );
    return slotBase;
  }

  virtual bool IsSlotConnected( RETURN_TYPE ( OBJ_CLASS::*callback)(ARG1,ARG2,ARG3,ARG4) )
  {
    return ( m_Callback == callback );
  }

  RETURN_TYPE(OBJ_CLASS::*m_Callback)(ARG1,ARG2,ARG3,ARG4);
};

#endif /// EVILENGINE_CORELIB_SIGNAL_SLOTBASE