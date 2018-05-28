
#include "Precompile.h"
#include "Mutex.h"

#if !defined(PLATFORM_ANDROID) && !defined(PLATFORM_IOS) && !defined(PLATFORM_OSXCLIENT)
  #include <boost/interprocess/sync/interprocess_mutex.hpp>
#endif

Mutex::Mutex()
{
#if !defined(PLATFORM_ANDROID) && !defined(PLATFORM_IOS) && !defined(PLATFORM_OSXCLIENT)
  m_LockMutex = new boost::interprocess::interprocess_mutex();
#endif
}

Mutex::~Mutex()
{
#if !defined(PLATFORM_ANDROID) && !defined(PLATFORM_IOS) && !defined(PLATFORM_OSXCLIENT)
  delete m_LockMutex;
#endif
}

void Mutex::Aquire() const
{
#if !defined(PLATFORM_ANDROID) && !defined(PLATFORM_IOS) && !defined(PLATFORM_OSXCLIENT)
  m_LockMutex->lock();
#endif
}
void Mutex::TryAquire() const
{
#if !defined(PLATFORM_ANDROID) && !defined(PLATFORM_IOS) && !defined(PLATFORM_OSXCLIENT)
  m_LockMutex->try_lock();
#endif
}
void Mutex::Release() const
{
#if !defined(PLATFORM_ANDROID) && !defined(PLATFORM_IOS) && !defined(PLATFORM_OSXCLIENT)
  m_LockMutex->unlock();
#endif
}
