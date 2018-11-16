#ifndef CONDITION_H_
#define CONDITION_H_

#include <baseline/Mutex.h>

namespace baseline {

class Condition
{
public:
  status_t wait( Mutex& mutex );
//status_t waitRelative(Mutex& mutex, nsecs_t reltime);

  /**
   * Signal one thread to wakeup
   */
  void signalOne();

  /**
   * Signal all threads to wakeup
   */
  void signalAll();

private:
  void* mData;

};

}

#endif // CONDITION_H_