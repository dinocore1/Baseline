#include "Private.h"

#include <baseline/Baseline.h>
#include <baseline/Condition.h>

namespace baseline {

  Condition::Condition() {
#if defined(CMAKE_USE_PTHREADS_INIT)
    pthread_cond_init(&mVar, NULL);
#elif defined(CMAKE_USE_WIN32_THREADS_INIT)
    InitializeConditionVariable(&mVar);
#endif
  }

  Condition::~Condition() {
#if defined(CMAKE_USE_PTHREADS_INIT)
    pthread_cond_destroy(&mVar);
#elif defined(CMAKE_USE_WIN32_THREADS_INIT)
    
#endif
  }
status_t Condition::wait( Mutex& mutex )
{
#if defined(CMAKE_USE_PTHREADS_INIT)
  pthread_cond_wait(&mVar, &mutex.mMutex);
#elif defined(CMAKE_USE_WIN32_THREADS_INIT)
  SleepConditionVariableCS(&mVar, &mutex.mMutex, INFINITE);
#endif

  return OK;
}

void Condition::signalOne()
{
#if defined(CMAKE_USE_PTHREADS_INIT)
  pthread_cond_signal(&mVar);
#elif defined(CMAKE_USE_WIN32_THREADS_INIT)
  WakeConditionVariable(&mVar);
#endif

}

void Condition::signalAll()
{
#if defined(CMAKE_USE_PTHREADS_INIT)
  pthread_cond_broadcast(&mVar);
#elif defined(CMAKE_USE_WIN32_THREADS_INIT)
  WakeAllConditionVariable(&mVar);
#endif

}

}