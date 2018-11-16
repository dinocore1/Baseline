#include <baseline/Baseline.h>

#include "Private.h"

#include <baseline/Mutex.h>
#include <baseline/Condition.h>
#include <baseline/Thread.h>

#include <pthread.h>

namespace baseline {


struct ThreadData {
  ThreadData( Thread* t )
    : mThread( t ), mRunning( false ) {}

  Thread* mThread;
  bool mRunning;
  Mutex mLock;
  Condition mThreadExitedCondition;
#ifdef CMAKE_USE_PTHREADS_INIT
  pthread_t threadId;
#elif CMAKE_USE_WIN32_THREADS_INIT
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

static void* trampoline( void* data )
{
  ThreadData* threadData = toThreadData( data );
  threadData->mThread->incStrong( data );
  {
    Mutex::Autolock l( threadData->mLock );
    threadData->mRunning = true;
  }
  threadData->mThread->run();
  {
    Mutex::Autolock l( threadData->mLock );
    threadData->mRunning = false;
    threadData->mThreadExitedCondition.signalAll();
  }
  threadData->mThread->decStrong( data );
  return NULL;
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
  ThreadData* data = static_cast<ThreadData*>( mData );

#ifdef CMAKE_USE_PTHREADS_INIT

  pthread_attr_t attr;
  pthread_attr_init( &attr );
  pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_DETACHED );
  pthread_create( &data->threadId, &attr, trampoline, data );

#elif CMAKE_USE_WIN32_THREADS_INIT

  _beginthreadex( NULL, 0, trampoline, data, &data->threadId );

#endif

  return OK;
}

status_t Thread::join()
{
  ThreadData* data = toThreadData( mData );
  Mutex::Autolock l( data->mLock );
  while( data->mRunning ) {
    data->mThreadExitedCondition.wait( data->mLock );
  }
}


}