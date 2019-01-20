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
#include <baseline/Condition.h>

namespace baseline {

Condition::Condition()
{
#if defined(CMAKE_USE_PTHREADS_INIT)
  pthread_cond_init( &mVar, NULL );
#elif defined(CMAKE_USE_WIN32_THREADS_INIT)
  InitializeConditionVariable( &mVar );
#endif
}

Condition::~Condition()
{
#if defined(CMAKE_USE_PTHREADS_INIT)
  pthread_cond_destroy( &mVar );
#elif defined(CMAKE_USE_WIN32_THREADS_INIT)

#endif
}

status_t Condition::wait( Mutex& mutex )
{
#if defined(CMAKE_USE_PTHREADS_INIT)
  pthread_cond_wait( &mVar, &mutex.mMutex );
#elif defined(CMAKE_USE_WIN32_THREADS_INIT)
  SleepConditionVariableCS( &mVar, &mutex.mMutex, INFINITE );
#endif

  return OK;
}

status_t Condition::waitTimeout( Mutex& mutex, uint32_t timeoutMS )
{
  status_t retval = OK;
#if defined(CMAKE_USE_PTHREADS_INIT)
#define BILLION 1000000000
  struct timespec ts;
  clock_gettime( CLOCK_REALTIME, &ts );
  ts.tv_sec += timeoutMS / 1000;
  ts.tv_nsec += ( timeoutMS % 1000 ) * 1000000;
  if( ts.tv_nsec > BILLION ) {
    ts.tv_nsec -= BILLION;
    ts.tv_sec += 1;
  }
  retval = -pthread_cond_timedwait( &mVar, &mutex.mMutex, &ts );
#elif defined(CMAKE_USE_WIN32_THREADS_INIT)
  if( SleepConditionVariableCS( &mVar, &mutex.mMutex, timeoutMS ) == 0 ) {
    retval = GetLastError();
    if( retval == ERROR_TIMEOUT ) {
      return TIMED_OUT;
    }
  }
#endif

  return retval;
}

void Condition::signalOne()
{
#if defined(CMAKE_USE_PTHREADS_INIT)
  pthread_cond_signal( &mVar );
#elif defined(CMAKE_USE_WIN32_THREADS_INIT)
  WakeConditionVariable( &mVar );
#endif

}

void Condition::signalAll()
{
#if defined(CMAKE_USE_PTHREADS_INIT)
  pthread_cond_broadcast( &mVar );
#elif defined(CMAKE_USE_WIN32_THREADS_INIT)
  WakeAllConditionVariable( &mVar );
#endif

}

}