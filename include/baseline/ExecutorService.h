#ifndef EXECUTORSERVICE_H_
#define EXECUTORSERVICE_H_

#include <baseline/String8.h>
#include <baseline/StrongPointer.h>
#include <baseline/RefBase.h>

namespace baseline {

class Runnable
{
public:
  virtual ~Runnable() {};
  virtual void run() = 0;
};

class Future : public RefBase
{
public:
  virtual void wait();
  virtual void cancel();
};

class ExecutorService : public RefBase
{
public:

  static sp<ExecutorService> createSingleThread( const String8& name );

  virtual void shutdown() = 0;
  virtual sp<Future> execute( Runnable* ) = 0;

};

}

#endif // EXECUTORSERVICE_H_