#include "Private.h"
#include <baseline/Baseline.h>
#include <baseline/Mutex.h>
#include <baseline/Condition.h>
#include <baseline/Thread.h>

#ifdef WIN32
  #include <process.h>
#endif

#if defined(CMAKE_USE_PTHREADS_INIT)
  #define TRAMPOLINE_RETURN_T void*
  #define TRAMPOLINE_RETURN return NULL;
#elif defined(CMAKE_USE_WIN32_THREADS_INIT)
  #define TRAMPOLINE_RETURN_T unsigned
  #define TRAMPOLINE_RETURN return 0;
#endif

namespace baseline {

struct ThreadData {
  ThreadData( Thread* t )
    : mThread( t ), mRunning( false ) {}

  Thread* mThread;
  bool mRunning;
  Mutex mLock;
  Condition mThreadExitedCondition;
#if defined(CMAKE_USE_PTHREADS_INIT)
  pthread_t threadId;
#elif defined(CMAKE_USE_WIN32_THREADS_INIT)
  unsigned threadId;
#endif
};

inline ThreadData* toThreadData( void* prt )
{
  return static_cast<ThreadData*>( prt );
}

inline Thread* toThread( void* ptr )
{
  return static_cast<Thread*>( ptr );
}


static TRAMPOLINE_RETURN_T trampoline( void* data )
{
  ThreadData* threadData = toThreadData( data );

  threadData->mThread->run();
  {
    Mutex::Autolock l( threadData->mLock );
    threadData->mRunning = false;
    threadData->mThreadExitedCondition.signalAll();
  }
  threadData->mThread->decStrong( data );

  TRAMPOLINE_RETURN
}

Thread::Thread()
  : mData( new ThreadData( this ) )
{}

Thread::~Thread()
{
  ThreadData* data = static_cast<ThreadData*>( mData );
  delete data;
  mData = nullptr;
}

status_t Thread::start()
{
  incStrong( this );
  ThreadData* data = toThreadData( mData );
  data->mRunning = true;

#if defined(CMAKE_USE_PTHREADS_INIT)

  pthread_attr_t attr;
  pthread_attr_init( &attr );
  pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_DETACHED );
  pthread_create( &data->threadId, &attr, trampoline, data );

#elif defined(CMAKE_USE_WIN32_THREADS_INIT)

  _beginthreadex( NULL, 0, trampoline, data, NULL, &data->threadId );

#endif

  return OK;
}

status_t Thread::join()
{
  incStrong( this );
  ThreadData* data = toThreadData( mData );
  Mutex::Autolock l( data->mLock );
  while( data->mRunning ) {
    data->mThreadExitedCondition.wait( data->mLock );
  }

  decStrong( this );
  return OK;
}


}