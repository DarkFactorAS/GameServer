
#ifndef CUSTOM_SCOPEDMUTEX_H
#define CUSTOM_SCOPEDMUTEX_H

#include "Mutex.h"

class ScopedMutex
{
public:
  ScopedMutex( const Mutex* mutex );
  ~ScopedMutex();
private:
  /// Mutex that protects the write operation
  Mutex* m_Mutex;
};

#endif // CUSTOM_SCOPEDMUTEX_H
