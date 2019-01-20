/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Modifications by Paul Soucy copyright (C) 2018
 */

#include <baseline/Baseline.h>
#include <baseline/Mutex.h>
#include <baseline/Condition.h>
#include <baseline/Thread.h>

#ifdef WIN32
  #include <process.h>
#else
  #include <unistd.h>
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
  thread_t mThreadId;
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
  pthread_create( &data->mThreadId, &attr, trampoline, data );
  pthread_attr_destroy( &attr );

#elif defined(CMAKE_USE_WIN32_THREADS_INIT)

  _beginthreadex( NULL, 0, trampoline, data, NULL, &data->mThreadId );

#endif

  return OK;
}

status_t Thread::join()
{
  ThreadData* data = toThreadData( mData );
  Mutex::Autolock l( data->mLock );
  while( data->mRunning ) {
    data->mThreadExitedCondition.waitTimeout( data->mLock, 500 );
  }

  return OK;
}

void Thread::sleep( uint32_t millisec )
{
#ifdef WIN32
  Sleep( millisec );
#else
  usleep( millisec * 1000 );
#endif

}


}