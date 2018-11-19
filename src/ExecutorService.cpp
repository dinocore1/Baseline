#include <baseline/Baseline.h>
#include <baseline/String8.h>
#include <baseline/ExecutorService.h>
#include <baseline/Vector.h>
#include <baseline/Thread.h>
#include <baseline/Mutex.h>
#include <baseline/Condition.h>

namespace baseline {

class DLL_LOCAL WorkerThread : public Thread
{
public:
  void run();

  Mutex& mMutex;
  Condition& mCondition;
  Vector<Runnable*>& mQueue;
  bool mRunning;

};

void WorkerThread::run()
{

  mRunning = true;
  while( mRunning ) {
    Mutex::Autolock l( mMutex );
    if( mQueue.isEmpty() ) {
      mCondition.wait( mMutex );
    } else {
      Runnable* r = mQueue[0];
      mQueue.pop();

      mMutex.unlock();
      r->run();
      delete r;
      mMutex.lock();
    }
  }

}

class DLL_LOCAL ExecutorServiceImpl : public ExecutorService
{
public:
  ExecutorServiceImpl( const String8& name );

  void shutdown();
  void execute( Runnable* task );
  void start();

  String8 mName;
  Mutex mMutex;
  Condition mCondition;
  Vector<Runnable*> mQueue;

};

ExecutorServiceImpl::ExecutorServiceImpl( const String8& name )
  : mName( name )
{
  mQueue.setCapacity( 10 );
}

void ExecutorServiceImpl::shutdown()
{

}

void ExecutorServiceImpl::execute( Runnable* task )
{
  Mutex::Autolock l( mMutex );
  mQueue.push_back( task );
  mCondition.signalOne();
}


sp<ExecutorService> ExecutorService::createSingleThread( const String8& name )
{
  sp<ExecutorServiceImpl> retval( new ExecutorServiceImpl( name ) );
  retval->start();

  return retval;
}

}