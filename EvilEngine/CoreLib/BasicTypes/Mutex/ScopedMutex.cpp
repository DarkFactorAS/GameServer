/*************************************************************************************
* Project     : Evil Engine 2012
* File        : ScopedMutex
* Description : Creates a mutex that will try to lock when the class is created and
*               released when the class is destroyed.
*               
* Author      : Thor Richard Hansen
*************************************************************************************/

#include "Precompile.h"
#include "ScopedMutex.h"
//#include <boost/interprocess/sync/interprocess_mutex.hpp>

ScopedMutex::ScopedMutex( const Mutex* mutex)
{
  m_Mutex = const_cast<Mutex*> ( mutex );
  m_Mutex->Aquire();
}

ScopedMutex::~ScopedMutex()
{
  m_Mutex->Release();
}
