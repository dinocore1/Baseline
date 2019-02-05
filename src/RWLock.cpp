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
#include <baseline/RWLock.h>

#ifdef WIN32
  #define MODE_READ 0
  #define MODE_WRITE 1
#endif

namespace baseline {

RWLock::RWLock()
{
#if defined(CMAKE_USE_PTHREADS_INIT)
  pthread_rwlock_init( &mRWLock, NULL );
#elif defined(CMAKE_USE_WIN32_THREADS_INIT)
  InitializeSRWLock( &mRWLock );
#endif
}

RWLock::~RWLock()
{
#if defined(CMAKE_USE_PTHREADS_INIT)
  pthread_rwlock_destroy( &mRWLock );
#elif defined(CMAKE_USE_WIN32_THREADS_INIT)
#endif
}

status_t RWLock::readLock()
{
#if defined(CMAKE_USE_PTHREADS_INIT)
  return -pthread_rwlock_rdlock( &mRWLock );
#elif defined(CMAKE_USE_WIN32_THREADS_INIT)
  AcquireSRWLockShared( &mRWLock );
  mMode = MODE_READ;
  return OK;
#endif
}

status_t RWLock::writeLock()
{
#if defined(CMAKE_USE_PTHREADS_INIT)
  return -pthread_rwlock_wrlock( &mRWLock );
#elif defined(CMAKE_USE_WIN32_THREADS_INIT)
  AcquireSRWLockExclusive( &mRWLock );
  mMode = MODE_WRITE;
  return OK;
#endif
}

void RWLock::unlock()
{
#if defined(CMAKE_USE_PTHREADS_INIT)
  pthread_rwlock_unlock( &mRWLock );
#elif defined(CMAKE_USE_WIN32_THREADS_INIT)
  if( mMode == MODE_READ ) {
    ReleaseSRWLockShared( &mRWLock );
  } else {
    ReleaseSRWLockExclusive( &mRWLock );
  }
#endif
}

} // baseline
