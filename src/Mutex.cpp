#include "Private.h"

#include <baseline/Baseline.h>
#include <baseline/Mutex.h>

namespace baseline {

Mutex::Mutex()
{
#if defined(CMAKE_USE_PTHREADS_INIT)
  pthread_mutex_init(&mMutex);
#elif defined(CMAKE_USE_WIN32_THREADS_INIT)
  InitializeCriticalSection(&mMutex);
#endif
}

Mutex::~Mutex()
{
#if defined(CMAKE_USE_PTHREADS_INIT)
  pthread_mutex_destroy(&mMutex);
#elif defined(CMAKE_USE_WIN32_THREADS_INIT)
  DeleteCriticalSection(&mMutex);
#endif
}

status_t Mutex::lock()
{
#if defined(CMAKE_USE_PTHREADS_INIT)
  pthread_mutex_lock(&mMutex);
#elif defined(CMAKE_USE_WIN32_THREADS_INIT)
  EnterCriticalSection(&mMutex);
#endif

  return OK;
}

void Mutex::unlock()
{
#if defined(CMAKE_USE_PTHREADS_INIT)
  pthread_mutex_unlock(&mMutex);
#elif defined(CMAKE_USE_WIN32_THREADS_INIT)
  LeaveCriticalSection(&mMutex);
#endif
}


}