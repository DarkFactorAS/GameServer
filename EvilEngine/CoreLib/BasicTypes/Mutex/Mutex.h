
#ifndef EVILENGINE_CORELIB_BASICTYPES_MUTEX_H
#define EVILENGINE_CORELIB_BASICTYPES_MUTEX_H

namespace boost
{
  namespace interprocess
  {
    class interprocess_mutex;
  }
};

class Mutex
{
public:
  Mutex();
  ~Mutex();

  void Aquire() const;
  void TryAquire() const;
  void Release() const;

protected:
  // Mutex that protects the write operation
  boost::interprocess::interprocess_mutex* m_LockMutex;
};

#endif // EVILENGINE_CORELIB_BASICTYPES_MUTEX_H
