#include <baseline/Baseline.h>
#include <baseline/String8.h>
#include <baseline/Vector.h>
#include <baseline/ExecutorService.h>
#include <baseline/Thread.h>
#include <baseline/Mutex.h>
#include <baseline/Condition.h>
#include <baseline/UniquePointer.h>

namespace baseline {

enum DLL_LOCAL State {
  Queued,
  Running,
  Canceled,
  Finished
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
  bool mRunning;
};

class DLL_LOCAL ExecutorServiceImpl : public ExecutorService
{
public:
  ExecutorServiceImpl( const String8& name );

  void shutdown();
  sp<Future> execute( Runnable* task );
  void start();

  String8 mName;
  Mutex mMutex;
  Condition mCondition;
  Vector<sp<WorkTask>> mQueue;
  sp<WorkerThread> mThread;

};

class DLL_LOCAL WorkTask : public Future
{
public:

  ExecutorServiceImpl& mExeService;
  up<Runnable> mRunnable;
  State mState;

  WorkTask( ExecutorServiceImpl& exeService, Runnable* runnable )
    : mExeService( exeService ), mRunnable( runnable ), mState( State::Queued )
  {}

  ~WorkTask() {}

  void wait() {
    Mutex::Autolock l( mExeService.mMutex );
    while( mState == State::Queued || mState == State::Running ) {
      mExeService.mCondition.wait( mExeService.mMutex );
    }
  }

  void cancel() {
    Mutex::Autolock l( mExeService.mMutex );
    if( mState == State::Queued ) {
      mState = State::Canceled;
    }
  }
};


void WorkerThread::run()
{
  Mutex::Autolock l( mExeService.mMutex );

  while( mRunning ) {
    if( mExeService.mQueue.isEmpty() ) {
      mExeService.mCondition.wait( mExeService.mMutex );
    } else {
      sp<WorkTask> r = mExeService.mQueue[0];
      mExeService.mQueue.pop();

      if( r->mState == State::Queued ) {
        r->mState = State::Running;
        mExeService.mMutex.unlock();
        r->mRunnable->run();
        mExeService.mMutex.lock();
        r->mState = State::Finished;
      }

      mExeService.mCondition.signalAll();

    }
  }

}


ExecutorServiceImpl::ExecutorServiceImpl( const String8& name )
  : mName( name ), mThread( new WorkerThread( *this ) )
{
  mQueue.setCapacity( 10 );
}

void ExecutorServiceImpl::start()
{
  Mutex::Autolock l( mMutex );
  mThread->mRunning = true;
  mThread->start();
}

void ExecutorServiceImpl::shutdown()
{
  {
    Mutex::Autolock l( mMutex );
    if( mThread->mRunning ) {
      mThread->mRunning = false;
      mCondition.signalAll();

    }
  }

  mThread->join();
}

sp<Future> ExecutorServiceImpl::execute( Runnable* runnable )
{
  sp<WorkTask> task( new WorkTask( *this, runnable ) );
  Mutex::Autolock l( mMutex );
  mQueue.push_back( task );
  mCondition.signalOne();
  return task;
}


sp<ExecutorService> ExecutorService::createSingleThread( const String8& name )
{
  sp<ExecutorServiceImpl> retval( new ExecutorServiceImpl( name ) );
  retval->start();

  return retval;
}

}