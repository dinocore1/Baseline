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
	WorkerThread(Mutex&, Condition&, Vector<Runnable*>&);
  void run();

  Mutex& mMutex;
  Condition& mCondition;
  Vector<Runnable*>& mQueue;
  bool mRunning;

};

WorkerThread::WorkerThread(Mutex& mutex, Condition& condition, Vector<Runnable*>& queue)
	: mMutex(mutex), mCondition(condition), mQueue(queue)
{

}

void WorkerThread::run()
{
	Mutex::Autolock l(mMutex);
  
  while( mRunning ) {
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
  sp<WorkerThread> mThread;

};

ExecutorServiceImpl::ExecutorServiceImpl( const String8& name )
  : mName( name )
{
  mQueue.setCapacity( 10 );
  mThread = sp<WorkerThread>(new WorkerThread(mMutex, mCondition, mQueue));
}

void ExecutorServiceImpl::start()
{
	Mutex::Autolock l(mMutex);
	mThread->mRunning = true;
	mThread->start();
}

void ExecutorServiceImpl::shutdown()
{
	{
		Mutex::Autolock l(mMutex);
		if (mThread->mRunning) {
			mThread->mRunning = false;
			mCondition.signalAll();

		}
	}

	mThread->join();
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