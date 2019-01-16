#include <baseline/Baseline.h>
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

  void shutdown() override;
  sp<Future> execute( const sp<Runnable>& task ) override;
  sp<Future> schedule( const sp<Runnable>& task, uint32_t delayMS ) override;
  sp<Future> scheduleWithFixedDelay( const sp<Runnable>& task, uint32_t delayMS ) override;
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
  sp<Runnable> mRunnable;
  State mState;
  int64_t mExecuteTime;

  WorkTask( ExecutorServiceImpl& exeService, const sp<Runnable>& runnable )
    : mExeService( exeService ), mRunnable( runnable ), mState( State::Queued )
  {}

  ~WorkTask() {}

  virtual void run() = 0;

  void wait() {
    Mutex::Autolock l( mExeService.mMutex );
    while( mState == State::Queued || mState == State::Running ) {
      mExeService.mCondition.wait( mExeService.mMutex );
    }
  }

  void cancel() {
    Mutex::Autolock l( mExeService.mMutex );
    if( mState == State::Queued || mState == State::Running ) {
      mState = State::Canceled;
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
    if( mState == State::Queued ) {
      mState = State::Running;
      mExeService.mMutex.unlock();
      mRunnable->run();
      mExeService.mMutex.lock();
      mState = State::Finished;
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

    if( mState == State::Canceled ) {
      mState = State::Finished;
      return;
    }

    if( mState == State::Queued ) {
      mState = State::Running;
      mExeService.mMutex.unlock();
      mRunnable->run();
      mExeService.mMutex.lock();
      if( mState == State::Running || mState == State::Queued ) {
        mExecuteTime = getTime() + mDelayMS;
        mExeService.mQueue.push_back( this );
        mExeService.mQueue.sort( taskComparator );
        mState = State::Queued;
      }
    }



  }

private:
  uint32_t mDelayMS;
};

void WorkerThread::run()
{
  Mutex::Autolock l( mExeService.mMutex );

  while( mRunning ) {
    if( mExeService.mQueue.isEmpty() ) {
      mExeService.mCondition.wait( mExeService.mMutex );
    } else {
      sp<WorkTask> r = mExeService.mQueue[0];

      const int64_t now = getTime();
      int msDelay = r->mExecuteTime - now;
      if( msDelay <= 0 ) {
        mExeService.mQueue.removeAt(0);
        r->run();
        mExeService.mCondition.signalAll();
      } else {
        mExeService.mCondition.waitTimeout( mExeService.mMutex, msDelay );
      }

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

    while(!mQueue.isEmpty()) {
      sp<WorkTask> task = mQueue[0];
      mQueue.pop();
      task->cancel();
      task->wait();
    }

    if( mThread->mRunning ) {
      mThread->mRunning = false;
      mCondition.signalAll();

    }
  }

  mThread->join();
}

sp<Future> ExecutorServiceImpl::execute( const sp<Runnable>& runnable )
{
  return schedule( runnable, 0 );
}

sp<Future> ExecutorServiceImpl::schedule( const sp<Runnable>& runnable, uint32_t delayMS )
{
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
  sp<WorkTask> task( new RepeatTask( *this, runnable, delayMS ) );
  task->mExecuteTime = getTime();
  task->mExecuteTime += delayMS;

  Mutex::Autolock l( mMutex );
  mQueue.push_back( task );
  mQueue.sort( taskComparator );
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