#include "Private.h"
#include <baseline/Baseline.h>
#include <baseline/Mutex.h>
#include <baseline/Condition.h>
#include <baseline/Thread.h>

#include <process.h>

namespace baseline {


  struct ThreadData {
    ThreadData(Thread* t)
      : mThread(t), mRunning(false) {}

    Thread* mThread;
    bool mRunning;
    Mutex mLock;
    Condition mThreadExitedCondition;
    unsigned threadId;
  };

  inline ThreadData* toThreadData(void* prt)
  {
    return static_cast<ThreadData*>(prt);
  }

  inline Thread* toThread(void* ptr)
  {
    return static_cast<Thread*>(ptr);
  }

  static unsigned trampoline(void* data)
  {
    ThreadData* threadData = toThreadData(data);
    
    threadData->mThread->run();
    {
      Mutex::Autolock l(threadData->mLock);
      threadData->mRunning = false;
      threadData->mThreadExitedCondition.signalAll();
    }
    threadData->mThread->decStrong(data);

    return 0;
  }

  Thread::Thread()
    : mData(new ThreadData(this))
  {}

  Thread::~Thread()
  {
    ThreadData* data = static_cast<ThreadData*>(mData);
    delete data;
    mData = nullptr;
  }

  status_t Thread::start()
  {
    incStrong(this);
    ThreadData* data = toThreadData(mData);
    data->mRunning = true;
    _beginthreadex(NULL, 0, trampoline, data, NULL, &data->threadId);
    return OK;
  }

  status_t Thread::join()
  {
    incStrong(this);
    ThreadData* data = toThreadData(mData);
    Mutex::Autolock l(data->mLock);
    while (data->mRunning) {
      data->mThreadExitedCondition.wait(data->mLock);
    }

    decStrong(this);
    return OK;
  }


}