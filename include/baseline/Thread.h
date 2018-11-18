#ifndef THREADS_H_
#define THREADS_H_

#include <baseline/RefBase.h>

namespace baseline {

class Thread : public RefBase
{
public:
  Thread();
  virtual ~Thread();
  virtual void run() = 0;

  status_t start();
  status_t join();

private:
  void* mData;
};

}

#endif // THREADS_H_