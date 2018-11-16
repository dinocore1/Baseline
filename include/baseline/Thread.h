#ifndef THREADS_H_
#define THREADS_H_

namespace baseline {

class Thread
{
public:
  virtual ~Thread();
  virtual void run() = 0;

  void start();
  void join();
};

}

#endif // THREADS_H_