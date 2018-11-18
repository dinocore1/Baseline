#ifndef MUTEX_H_
#define MUTEX_H_

namespace baseline {

class Mutex
{
public:
  Mutex();
  ~Mutex();
  status_t lock();
  void unlock();
  status_t tryLock();

  class Autolock
  {
  public:
    inline Autolock( Mutex& mutex ) : mLock( mutex ) {
      mLock.lock();
    }
    inline Autolock( Mutex* mutex ) : mLock( *mutex ) {
      mLock.lock();
    }
    inline ~Autolock() {
      mLock.unlock();
    }
  private:
    Mutex& mLock;
  };

private:
  friend class Condition;

  //A mutex cannot be copied
  Mutex( const Mutex& );
  Mutex& operator = ( const Mutex& );

  mutex_t mMutex;
};

}

#endif // MUTEX_H_