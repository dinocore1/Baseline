#ifndef EXECUTORSERVICE_H_
#define EXECUTORSERVICE_H_

#include <baseline/RefBase.h>

namespace baseline {

class Runnable
{
public:
  virtual ~Runnable();
  virtual void run() = 0;
};

class ExecutorService : public RefBase
{
public:

  static sp<ExecutorService> createSingleThread( const String8& name );

  void shutdown();
  void execute( Runnable* );

};

}

#endif // EXECUTORSERVICE_H_