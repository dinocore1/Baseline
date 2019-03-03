/*
 * Copyright (C) 2018 Baseline
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
 */

#include <baseline/Baseline.h>
#include <baseline/Log.h>
#include <baseline/String8.h>
#include <baseline/Vector.h>
#include <baseline/ExecutorService.h>
#include <baseline/Thread.h>
#include <baseline/Mutex.h>
#include <baseline/Condition.h>
#include <baseline/UniquePointer.h>

#include <time.h>

namespace baseline {

static
int64_t getTime()
{
  int64_t retval;
#ifdef WIN32
  retval = GetTickCount64();
#else
  struct timespec ts;
  clock_gettime( CLOCK_MONOTONIC, &ts );
  retval = ts.tv_sec * 1000;
  retval += ts.tv_nsec / 1000000;
#endif
  return retval;
}

enum struct DLL_LOCAL TaskState {
  Queued,
  Running,
  Canceled,
  Finished
};

enum struct DLL_LOCAL ExecutorState {
  Ready,
  Running,
  ShuttingDown,
  Stopped
};

void Future::wait()
{}

void Future::cancel()
{}

class WorkTask;
class ExecutorServiceImpl;

class DLL_LOCAL WorkerThread : public Thread
{
public:
  WorkerThread( ExecutorServiceImpl& exeService )
    : mExeService( exeService ) {}
  void run();

  ExecutorServiceImpl& mExeService;
};

class DLL_LOCAL ExecutorServiceImpl : public ExecutorService
{
public:
  ExecutorServiceImpl( const String8& name, int numThreads );

  void shutdown() override;
  sp<Future> execute( const sp<Runnable>& task ) override;
  sp<Future> schedule( const sp<Runnable>& task, uint32_t delayMS ) override;
  sp<Future> scheduleWithFixedDelay( const sp<Runnable>& task, uint32_t delayMS ) override;
  void start();

  String8 mName;
  Mutex mMutex;
  Condition mCondition;
  ExecutorState mState;
  Vector<sp<WorkTask>> mQueue;
  Vector<sp<WorkerThread>> mThreads;

};

class DLL_LOCAL WorkTask : public Future
{
public:

  ExecutorServiceImpl& mExeService;
  sp<Runnable> mRunnable;
  TaskState mState;
  int64_t mExecuteTime;

  WorkTask( ExecutorServiceImpl& exeService, const sp<Runnable>& runnable )
    : mExeService( exeService ), mRunnable( runnable ), mState( TaskState::Queued )
  {}

  ~WorkTask() {}

  virtual void run() = 0;

  void wait() {
    Mutex::Autolock l( mExeService.mMutex );
    while( mState == TaskState::Queued || mState == TaskState::Running ) {
      mExeService.mCondition.wait( mExeService.mMutex );
    }
  }

  void cancel() {
    Mutex::Autolock l( mExeService.mMutex );
    if( mState == TaskState::Queued || mState == TaskState::Running ) {
      mState = TaskState::Canceled;
      mExeService.mCondition.signalAll();
    }
  }
};

static
int taskComparator( const sp<WorkTask>* a, const sp<WorkTask>* b )
{
  return a->get()->mExecuteTime - b->get()->mExecuteTime;
}

class DLL_LOCAL OneTimeTask : public WorkTask
{
public:

  OneTimeTask( ExecutorServiceImpl& exeService, const sp<Runnable>& runnable )
    : WorkTask( exeService, runnable )
  {}

  void run() {
    if( mState == TaskState::Queued ) {
      mState = TaskState::Running;
      mExeService.mMutex.unlock();
      mRunnable->run();
      mExeService.mMutex.lock();
      mState = TaskState::Finished;
    }
  }
};

class DLL_LOCAL RepeatTask : public WorkTask
{
public:

  RepeatTask( ExecutorServiceImpl& exeService, const sp<Runnable>& runnable, uint32_t delayMS )
    : WorkTask( exeService, runnable ), mDelayMS( delayMS )
  {}

  void run() {

    if( mState == TaskState::Canceled ) {
      mState = TaskState::Finished;
      return;
    }

    if( mState == TaskState::Queued ) {
      mState = TaskState::Running;
      mExeService.mMutex.unlock();
      mRunnable->run();
      mExeService.mMutex.lock();
      if( mState == TaskState::Running || mState == TaskState::Queued ) {
        mExecuteTime = getTime() + mDelayMS;
        mExeService.mQueue.push_back( this );
        mExeService.mQueue.sort( taskComparator );
        mState = TaskState::Queued;
      }
    }



  }

private:
  uint32_t mDelayMS;
};

void WorkerThread::run()
{
  Mutex::Autolock l( mExeService.mMutex );

  while( mExeService.mState == ExecutorState::Running ) {
    if( mExeService.mQueue.isEmpty() ) {
      mExeService.mCondition.waitTimeout( mExeService.mMutex, 500 );
    } else {
      sp<WorkTask> r = mExeService.mQueue[0];

      const int64_t now = getTime();
      int msDelay = r->mExecuteTime - now;
      if( msDelay <= 0 ) {
        mExeService.mQueue.removeAt( 0 );
        r->run();
        mExeService.mCondition.signalAll();
      } else {
        mExeService.mCondition.waitTimeout( mExeService.mMutex, msDelay );
      }

    }
  }

}


ExecutorServiceImpl::ExecutorServiceImpl( const String8& name, int numThreads )
  : mName( name ), mState( ExecutorState::Ready )
{
  mQueue.setCapacity( 10 );
  mThreads.setCapacity( numThreads );
  for( int i = 0; i < numThreads; i++ ) {
    mThreads.add( new WorkerThread( *this ) );
  }
}

void ExecutorServiceImpl::start()
{
  Mutex::Autolock l( mMutex );
  if( mState != ExecutorState::Ready ) {
    LOG_ERROR( "ExecutorService", "not in Ready state" );
    return;
  }

  mState = ExecutorState::Running;

  for( size_t i = 0; i < mThreads.size(); i++ ) {
    mThreads[i]->start();
  }

}

void ExecutorServiceImpl::shutdown()
{
  {
    Mutex::Autolock l( mMutex );
    if( mState != ExecutorState::Running ) {
      LOG_ERROR( "ExecutorService", "not in running state" );
      return;
    }

    mState = ExecutorState::ShuttingDown;

    while( !mQueue.isEmpty() ) {
      sp<WorkTask> task = mQueue[0];
      mQueue.pop();
      task->cancel();
      task->wait();
    }

    mCondition.signalAll();


  }

  for( size_t i = 0; i < mThreads.size(); i++ ) {
    mThreads[i]->join();
  }

  {
    Mutex::Autolock l( mMutex );
    mState = ExecutorState::Stopped;
  }
}

sp<Future> ExecutorServiceImpl::execute( const sp<Runnable>& runnable )
{
  return schedule( runnable, 0 );
}

sp<Future> ExecutorServiceImpl::schedule( const sp<Runnable>& runnable, uint32_t delayMS )
{
  if( mState != ExecutorState::Running ) {
    LOG_ERROR( "ExecutorService", "not in running state" );
    return nullptr;
  }
  sp<WorkTask> task( new OneTimeTask( *this, runnable ) );
  task->mExecuteTime = getTime();
  task->mExecuteTime += delayMS;

  Mutex::Autolock l( mMutex );
  mQueue.push_back( task );
  mQueue.sort( taskComparator );
  mCondition.signalOne();
  return task;
}

sp<Future> ExecutorServiceImpl::scheduleWithFixedDelay( const sp<Runnable>& runnable, uint32_t delayMS )
{
  if( mState != ExecutorState::Running ) {
    LOG_ERROR( "ExecutorService", "not in running state" );
    return nullptr;
  }
  sp<WorkTask> task( new RepeatTask( *this, runnable, delayMS ) );
  task->mExecuteTime = getTime();
  task->mExecuteTime += delayMS;

  Mutex::Autolock l( mMutex );
  mQueue.push_back( task );
  mQueue.sort( taskComparator );
  mCondition.signalOne();
  return task;
}


sp<ExecutorService> ExecutorService::createExecutorService( const String8& name, int numThreads )
{
  sp<ExecutorServiceImpl> retval( new ExecutorServiceImpl( name, numThreads ) );
  retval->start();

  return retval;
}

}