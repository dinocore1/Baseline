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

namespace baseline {

Mutex::Mutex()
{
#if defined(CMAKE_USE_PTHREADS_INIT)
  pthread_mutexattr_t type;
  pthread_mutexattr_settype( &type, PTHREAD_MUTEX_RECURSIVE );
  pthread_mutex_init( &mMutex, &type );
#elif defined(CMAKE_USE_WIN32_THREADS_INIT)
  InitializeCriticalSection( &mMutex );
#endif
}

Mutex::~Mutex()
{
#if defined(CMAKE_USE_PTHREADS_INIT)
  pthread_mutex_destroy( &mMutex );
#elif defined(CMAKE_USE_WIN32_THREADS_INIT)
  DeleteCriticalSection( &mMutex );
#endif
}

status_t Mutex::lock()
{
#if defined(CMAKE_USE_PTHREADS_INIT)
  pthread_mutex_lock( &mMutex );
#elif defined(CMAKE_USE_WIN32_THREADS_INIT)
  EnterCriticalSection( &mMutex );
#endif

  return OK;
}

void Mutex::unlock()
{
#if defined(CMAKE_USE_PTHREADS_INIT)
  pthread_mutex_unlock( &mMutex );
#elif defined(CMAKE_USE_WIN32_THREADS_INIT)
  LeaveCriticalSection( &mMutex );
#endif
}


}